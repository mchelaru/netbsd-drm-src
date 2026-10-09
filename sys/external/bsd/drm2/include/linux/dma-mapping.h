/*	$NetBSD: dma-mapping.h,v 1.10 2021/12/19 12:21:30 riastradh Exp $	*/

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

#ifndef _LINUX_DMA_MAPPING_H_
#define _LINUX_DMA_MAPPING_H_

#include <sys/param.h>
#include <sys/bus.h>

#include <machine/limits.h>

#include <linux/types.h>
#include <linux/gfp.h>
#include <linux/mm_types.h>
#include <asm/page.h>

enum dma_data_direction {
	DMA_NONE		= 0,
	DMA_TO_DEVICE		= 1,
	DMA_FROM_DEVICE		= 2,
	DMA_BIDIRECTIONAL	= 3,

	PCI_DMA_NONE		= DMA_NONE,
	PCI_TO_DEVICE		= DMA_TO_DEVICE,
	PCI_FROM_DEVICE		= DMA_FROM_DEVICE,
	PCI_DMA_BIDIRECTIONAL	= DMA_BIDIRECTIONAL,
};

enum {
	DMA_ATTR_NO_WARN	= __BIT(0),
	DMA_ATTR_SKIP_CPU_SYNC	= __BIT(1),
	DMA_ATTR_FORCE_CONTIGUOUS = __BIT(2),
};

static inline uintmax_t
DMA_BIT_MASK(unsigned nbits)
{

	if (nbits == CHAR_BIT*sizeof(uintmax_t))
		return ~(uintmax_t)0;
	return ~(~(uintmax_t)0 << nbits);
}

static inline bool
dma_addressing_limited(device_t dev)
{

	return false;
}

static inline void *
dma_alloc_attrs(device_t dev, size_t size, dma_addr_t *dma_handle, gfp_t gfp,
    unsigned long attrs)
{
	(void)dev;
	(void)size;
	(void)dma_handle;
	(void)gfp;
	(void)attrs;
	return NULL;
}

static inline void
dma_free_attrs(device_t dev, size_t size, void *cpu_addr, dma_addr_t dma_handle,
    unsigned long attrs)
{
	(void)dev;
	(void)size;
	(void)cpu_addr;
	(void)dma_handle;
	(void)attrs;
}

static inline dma_addr_t
dma_map_page(device_t dev, struct page *page, size_t offset, size_t size,
    int dir)
{
	(void)dev;
	(void)size;
	(void)dir;
	/* Identity map: amdgpu GART wants the CPU physical address. */
	return (dma_addr_t)page_to_phys(page) + offset;
}

static inline void
dma_unmap_page(device_t dev, dma_addr_t addr, size_t size, int dir)
{
	(void)dev;
	(void)addr;
	(void)size;
	(void)dir;
}

static inline int
dma_mapping_error(device_t dev, dma_addr_t addr)
{
	(void)dev;
	(void)addr;
	return 0;
}

static inline int
dma_set_mask(device_t dev, uint64_t mask)
{
	(void)dev;
	(void)mask;
	return 0;
}

static inline int
dma_set_coherent_mask(device_t dev, uint64_t mask)
{
	(void)dev;
	(void)mask;
	return 0;
}

static inline int
dma_set_mask_and_coherent(device_t dev, uint64_t mask)
{
	int rc = dma_set_mask(dev, mask);

	if (rc == 0)
		dma_set_coherent_mask(dev, mask);
	return rc;
}

static inline void *
dma_alloc_coherent(device_t dev, size_t size, dma_addr_t *dma_handle, gfp_t gfp)
{
	(void)dev;
	(void)size;
	(void)dma_handle;
	(void)gfp;
	return NULL;
}

static inline void
dma_free_coherent(device_t dev, size_t size, void *cpu_addr, dma_addr_t dma_handle)
{
	(void)dev;
	(void)size;
	(void)cpu_addr;
	(void)dma_handle;
}

#endif  /* _LINUX_DMA_MAPPING_H_ */
