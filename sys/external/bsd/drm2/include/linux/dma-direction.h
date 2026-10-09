/*	$NetBSD$	*/

/* SPDX-License-Identifier: GPL-2.0 */
#ifndef _LINUX_DMA_DIRECTION_H
#define _LINUX_DMA_DIRECTION_H

/*
 * NetBSD keeps dma_data_direction in <linux/dma-mapping.h>.
 * Pull that definition so a second enum declaration is not introduced.
 */
#include <linux/dma-mapping.h>

static inline int
valid_dma_direction(enum dma_data_direction dir)
{
	return dir == DMA_BIDIRECTIONAL || dir == DMA_TO_DEVICE ||
	    dir == DMA_FROM_DEVICE || dir == DMA_NONE;
}

#endif /* _LINUX_DMA_DIRECTION_H */
