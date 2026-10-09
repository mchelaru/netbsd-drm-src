/*	$NetBSD: ttm_bo_vm.c,v 1.28 2024/06/23 00:49:31 riastradh Exp $	*/

/*-
 * Copyright (c) 2014 The NetBSD Foundation, Inc.
 * All rights reserved.
 *
 * This code is derived from software contributed to The NetBSD Foundation
 * by Taylor R. Campbell.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE NETBSD FOUNDATION, INC. AND CONTRIBUTORS
 * ``AS IS'' AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED
 * TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
 * PURPOSE ARE DISCLAIMED.  IN NO EVENT SHALL THE FOUNDATION OR CONTRIBUTORS
 * BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 */

/**************************************************************************
 *
 * Copyright (c) 2006-2009 VMware, Inc., Palo Alto, CA., USA
 * All Rights Reserved.
 *
 * Permission is hereby granted, free of charge, to any person obtaining a
 * copy of this software and associated documentation files (the
 * "Software"), to deal in the Software without restriction, including
 * without limitation the rights to use, copy, modify, merge, publish,
 * distribute, sub license, and/or sell copies of the Software, and to
 * permit persons to whom the Software is furnished to do so, subject to
 * the following conditions:
 *
 * The above copyright notice and this permission notice (including the
 * next paragraph) shall be included in all copies or substantial portions
 * of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NON-INFRINGEMENT. IN NO EVENT SHALL
 * THE COPYRIGHT HOLDERS, AUTHORS AND/OR ITS SUPPLIERS BE LIABLE FOR ANY CLAIM,
 * DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR
 * OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE
 * USE OR OTHER DEALINGS IN THE SOFTWARE.
 *
 **************************************************************************/
/*
 * Authors: Thomas Hellstrom <thellstrom-at-vmware-dot-com>
 */

#include <sys/cdefs.h>
__KERNEL_RCSID(0, "$NetBSD: ttm_bo_vm.c,v 1.28 2024/06/23 00:49:31 riastradh Exp $");

#include <sys/types.h>

#include <uvm/uvm.h>
#include <uvm/uvm_extern.h>
#include <uvm/uvm_fault.h>

#include <linux/bitops.h>
#include <linux/sched.h>

#include <drm/drm_vma_manager.h>
#include <drm/ttm/ttm_bo.h>
#include <drm/ttm/ttm_placement.h>
#include <drm/ttm/ttm_tt.h>

/* use the same trick as dist/drm/i915/gem/i915_gem_mman.c */
int	pmap_enter_default(pmap_t, vaddr_t, paddr_t, vm_prot_t, unsigned);

static int	ttm_bo_uvm_lookup(struct ttm_device *, unsigned long,
		    unsigned long, struct ttm_buffer_object **);

static unsigned long
ttm_bo_io_mem_pfn(struct ttm_buffer_object *bo, unsigned long page_offset)
{
	struct ttm_device *bdev = bo->bdev;

	if (bdev->funcs->io_mem_pfn)
		return bdev->funcs->io_mem_pfn(bo, page_offset);

	return (bo->resource->bus.offset >> PAGE_SHIFT) + page_offset;
}

void
ttm_bo_uvm_reference(struct uvm_object *uobj)
{
	struct ttm_buffer_object *const bo = container_of(uobj,
	    struct ttm_buffer_object, uvmobj);

	(void)ttm_bo_get(bo);
}

void
ttm_bo_uvm_detach(struct uvm_object *uobj)
{
	struct ttm_buffer_object *bo = container_of(uobj,
	    struct ttm_buffer_object, uvmobj);

	ttm_bo_put(bo);
}

static int
ttm_bo_vm_fault_idle(struct ttm_buffer_object *bo, struct uvm_faultinfo *vmf)
{

	/*
	 * Quick non-stalling check for idle.
	 */
	if (__predict_true(dma_resv_test_signaled(bo->base.resv,
	    DMA_RESV_USAGE_KERNEL)))
		return 0;

	/*
	 * Avoid waiting for GPU with mmap locks held: drop locks, wait,
	 * and ask UVM to retry the fault.
	 */
	ttm_bo_get(bo);
	uvmfault_unlockall(vmf, vmf->entry->aref.ar_amap, NULL);
	(void)dma_resv_wait_timeout(bo->base.resv, DMA_RESV_USAGE_KERNEL,
	    true, MAX_SCHEDULE_TIMEOUT);
	dma_resv_unlock(bo->base.resv);
	ttm_bo_put(bo);
	return ERESTART;
}

static int
ttm_bo_uvm_reserve(struct ttm_buffer_object *bo, struct uvm_faultinfo *vmf)
{

	/*
	 * Work around locking order reversal in fault / nopfn between
	 * mmap locks and bo_reserve: trylock, and if it fails, retry the
	 * fault after waiting for the buffer to become unreserved.
	 */
	if (__predict_false(!dma_resv_trylock(bo->base.resv))) {
		ttm_bo_get(bo);
		uvmfault_unlockall(vmf, vmf->entry->aref.ar_amap, NULL);
		if (!dma_resv_lock_interruptible(bo->base.resv, NULL))
			dma_resv_unlock(bo->base.resv);
		ttm_bo_put(bo);
		return ERESTART;
	}

	/*
	 * Refuse to fault imported pages unless they are marked mappable.
	 */
	if (bo->ttm && (bo->ttm->page_flags & TTM_TT_FLAG_EXTERNAL)) {
		if (!(bo->ttm->page_flags & TTM_TT_FLAG_EXTERNAL_MAPPABLE)) {
			dma_resv_unlock(bo->base.resv);
			return EINVAL;	/* SIGBUS */
		}
	}

	return 0;
}

static int
ttm_bo_uvm_fault_reserved(struct uvm_faultinfo *vmf, vaddr_t vaddr,
    struct vm_page **pps, int npages, int centeridx, vm_prot_t access_type,
    int flags)
{
	struct uvm_object *const uobj = vmf->entry->object.uvm_obj;
	struct ttm_buffer_object *const bo = container_of(uobj,
	    struct ttm_buffer_object, uvmobj);
	struct ttm_device *const bdev = bo->bdev;
	struct ttm_tt *ttm = NULL;
	size_t size __diagused;
	voff_t uoffset;		/* offset in bytes into bo */
	unsigned startpage;	/* offset in pages into bo */
	unsigned i;
	vm_prot_t vm_prot = vmf->entry->protection; /* VM_PROT_* */
	pgprot_t prot = vm_prot; /* VM_PROT_* | PMAP_* cacheability flags */
	int err, ret;

	/*
	 * Wait for buffer data in transit, due to a pipelined move.
	 */
	ret = ttm_bo_vm_fault_idle(bo, vmf);
	if (__predict_false(ret != 0))
		return ret;

	err = ttm_mem_io_reserve(bdev, bo->resource);
	if (__predict_false(err != 0))
		return EINVAL;	/* SIGBUS */

	prot = ttm_io_prot(bo, bo->resource, prot);
	if (!bo->resource->bus.is_iomem) {
		struct ttm_operation_ctx ctx = {
			.interruptible = false,
			.no_wait_gpu = false,
			.force_alloc = true,
		};

		ttm = bo->ttm;
		size = (size_t)bo->ttm->num_pages << PAGE_SHIFT;
		err = ttm_bo_populate(bo, &ctx);
		if (err) {
			ret = ENOMEM;
			goto out;
		}
	} else {
		size = bo->resource->size;
	}

	KASSERT(vmf->entry->start <= vaddr);
	KASSERT((vmf->entry->offset & (PAGE_SIZE - 1)) == 0);
	KASSERT(vmf->entry->offset <= size);
	KASSERT((vaddr - vmf->entry->start) <= (size - vmf->entry->offset));
	KASSERTMSG(((size_t)npages << PAGE_SHIFT <=
		((size - vmf->entry->offset) - (vaddr - vmf->entry->start))),
	    "vaddr=%jx npages=%d bo=%p is_iomem=%d size=%zu"
	    " start=%jx offset=%jx",
	    (uintmax_t)vaddr, npages, bo, (int)bo->resource->bus.is_iomem, size,
	    (uintmax_t)vmf->entry->start, (uintmax_t)vmf->entry->offset);
	uoffset = (vmf->entry->offset + (vaddr - vmf->entry->start));
	startpage = (uoffset >> PAGE_SHIFT);
	for (i = 0; i < npages; i++) {
		paddr_t paddr;

		if ((flags & PGO_ALLPAGES) == 0 && i != centeridx)
			continue;
		if (pps[i] == PGO_DONTCARE)
			continue;
		if (!bo->resource->bus.is_iomem) {
			paddr = page_to_phys(ttm->pages[startpage + i]);
		} else if (bdev->funcs->io_mem_pfn) {
			paddr = (paddr_t)ttm_bo_io_mem_pfn(bo,
			    startpage + i) << PAGE_SHIFT;
		} else {
			const paddr_t cookie = bus_space_mmap(bdev->memt,
			    bo->resource->bus.offset,
			    (off_t)(startpage + i) << PAGE_SHIFT,
			    vm_prot, 0);

			paddr = pmap_phys_address(cookie);
		}
		ret = pmap_enter_default(vmf->orig_map->pmap, vaddr + i*PAGE_SIZE,
		    paddr, vm_prot, PMAP_CANFAIL | prot);
		if (ret) {
			KASSERT(ret != ERESTART);
			break;
		}
	}
	pmap_update(vmf->orig_map->pmap);
	ret = 0;		/* retry access in userland */
out:
	KASSERT(ret != ERESTART);
	return ret;
}

int
ttm_bo_uvm_fault(struct uvm_faultinfo *vmf, vaddr_t vaddr,
    struct vm_page **pps, int npages, int centeridx, vm_prot_t access_type,
    int flags)
{
	struct uvm_object *const uobj = vmf->entry->object.uvm_obj;
	struct ttm_buffer_object *const bo = container_of(uobj,
	    struct ttm_buffer_object, uvmobj);
	int ret;

	/* Thanks, uvm, but we don't need this lock.  */
	rw_exit(uobj->vmobjlock);

	/* Copy-on-write mappings make no sense for the graphics aperture.  */
	if (UVM_ET_ISCOPYONWRITE(vmf->entry)) {
		ret = EINVAL;	/* SIGBUS */
		goto out;
	}

	ret = ttm_bo_uvm_reserve(bo, vmf);
	if (ret) {
		/* ttm_bo_uvm_reserve already unlocked on ERESTART */
		KASSERTMSG(ret == ERESTART || ret == EINVAL, "ret=%d", ret);
		if (ret == ERESTART)
			return ret;
		goto out;
	}

	ret = ttm_bo_uvm_fault_reserved(vmf, vaddr, pps, npages, centeridx,
	    access_type, flags);
	if (ret == ERESTART)	/* already unlocked on ERESTART */
		return ret;

	dma_resv_unlock(bo->base.resv);

out:	uvmfault_unlockall(vmf, vmf->entry->aref.ar_amap, NULL);
	return ret;
}

int
ttm_bo_mmap_object(struct ttm_device *bdev, off_t offset, size_t size,
    vm_prot_t prot, struct uvm_object **uobjp, voff_t *uoffsetp,
    struct file *file)
{
	const unsigned long startpage = (offset >> PAGE_SHIFT);
	const unsigned long npages = (size >> PAGE_SHIFT);
	struct ttm_buffer_object *bo;
	int ret;

	KASSERT(0 == (offset & (PAGE_SIZE - 1)));
	KASSERT(0 == (size & (PAGE_SIZE - 1)));

	ret = ttm_bo_uvm_lookup(bdev, startpage, npages, &bo);
	if (ret)
		goto fail0;
	KASSERTMSG((drm_vma_node_start(&bo->base.vma_node) <= startpage),
	    "mapping npages=0x%jx @ pfn=0x%jx"
	    " from vma npages=0x%jx @ pfn=0x%jx",
	    (uintmax_t)npages,
	    (uintmax_t)startpage,
	    (uintmax_t)drm_vma_node_size(&bo->base.vma_node),
	    (uintmax_t)drm_vma_node_start(&bo->base.vma_node));
	KASSERTMSG((npages <= drm_vma_node_size(&bo->base.vma_node)),
	    "mapping npages=0x%jx @ pfn=0x%jx"
	    " from vma npages=0x%jx @ pfn=0x%jx",
	    (uintmax_t)npages,
	    (uintmax_t)startpage,
	    (uintmax_t)drm_vma_node_size(&bo->base.vma_node),
	    (uintmax_t)drm_vma_node_start(&bo->base.vma_node));
	KASSERTMSG(((startpage - drm_vma_node_start(&bo->base.vma_node))
		<= (drm_vma_node_size(&bo->base.vma_node) - npages)),
	    "mapping npages=0x%jx @ pfn=0x%jx"
	    " from vma npages=0x%jx @ pfn=0x%jx",
	    (uintmax_t)npages,
	    (uintmax_t)startpage,
	    (uintmax_t)drm_vma_node_size(&bo->base.vma_node),
	    (uintmax_t)drm_vma_node_start(&bo->base.vma_node));

	ret = drm_vma_node_verify_access(&bo->base.vma_node, file->f_data);
	if (ret)
		goto fail1;

	/* Success!  */
	*uobjp = &bo->uvmobj;
	*uoffsetp = (offset -
	    ((off_t)drm_vma_node_start(&bo->base.vma_node) << PAGE_SHIFT));
	return 0;

fail1:	ttm_bo_put(bo);
fail0:	KASSERT(ret);
	return ret;
}

static int
ttm_bo_uvm_lookup(struct ttm_device *bdev, unsigned long startpage,
    unsigned long npages, struct ttm_buffer_object **bop)
{
	struct ttm_buffer_object *bo = NULL;
	struct drm_vma_offset_node *node;

	drm_vma_offset_lock_lookup(bdev->vma_manager);
	node = drm_vma_offset_lookup_locked(bdev->vma_manager, startpage,
	    npages);
	if (node != NULL) {
		bo = container_of(node, struct ttm_buffer_object,
		    base.vma_node);
		if (!kref_get_unless_zero(&bo->kref))
			bo = NULL;
	}
	drm_vma_offset_unlock_lookup(bdev->vma_manager);

	if (bo == NULL)
		return -ENOENT;

	*bop = bo;
	return 0;
}
