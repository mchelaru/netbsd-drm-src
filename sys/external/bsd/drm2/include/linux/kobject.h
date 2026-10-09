/*	$NetBSD: kobject.h,v 1.2 2014/03/18 18:20:43 riastradh Exp $	*/

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

#ifndef _LINUX_KOBJECT_H_
#define _LINUX_KOBJECT_H_

#include <sys/types.h>
#include <sys/systm.h>

#include <linux/list.h>

/*
 * Minimal kobject stub so structures can embed struct kobject.
 * sysfs registration is a no-op on NetBSD.
 */
struct kobject {
	const char		*name;
	struct list_head	entry;
	struct kobject		*parent;
	unsigned int		state_initialized:1;
};

static inline void
kobject_init(struct kobject *kobj, void *ktype)
{
	(void)ktype;
	memset(kobj, 0, sizeof(*kobj));
	kobj->state_initialized = 1;
}

static inline int
kobject_add(struct kobject *kobj, struct kobject *parent, const char *fmt, ...)
{
	(void)kobj;
	(void)parent;
	(void)fmt;
	return 0;
}

static inline void
kobject_put(struct kobject *kobj)
{
	(void)kobj;
}

static inline void
kobject_del(struct kobject *kobj)
{
	(void)kobj;
}

#endif  /* _LINUX_KOBJECT_H_ */
