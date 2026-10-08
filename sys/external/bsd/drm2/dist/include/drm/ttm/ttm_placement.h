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

#ifndef _TTM_PLACEMENT_H_
#define _TTM_PLACEMENT_H_

#include <linux/types.h>

/*
 * Memory regions for data placement.
 *
 * Buffers placed in TTM_PL_SYSTEM are considered under TTMs control and can
 * be swapped out whenever TTMs thinks it is a good idea.
 * In cases where drivers would like to use TTM_PL_SYSTEM as a valid
 * placement they need to be able to handle the issues that arise due to the
 * above manually.
 *
 * For BO's which reside in system memory but for which the accelerator
 * requires direct access (i.e. their usage needs to be synchronized
 * between the CPU and accelerator via fences) a new, driver private
 * placement that can handle such scenarios is a good idea.
 */

#define TTM_PL_SYSTEM           0
#define TTM_PL_TT               1
#define TTM_PL_VRAM             2
#define TTM_PL_PRIV             3

/*
 * TTM_PL_FLAG_TOPDOWN requests to be placed from the
 * top of the memory area, instead of the bottom.
 */

#define TTM_PL_FLAG_CONTIGUOUS  (1 << 0)
#define TTM_PL_FLAG_TOPDOWN     (1 << 1)

/* For multihop handling */
#define TTM_PL_FLAG_TEMPORARY   (1 << 2)

/* Placement is never used during eviction */
#define TTM_PL_FLAG_DESIRED	(1 << 3)

/* Placement is only used during eviction */
#define TTM_PL_FLAG_FALLBACK	(1 << 4)

#ifdef __NetBSD__
/*
 * Compatibility with older TTM placement flags still used by nouveau/radeon
 * in this tree.  New amdgpu code uses mem_type + the flags above.
 */
#define TTM_PL_FLAG_SYSTEM      (1 << TTM_PL_SYSTEM)
#define TTM_PL_FLAG_TT          (1 << TTM_PL_TT)
#define TTM_PL_FLAG_VRAM        (1 << TTM_PL_VRAM)
#define TTM_PL_FLAG_PRIV        (1 << TTM_PL_PRIV)
#define TTM_PL_MASK_MEM         0x0000FFFF
#define TTM_PL_FLAG_CACHED      (1 << 16)
#define TTM_PL_FLAG_UNCACHED    (1 << 17)
#define TTM_PL_FLAG_WC          (1 << 18)
#define TTM_PL_FLAG_NO_EVICT    (1 << 21)
#define TTM_PL_MASK_CACHING     (TTM_PL_FLAG_CACHED | \
				 TTM_PL_FLAG_UNCACHED | \
				 TTM_PL_FLAG_WC)
#define TTM_PL_MASK_MEMTYPE     (TTM_PL_MASK_MEM | TTM_PL_MASK_CACHING)
#endif

/**
 * struct ttm_place
 *
 * @fpfn:	first valid page frame number to put the object
 * @lpfn:	last valid page frame number to put the object
 * @mem_type:	One of TTM_PL_* where the resource should be allocated from.
 * @flags:	memory domain and caching flags for the object
 *
 * Structure indicating a possible place to put an object.
 */
struct ttm_place {
	unsigned	fpfn;
	unsigned	lpfn;
	uint32_t	mem_type;
	uint32_t	flags;
};

/**
 * struct ttm_placement
 *
 * @num_placement:	number of preferred placements
 * @placement:		preferred placements
 *
 * Structure indicating the placement you request for an object.
 */
struct ttm_placement {
	unsigned		num_placement;
	const struct ttm_place	*placement;
#ifdef __NetBSD__
	/* Legacy fields still referenced by nouveau/radeon. */
	unsigned		num_busy_placement;
	const struct ttm_place	*busy_placement;
#endif
};

#endif
