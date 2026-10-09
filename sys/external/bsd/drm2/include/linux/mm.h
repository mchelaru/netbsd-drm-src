/*	$NetBSD: mm.h,v 1.24 2021/12/19 12:21:30 riastradh Exp $	*/

/*-
 * Copyright (c) 2013 The NetBSD Foundation, Inc.
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

#ifndef _LINUX_MM_H_
#define _LINUX_MM_H_

#include <uvm/uvm_extern.h>
#include <uvm/uvm_object.h>
#include <uvm/uvm.h>
#include <uvm/uvm_page.h>

#include <asm/page.h>

#include <linux/pfn.h>
#include <linux/rwsem.h>
#include <linux/shrinker.h>
#include <linux/slab.h>
#include <linux/sizes.h>
#include <linux/mm_types.h>
#include <linux/numa.h>
#include <linux/topology.h>
#include <linux/string.h>

struct file;
struct address_space;

/*
 * Minimal stand-ins for Linux fault handling.  NetBSD TTM uses UVM
 * fault entry points (see drm2/ttm/ttm_bo_vm.c); these exist only so
 * upstream TTM headers and unused Linux vm helpers can compile.
 */
#define	VM_FAULT_OOM		0x0001
#define	VM_FAULT_SIGBUS		0x0002
#define	VM_FAULT_NOPAGE		0x0004
#define	VM_FAULT_RETRY		0x0008
#define	VM_FAULT_ERROR		(VM_FAULT_OOM | VM_FAULT_SIGBUS)

#define	FAULT_FLAG_ALLOW_RETRY		0x01
#define	FAULT_FLAG_RETRY_NOWAIT		0x02

struct mm_struct {
	struct rw_semaphore	mmap_sem;
};

struct vm_area_struct {
	struct mm_struct	*vm_mm;
	struct file		*vm_file;
	void			*vm_private_data;
	pgprot_t		vm_page_prot;
	unsigned long		vm_start;
	unsigned long		vm_end;
	unsigned long		vm_flags;
};

struct vm_fault {
	struct vm_area_struct	*vma;
	unsigned int		flags;
	unsigned long		address;
};

/* XXX Ugh bletch!  Whattakludge!  Linux's sense is reversed...  */
#undef	PAGE_MASK
#define	PAGE_MASK	(~(PAGE_SIZE-1))

#define	PAGE_ALIGN(x)		(((x) + (PAGE_SIZE-1)) & ~(PAGE_SIZE-1))
#define	offset_in_page(x)	((uintptr_t)(x) & (PAGE_SIZE-1))

#define	untagged_addr(x)	(x)

struct sysinfo {
	unsigned long totalram;
	unsigned long totalhigh;
	unsigned long freeram;
	unsigned long freehigh;
	uint32_t mem_unit;
};

static inline void
si_meminfo(struct sysinfo *si)
{

	si->totalram = uvmexp.npages;
	si->totalhigh = kernel_map->size >> PAGE_SHIFT;
	si->freeram = uvmexp.free;
	si->freehigh = 0;
	si->mem_unit = PAGE_SIZE;
	/* XXX Fill in more as needed.  */
}

static inline size_t
si_mem_available(void)
{

	/* XXX ? */
	return uvmexp.free;
}

static inline unsigned long
vm_mmap(struct file *file __unused, unsigned long base __unused,
    unsigned long size __unused, unsigned long prot __unused,
    unsigned long flags __unused, unsigned long token __unused)
{

	return -ENODEV;
}

static inline unsigned long
totalram_pages(void)
{

	return uvmexp.npages;
}

static inline unsigned long
get_num_physpages(void)
{

	return uvmexp.npages;
}

static inline void *
kvmalloc(size_t size, gfp_t gfp)
{

	return kmalloc(size, gfp);
}

static inline void *
kvzalloc(size_t size, gfp_t gfp)
{

	return kmalloc(size, gfp | __GFP_ZERO);
}

static inline void *
kvcalloc(size_t nelem, size_t elemsize, gfp_t gfp)
{

	KASSERT(elemsize > 0);
	if (SIZE_MAX/elemsize < nelem)
		return NULL;
	return kvzalloc(nelem * elemsize, gfp);
}

static inline void *
kvmalloc_array(size_t nelem, size_t elemsize, gfp_t gfp)
{

	KASSERT(elemsize != 0);
	if (nelem > SIZE_MAX/elemsize)
		return NULL;
	return kmalloc(nelem * elemsize, gfp);
}

/*
 * Linux kvrealloc: grow/shrink kvalloc memory.  NetBSD kvmalloc is
 * kmalloc, so krealloc is sufficient.
 */
static inline void *
kvrealloc(void *ptr, size_t size, gfp_t gfp)
{

	return krealloc(ptr, size, gfp);
}

/*
 * XXX kvfree must additionally work on kmalloc (linux/slab.h) and
 * vmalloc (linux/vmalloc.h).  If you change either of those, be sure
 * to change this too.
 */

static inline void
kvfree(void *ptr)
{
	kfree(ptr);
}

static inline void
set_page_dirty(struct page *page)
{
	struct vm_page *pg = &page->p_vmp;

	/* XXX */
	if (pg->uobject != NULL) {
		rw_enter(pg->uobject->vmobjlock, RW_WRITER);
		uvm_pagemarkdirty(pg, UVM_PAGE_STATUS_DIRTY);
		rw_exit(pg->uobject->vmobjlock);
	} else {
		uvm_pagemarkdirty(pg, UVM_PAGE_STATUS_DIRTY);
	}
}

static inline void
put_page(struct page *page)
{
	(void)page;
	/* XXX: NetBSD TTM holds wired uvm pages; no page refcount yet. */
}

static inline void
get_page(struct page *page)
{
	(void)page;
}

static inline void
mark_page_accessed(struct page *page)
{
	(void)page;
}

static inline bool
want_init_on_free(void)
{
	return false;
}

/*
 * alloc_page / alloc_pages: back Linux page allocation with UVM.
 *
 * struct page embeds struct vm_page as p_vmp, so container_of works with
 * PHYS_TO_VM_PAGE.  Extra Linux fields (lru, private) overlay the next
 * vm_page in the array and must not be written for UVM-backed pages;
 * NetBSD TTM pool code avoids those fields (see ttm_pool.c).
 *
 * Only order-0 allocations are supported for now.  Higher orders return
 * NULL; callers that need them (ttm_pool) are forced to order 0 on NetBSD.
 */
static inline struct page *
alloc_pages_node(int nid, gfp_t gfp, unsigned int order)
{
	struct vm_page *pg;
	int pga = 0;
	bool canwait = (gfp & __GFP_WAIT) != 0 &&
	    (gfp & __GFP_NORETRY) == 0;

	(void)nid;

	if (order != 0)
		return NULL;

	if (gfp & __GFP_ZERO)
		pga |= UVM_PGA_ZERO;

	for (;;) {
		if (gfp & __GFP_DMA32) {
			struct pglist plist;
			int error;

			TAILQ_INIT(&plist);
			/* Prefer pages below 4GiB when GFP_DMA32 is set. */
			error = uvm_pglistalloc(PAGE_SIZE, 0, 0xffffffffUL,
			    PAGE_SIZE, 0, &plist, 1, canwait ? 1 : 0);
			if (error) {
				pg = NULL;
			} else {
				pg = TAILQ_FIRST(&plist);
				KASSERT(pg != NULL);
				if (gfp & __GFP_ZERO)
					uvm_pagezero(pg);
			}
		} else {
			pg = uvm_pagealloc(NULL, 0, NULL, pga);
		}

		if (pg != NULL)
			break;
		if (!canwait)
			return NULL;
		uvm_wait("lpgalloc");
	}

	return container_of(pg, struct page, p_vmp);
}

static inline struct page *
alloc_page(gfp_t gfp)
{
	return alloc_pages_node(NUMA_NO_NODE, gfp, 0);
}

#define	alloc_pages(gfp, order)	alloc_pages_node(NUMA_NO_NODE, (gfp), (order))

static inline void
__free_pages(struct page *page, unsigned int order)
{
	struct vm_page *pg;
	unsigned int i, n;

	if (page == NULL)
		return;

	KASSERT(order == 0);
	n = 1U << order;
	pg = &page->p_vmp;
	for (i = 0; i < n; i++)
		uvm_pagefree(pg + i);
}

static inline void
__free_page(struct page *page)
{
	__free_pages(page, 0);
}

static inline int
PageHighMem(struct page *page)
{
	(void)page;
	return 0;
}

static inline void *
page_address(struct page *page)
{
	/* Direct-map stand-in: phys -> kernel VA where available. */
	return (void *)(uintptr_t)page_to_phys(page);
}

static inline void
clear_page(void *addr)
{
	memset(addr, 0, PAGE_SIZE);
}

static inline void
clear_highpage(struct page *page)
{
	clear_page(page_address(page));
}

/* XXX amdgpu: Linux NUMA/RAM helpers unused on NetBSD. */
static inline int
page_is_ram(unsigned long pfn)
{
	(void)pfn;
	return 0;
}

static inline int
pfn_to_nid(unsigned long pfn)
{
	(void)pfn;
	return 0;
}

#endif  /* _LINUX_MM_H_ */
