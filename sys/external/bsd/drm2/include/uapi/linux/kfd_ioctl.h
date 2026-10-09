/*	$NetBSD: kfd_ioctl.h,v 1.2 2021/12/19 12:02:40 riastradh Exp $	*/

/*-
 * Copyright (c) 2021 The NetBSD Foundation, Inc.
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

#ifndef _LINUX_UAPI_KFD_IOCTL_H_
#define _LINUX_UAPI_KFD_IOCTL_H_

enum {
	KFD_MMIO_REMAP_HDP_MEM_FLUSH_CNTL	= 0,
	KFD_MMIO_REMAP_HDP_REG_FLUSH_CNTL	= 4,
};

/* Memory allocation flags used by amdgpu_amdkfd (KFD not fully ported). */
#define KFD_IOC_ALLOC_MEM_FLAGS_VRAM		(1U << 0)
#define KFD_IOC_ALLOC_MEM_FLAGS_GTT		(1U << 1)
#define KFD_IOC_ALLOC_MEM_FLAGS_USERPTR		(1U << 2)
#define KFD_IOC_ALLOC_MEM_FLAGS_DOORBELL	(1U << 3)
#define KFD_IOC_ALLOC_MEM_FLAGS_MMIO_REMAP	(1U << 4)
#define KFD_IOC_ALLOC_MEM_FLAGS_WRITABLE	(1U << 31)
#define KFD_IOC_ALLOC_MEM_FLAGS_EXECUTABLE	(1U << 30)
#define KFD_IOC_ALLOC_MEM_FLAGS_PUBLIC		(1U << 29)
#define KFD_IOC_ALLOC_MEM_FLAGS_NO_SUBSTITUTE	(1U << 28)
#define KFD_IOC_ALLOC_MEM_FLAGS_AQL_QUEUE_MEM	(1U << 27)
#define KFD_IOC_ALLOC_MEM_FLAGS_COHERENT	(1U << 26)
#define KFD_IOC_ALLOC_MEM_FLAGS_UNCACHED	(1U << 25)
#define KFD_IOC_ALLOC_MEM_FLAGS_EXT_COHERENT	(1U << 24)
#define KFD_IOC_ALLOC_MEM_FLAGS_CONTIGUOUS	(1U << 23)

#endif	/* _LINUX_UAPI_KFD_IOCTL_H_ */
