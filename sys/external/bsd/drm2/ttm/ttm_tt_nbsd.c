#include <sys/cdefs.h>

#include <sys/param.h>
#include <sys/types.h>

#include <uvm/uvm_extern.h>
#include <uvm/uvm_object.h>
#include <uvm/uvm_page.h>

#include <linux/mm.h>

#include <drm/ttm/ttm_tt.h>
#include <drm/ttm/ttm_device.h>

/*
 * ttm_tt_wire(ttm)
 *
 *	Wire the uvm pages of ttm and fill the ttm page array.  ttm
 *	must be unpopulated, and must be marked swapped.  This does not
 *	change either state -- the caller is expected to include it
 *	among other operations for such a state transition.
 */
int
ttm_tt_wire(struct ttm_tt *ttm)
{
	struct uvm_object *uobj = ttm->swap_storage;
	struct vm_page *vm_page;
	unsigned i;
	int error;

	KASSERTMSG((ttm->state == tt_unpopulated),
	    "ttm_tt %p must be unpopulated for wiring, but state=%d",
	    ttm, (int)ttm->state);
	KASSERT(ISSET(ttm->page_flags, TTM_PAGE_FLAG_SWAPPED));
	KASSERT(uobj != NULL);

	error = uvm_obj_wirepages(uobj, 0, (ttm->num_pages << PAGE_SHIFT),
	    NULL);
	if (error)
		/* XXX errno NetBSD->Linux */
		return -error;

	rw_enter(uobj->vmobjlock, RW_READER);
	for (i = 0; i < ttm->num_pages; i++) {
		vm_page = uvm_pagelookup(uobj, ptoa(i));
		ttm->pages[i] = container_of(vm_page, struct page, p_vmp);
	}
	rw_exit(uobj->vmobjlock);

	/* Success!  */
	return 0;
}

/*
 * ttm_tt_unwire(ttm)
 *
 *	Nullify the ttm page array and unwire the uvm pages of ttm.
 *	ttm must be unbound and must be marked swapped.  This does not
 *	change either state -- the caller is expected to include it
 *	among other operations for such a state transition.
 */
void
ttm_tt_unwire(struct ttm_tt *ttm)
{
	struct uvm_object *uobj = ttm->swap_storage;
	unsigned i;

	KASSERTMSG((ttm->state == tt_unbound),
	    "ttm_tt %p must be unbound for unwiring, but state=%d",
	    ttm, (int)ttm->state);
	KASSERT(!ISSET(ttm->page_flags, TTM_PAGE_FLAG_SWAPPED));
	KASSERT(uobj != NULL);

	uvm_obj_unwirepages(uobj, 0, (ttm->num_pages << PAGE_SHIFT));
	for (i = 0; i < ttm->num_pages; i++)
		ttm->pages[i] = NULL;
}

int
ttm_dma_tt_init(struct ttm_dma_tt *ttm_dma, struct ttm_buffer_object *bo,
    uint32_t page_flags)
{
	int ret;

	ret = ttm_tt_init(&ttm_dma->ttm, bo, page_flags, ttm_cached, 0);
	if (ret)
		return ret;
	INIT_LIST_HEAD(&ttm_dma->pages_list);
	ttm_dma->dma_address = NULL;
	return 0;
}

void
ttm_dma_tt_fini(struct ttm_dma_tt *ttm_dma)
{
	ttm_tt_fini(&ttm_dma->ttm);
}
