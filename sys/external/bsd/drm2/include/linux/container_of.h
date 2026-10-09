/*	$NetBSD$	*/

/* SPDX-License-Identifier: GPL-2.0 */

/*
 * NetBSD already provides container_of() in <sys/container_of.h>.
 * Newer Linux DRM headers include <linux/container_of.h> directly.
 */

#ifndef _LINUX_CONTAINER_OF_H
#define _LINUX_CONTAINER_OF_H

#include <sys/container_of.h>

#include <linux/stddef.h>

#define typeof_member(T, m)	typeof(((T *)0)->m)

#ifndef container_of_const
#define container_of_const(ptr, type, member)				\
	_Generic(ptr,							\
		const typeof(*(ptr)) *:					\
		    ((const type *)container_of(ptr, type, member)),	\
		default: ((type *)container_of(ptr, type, member))	\
	)
#endif

#endif /* _LINUX_CONTAINER_OF_H */
