/*
 * Linux set_pages_array_{wc,uc,wb} implementation for NetBSD
 *
 * Linux changes PAT on the direct map (4K CPA, large-page split).  NetBSD's
 * amd64 direct map is 2MB large pages withou change page attr support , so
 * we cannot flip  a single page's home mapping to WC.
 *
 * Instead:
 *  1. clflush the page via the current (WB) direct map so dirty lines land
 *     in DRAM before any WC alias is used;
 *  2. record the desired cache mode by paddr;
 *  3. linux_kmap / kmap_atomic skip the WB direct map for those pages and
 *     pmap_kenter_pa with PMAP_WRITE_COMBINE / PMAP_NOCACHE.
 *
 * TTM vmap() already passes PMAP_WRITE_COMBINE via ttm_prot_from_caching();
 * this makes single-page kmap and any direct-map-avoidance path match.
 */

#include <sys/cdefs.h>

#include <sys/param.h>
#include <sys/types.h>
#include <sys/kmem.h>
#include <sys/mutex.h>
#include <sys/rbtree.h>
#include <sys/systm.h>

#include <uvm/uvm_extern.h>
#include <uvm/uvm_page.h>

#include <machine/pmap.h>
#if defined(__i386__) || defined(__x86_64__)
#include <machine/cpufunc.h>
#include <machine/cpu.h>
#include <x86/specialreg.h>
#endif

#include <linux/mm.h>
#include <linux/mm_types.h>

#include <asm/set_memory.h>

enum linux_page_cache_mode {
	LINUX_PCCM_WB = 0,
	LINUX_PCCM_WC,
	LINUX_PCCM_UC,
};

struct linux_page_cache_entry {
	paddr_t				lpce_paddr;
	enum linux_page_cache_mode	lpce_mode;
	rb_node_t			lpce_node;
};

static kmutex_t linux_page_cache_lock;
static rb_tree_t linux_page_cache_tree;
static bool linux_page_cache_ready;

static int
lpce_compare_nodes(void *ctx __unused, const void *an, const void *bn)
{
	const struct linux_page_cache_entry *a = an;
	const struct linux_page_cache_entry *b = bn;

	if (a->lpce_paddr < b->lpce_paddr)
		return -1;
	if (a->lpce_paddr > b->lpce_paddr)
		return +1;
	return 0;
}

static int
lpce_compare_key(void *ctx __unused, const void *node, const void *key)
{
	const struct linux_page_cache_entry *lpce = node;
	const paddr_t *paddrp = key;

	if (lpce->lpce_paddr < *paddrp)
		return -1;
	if (lpce->lpce_paddr > *paddrp)
		return +1;
	return 0;
}

static const rb_tree_ops_t linux_page_cache_ops = {
	.rbto_compare_nodes = lpce_compare_nodes,
	.rbto_compare_key = lpce_compare_key,
	.rbto_node_offset = offsetof(struct linux_page_cache_entry, lpce_node),
	.rbto_context = NULL,
};

static void
linux_clflush_page_direct(struct page *page)
{
#if (defined(__i386__) || defined(__x86_64__)) && defined(__HAVE_DIRECT_MAP)
	paddr_t pa = VM_PAGE_TO_PHYS(&page->p_vmp);
	vaddr_t va = PMAP_DIRECT_MAP(pa);
	vaddr_t start, end, v;
	unsigned clsize;

	if (!ISSET(cpu_info_primary.ci_feat_val[0], CPUID_CLFSH)) {
		/* flush everything like there's no tomorrow */
		wbinvd();
		return;
	}
	clsize = cpu_info_primary.ci_cflush_lsize;
	KASSERT(clsize > 0);
	start = rounddown(va, clsize);
	end = roundup(va + PAGE_SIZE, clsize);
	x86_mfence(); /* make sure the data is in the cache */
	for (v = start; v < end; v += clsize)
		asm volatile ("clflush %0" : : "m" (*(const char *)v));
	x86_mfence(); /* make sure the writebacks are complete */
#else
	/* TODO: not clear if this code will be used on non-amd64 */
	(void)page;
#endif
}

static int
linux_page_cache_set(paddr_t paddr, enum linux_page_cache_mode mode)
{
	struct linux_page_cache_entry *lpce, *collision;

	mutex_enter(&linux_page_cache_lock);
	lpce = rb_tree_find_node(&linux_page_cache_tree, &paddr);
	if (mode == LINUX_PCCM_WB) {
		if (lpce != NULL) {
			rb_tree_remove_node(&linux_page_cache_tree, lpce);
			mutex_exit(&linux_page_cache_lock);
			kmem_free(lpce, sizeof(*lpce));
		} else {
			mutex_exit(&linux_page_cache_lock);
		}
		return 0;
	}
	if (lpce != NULL) {
		lpce->lpce_mode = mode;
		mutex_exit(&linux_page_cache_lock);
		return 0;
	}
	mutex_exit(&linux_page_cache_lock);

	lpce = kmem_alloc(sizeof(*lpce), KM_NOSLEEP);
	if (lpce == NULL)
		lpce = kmem_alloc(sizeof(*lpce), KM_SLEEP);
	lpce->lpce_paddr = paddr;
	lpce->lpce_mode = mode;

	mutex_enter(&linux_page_cache_lock);
	collision = rb_tree_insert_node(&linux_page_cache_tree, lpce);
	if (collision != lpce) {
		collision->lpce_mode = mode;
		mutex_exit(&linux_page_cache_lock);
		kmem_free(lpce, sizeof(*lpce));
	} else {
		mutex_exit(&linux_page_cache_lock);
	}
	return 0;
}

u_int
linux_page_pmap_cache_flags(paddr_t paddr)
{
	struct linux_page_cache_entry *lpce;
	u_int flags = 0;

	if (!linux_page_cache_ready)
		return 0;

	mutex_enter(&linux_page_cache_lock);
	lpce = rb_tree_find_node(&linux_page_cache_tree, &paddr);
	if (lpce != NULL) {
		switch (lpce->lpce_mode) {
		case LINUX_PCCM_WC:
			flags = PMAP_WRITE_COMBINE;
			break;
		case LINUX_PCCM_UC:
			flags = PMAP_NOCACHE;
			break;
		case LINUX_PCCM_WB:
		default:
			flags = 0;
			break;
		}
	}
	mutex_exit(&linux_page_cache_lock);
	return flags;
}

bool
linux_page_skip_direct_map(paddr_t paddr)
{
	return linux_page_pmap_cache_flags(paddr) != 0;
}

static int
linux_set_pages_array_mode(struct page **pages, int numpages,
    enum linux_page_cache_mode mode)
{
	int i;

	if (pages == NULL || numpages <= 0)
		return 0;

	for (i = 0; i < numpages; i++) {
		struct page *page = pages[i];
		paddr_t paddr;

		if (page == NULL)
			continue;
		/*
		 * Drop WB dirty lines before any WC
		 * mapping is installed
		 */
		if (mode != LINUX_PCCM_WB)
			linux_clflush_page_direct(page);
		paddr = VM_PAGE_TO_PHYS(&page->p_vmp);
		(void)linux_page_cache_set(paddr, mode);
	}
	return 0;
}

int
set_pages_array_wc(struct page **pages, int addrinarray)
{
	return linux_set_pages_array_mode(pages, addrinarray, LINUX_PCCM_WC);
}

int
set_pages_array_uc(struct page **pages, int addrinarray)
{
	return linux_set_pages_array_mode(pages, addrinarray, LINUX_PCCM_UC);
}

int
set_pages_array_wb(struct page **pages, int addrinarray)
{
	return linux_set_pages_array_mode(pages, addrinarray, LINUX_PCCM_WB);
}

int
set_pages_wb(struct page *page, int numpages)
{
	int i;

	if (page == NULL || numpages <= 0)
		return 0;
	/*
	 * Linux takes a contiguous page range.  NetBSD alloc_pages is
	 * order-0 only, so callers pass numpages==1 from
	 * ttm_pool_free_page, but yeah.
	 */
	for (i = 0; i < numpages; i++) {
		paddr_t paddr = VM_PAGE_TO_PHYS(&(page + i)->p_vmp);

		(void)linux_page_cache_set(paddr, LINUX_PCCM_WB);
	}
	return 0;
}

int
linux_set_memory_init(void)
{

	mutex_init(&linux_page_cache_lock, MUTEX_DEFAULT, IPL_NONE);
	rb_tree_init(&linux_page_cache_tree, &linux_page_cache_ops);
	linux_page_cache_ready = true;
	return 0;
}

void
linux_set_memory_fini(void)
{

	KASSERT(RB_TREE_MIN(&linux_page_cache_tree) == NULL);
	linux_page_cache_ready = false;
	mutex_destroy(&linux_page_cache_lock);
}
