/*	$NetBSD: io.h,v 1.10 2022/10/25 23:33:18 riastradh Exp $	*/

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

#ifndef _ASM_IO_H_
#define _ASM_IO_H_

#include <sys/cdefs.h>
#include <sys/systm.h>

#include <linux/string.h>
#include <linux/vmalloc.h>

#define	memcpy_fromio(d,s,n)	memcpy((d), (const void *)(uintptr_t)(s), (n))
#define	memcpy_toio(d,s,n)	memcpy((void *)(uintptr_t)(d), (s), (n))

#if defined(__NetBSD__) && defined(__aarch64__)
static inline void
memset_io(volatile void *b, int c, size_t len)
{
	volatile uint8_t *ptr = b;

	while (len > 0) {
		*ptr++ = c;
		len--;
	}
}
#else
#define	memset_io(b,c,n)	memset(__UNVOLATILE(b),(c),(n))
#endif

/* XXX wrong place */
#define	__force

/*
 * MMIO accessors for CPU-mapped bus_space regions (via bus_space_vaddr).
 * Matches the legacy drmP.h definitions used by older NetBSD DRM.
 */
#ifndef readb
#define	readb(va)	(*(volatile uint8_t *)(va))
#define	readw(va)	(*(volatile uint16_t *)(va))
#define	readl(va)	(*(volatile uint32_t *)(va))
#define	readq(va)	(*(volatile uint64_t *)(va))
#define	writeb(v, va)	(*(volatile uint8_t *)(va) = (v))
#define	writew(v, va)	(*(volatile uint16_t *)(va) = (v))
#define	writel(v, va)	(*(volatile uint32_t *)(va) = (v))
#define	writeq(v, va)	(*(volatile uint64_t *)(va) = (v))
#endif

#ifndef ioread8
#define	ioread8(p)	readb(p)
#define	ioread16(p)	readw(p)
#define	ioread32(p)	readl(p)
#define	ioread64(p)	readq(p)
#define	iowrite8(v, p)	writeb(v, p)
#define	iowrite16(v, p)	writew(v, p)
#define	iowrite32(v, p)	writel(v, p)
#define	iowrite64(v, p)	writeq(v, p)
#endif

#endif  /* _ASM_IO_H_ */
