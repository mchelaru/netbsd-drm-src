/*	$NetBSD$	*/

/*-
 * Copyright (c) 2026 The NetBSD Foundation, Inc.
 * All rights reserved.
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

#ifndef _LINUX_MEMREMAP_H_
#define _LINUX_MEMREMAP_H_

#include <sys/types.h>

#include <linux/types.h>

struct device;
struct page;
struct vm_fault;

/* Minimal stand-in for Linux's address range helper. */
struct range {
	u64	start;
	u64	end;
};

enum memory_type {
	MEMORY_DEVICE_PRIVATE = 1,
	MEMORY_DEVICE_COHERENT,
	MEMORY_DEVICE_FS_DAX,
	MEMORY_DEVICE_GENERIC,
	MEMORY_DEVICE_PCI_P2PDMA,
};

struct dev_pagemap;

struct dev_pagemap_ops {
	void	(*page_free)(struct page *);
	unsigned int (*migrate_to_ram)(struct vm_fault *);
	int	(*memory_failure)(struct dev_pagemap *, unsigned long,
		    unsigned long, int);
};

/*
 * Only the fields referenced by amdgpu/amdkfd on NetBSD.  Full
 * ZONE_DEVICE memremap is not implemented here.
 */
struct dev_pagemap {
	enum memory_type		type;
	unsigned int			flags;
	const struct dev_pagemap_ops	*ops;
	void				*owner;
	int				nr_range;
	struct range			range;
};

static inline void *
memremap_pages(struct dev_pagemap *pgmap, int nid)
{
	(void)pgmap;
	(void)nid;
	return NULL;
}

static inline void
memunmap_pages(struct dev_pagemap *pgmap)
{
	(void)pgmap;
}

static inline void *
devm_memremap_pages(struct device *dev, struct dev_pagemap *pgmap)
{
	(void)dev;
	(void)pgmap;
	return NULL;
}

static inline void
devm_memunmap_pages(struct device *dev, struct dev_pagemap *pgmap)
{
	(void)dev;
	(void)pgmap;
}

static inline void *
memremap(resource_size_t offset, size_t size, unsigned long flags)
{
	(void)offset;
	(void)size;
	(void)flags;
	return NULL;
}

static inline void
memunmap(void *addr)
{
	(void)addr;
}

#define	MEMREMAP_WB	0x01
#define	MEMREMAP_WT	0x02
#define	MEMREMAP_WC	0x04

#endif /* _LINUX_MEMREMAP_H_ */
