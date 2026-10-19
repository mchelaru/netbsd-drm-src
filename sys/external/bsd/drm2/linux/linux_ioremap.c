/*
 * ioremap / ioremap_wc / ioremap_cache that obey PAT flags
 */

#include <sys/cdefs.h>

#include <sys/param.h>
#include <sys/types.h>
#include <sys/kmem.h>
#include <sys/mutex.h>
#include <sys/rbtree.h>
#include <sys/systm.h>

#include <uvm/uvm_extern.h>
#include <machine/pmap.h>

#include <linux/io.h>
#include <linux/types.h>

struct linux_ioremap_entry {
	vaddr_t		lie_va;
	vsize_t		lie_size;
	rb_node_t	lie_node;
};

static kmutex_t linux_ioremap_lock;
static rb_tree_t linux_ioremap_tree;
static bool linux_ioremap_ready;

static int
lie_compare_nodes(void *ctx __unused, const void *an, const void *bn)
{
	const struct linux_ioremap_entry *a = an;
	const struct linux_ioremap_entry *b = bn;

	if (a->lie_va < b->lie_va)
		return -1;
	if (a->lie_va > b->lie_va)
		return +1;
	return 0;
}

static int
lie_compare_key(void *ctx __unused, const void *node, const void *key)
{
	const struct linux_ioremap_entry *lie = node;
	const vaddr_t *vap = key;

	if (lie->lie_va < *vap)
		return -1;
	if (lie->lie_va > *vap)
		return +1;
	return 0;
}

static const rb_tree_ops_t linux_ioremap_ops = {
	.rbto_compare_nodes = lie_compare_nodes,
	.rbto_compare_key = lie_compare_key,
	.rbto_node_offset = offsetof(struct linux_ioremap_entry, lie_node),
	.rbto_context = NULL,
};

void
linux_ioremap_init(void)
{

	mutex_init(&linux_ioremap_lock, MUTEX_DEFAULT, IPL_NONE);
	rb_tree_init(&linux_ioremap_tree, &linux_ioremap_ops);
	linux_ioremap_ready = true;
}

void
linux_ioremap_fini(void)
{

	KASSERT(RB_TREE_MIN(&linux_ioremap_tree) == NULL);
	linux_ioremap_ready = false;
	mutex_destroy(&linux_ioremap_lock);
}

static void *
linux_ioremap_flags(resource_size_t offset, unsigned long size, u_int pmapflags)
{
	struct linux_ioremap_entry *lie, *collision;
	vaddr_t va;
	vsize_t vsize;
	paddr_t pa, end;
	uint64_t off;

	if (size == 0)
		return NULL;

	pa = (paddr_t)offset;
	end = pa + (paddr_t)size;
	if (end < pa)
		return NULL;

	off = (uint64_t)(pa & (PAGE_SIZE - 1));
	pa &= ~(paddr_t)(PAGE_SIZE - 1);
	vsize = (vsize_t)((off + size + PAGE_SIZE - 1) &
	    ~(uint64_t)(PAGE_SIZE - 1));

	va = uvm_km_alloc(kernel_map, vsize, PAGE_SIZE,
	    UVM_KMF_VAONLY | UVM_KMF_NOWAIT);
	if (va == 0) {
		va = uvm_km_alloc(kernel_map, vsize, PAGE_SIZE,
		    UVM_KMF_VAONLY | UVM_KMF_WAITVA);
	}
	if (va == 0)
		return NULL;

	for (off = 0; off < vsize; off += PAGE_SIZE) {
		pmap_kenter_pa(va + off, pa + off,
		    VM_PROT_READ | VM_PROT_WRITE, pmapflags);
	}
	pmap_update(pmap_kernel());

	lie = kmem_alloc(sizeof(*lie), KM_SLEEP);
	lie->lie_va = va;
	lie->lie_size = vsize;

	mutex_enter(&linux_ioremap_lock);
	collision = rb_tree_insert_node(&linux_ioremap_tree, lie);
	KASSERT(collision == lie);
	mutex_exit(&linux_ioremap_lock);

	/* Return pointer into the mapping at the original offset. */
	return (void *)(va + (vaddr_t)(offset & (PAGE_SIZE - 1)));
}

void *
ioremap(resource_size_t offset, unsigned long size)
{
	return linux_ioremap_flags(offset, size, PMAP_NOCACHE);
}

void *
ioremap_wc(resource_size_t offset, unsigned long size)
{
	return linux_ioremap_flags(offset, size, PMAP_WRITE_COMBINE);
}

void *
ioremap_cache(resource_size_t offset, unsigned long size)
{
	return linux_ioremap_flags(offset, size, PMAP_WRITE_BACK);
}

void
iounmap(volatile void *addr)
{
	struct linux_ioremap_entry *lie;
	vaddr_t key, va;

	if (addr == NULL || !linux_ioremap_ready)
		return;

	va = trunc_page((vaddr_t)addr);
	key = va;

	mutex_enter(&linux_ioremap_lock);
	lie = rb_tree_find_node(&linux_ioremap_tree, &key);
	if (lie == NULL) {
		/*
		 * Caller may have passed an interior pointer; find the
		 * covering mapping by walking predecessors.
		 */
		lie = rb_tree_find_node_leq(&linux_ioremap_tree, &key);
		if (lie == NULL ||
		    (vaddr_t)addr >= lie->lie_va + lie->lie_size)
			lie = NULL;
	}
	if (lie != NULL)
		rb_tree_remove_node(&linux_ioremap_tree, lie);
	mutex_exit(&linux_ioremap_lock);

	if (lie == NULL)
		return;

	pmap_kremove(lie->lie_va, lie->lie_size);
	pmap_update(pmap_kernel());
	uvm_km_free(kernel_map, lie->lie_va, lie->lie_size, UVM_KMF_VAONLY);
	kmem_free(lie, sizeof(*lie));
}
