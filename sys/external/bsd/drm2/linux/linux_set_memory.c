/*
 * Linux set_pages_array_{wc,uc,wb} implementation for NetBSD
 */

#include <sys/cdefs.h>

#include <sys/param.h>
#include <sys/types.h>
#include <sys/errno.h>
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

#define	LINUX_CPA_RETRIES	32

	static u_int
linux_mode_to_pmap_flags(enum linux_page_cache_mode mode)
{

	switch (mode) {
		case LINUX_PCCM_WC:
			return PMAP_WRITE_COMBINE;
		case LINUX_PCCM_UC:
			return PMAP_NOCACHE;
		case LINUX_PCCM_WB:
	default:
		return PMAP_WRITE_BACK;
	}
}

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
	x86_mfence();
	for (v = start; v < end; v += clsize)
		asm volatile ("clflush %0" : : "m" (*(const char *)v));
	x86_mfence();
#else
	/* TODO: not clear if this code will be used on non-amd64 */
	(void)page;
#endif
}

/*
 * Change the amd64 direct-map PTE cache mode via pmap_page_set_memattr()
 * so there is a single cache type for the physical page, no WB+WC alias.
 * Retry transient failures (EAGAIN races, ENOMEM for the demote PTP).
 * Propagate fatal errors to the caller.
 *
 * TTM vmap() already passes PMAP_WRITE_COMBINE via ttm_prot_from_caching().
 */
static int
linux_pmap_page_set_memattr_retry(paddr_t paddr, u_int pmap_flags)
{
	int error = 0;
	unsigned try;

	for (try = 0; try < LINUX_CPA_RETRIES; try++) {
		/* TODO: sleep before retrying last time */
		error = pmap_page_set_memattr(paddr, pmap_flags);
		if (error == 0)
			return 0;
		if (error != EAGAIN && error != ENOMEM)
			return error;
	}
	return error;
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
		u_int pmap_flags;
		int error;

		if (page == NULL)
			continue;

		paddr = VM_PAGE_TO_PHYS(&page->p_vmp);
		pmap_flags = linux_mode_to_pmap_flags(mode);

		 /* clflush only while the page is still WB */
		if (mode != LINUX_PCCM_WB)
			linux_clflush_page_direct(page);
#if defined(__i386__) || defined(__x86_64__)
		else
			x86_sfence();
#endif

		error = linux_pmap_page_set_memattr_retry(paddr, pmap_flags);
		if (error)
			return error;

#if defined(__i386__) || defined(__x86_64__)
		if (mode == LINUX_PCCM_WC || mode == LINUX_PCCM_UC)
			x86_sfence();
#endif
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
	 * ttm_pool_free_page.
	 */
	for (i = 0; i < numpages; i++) {
		struct page *p = page + i;
		paddr_t paddr = VM_PAGE_TO_PHYS(&p->p_vmp);
		int error;

#if defined(__i386__) || defined(__x86_64__)
		x86_sfence(); /* drain WC before restoring WB PTE */
#endif
		error = linux_pmap_page_set_memattr_retry(paddr,
		    PMAP_WRITE_BACK);
		if (error)
			return error;
	}
	return 0;
}

/* TODO: cleanup below */

int
linux_set_memory_init(void)
{

	return 0;
}

void
linux_set_memory_fini(void)
{
}
