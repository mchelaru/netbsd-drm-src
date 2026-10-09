/*	$NetBSD$	*/

#ifndef _LINUX_COMPILER_TYPES_H_
#define _LINUX_COMPILER_TYPES_H_

#include <linux/compiler.h>

#ifndef __iomem
#define	__iomem
#endif

#ifndef __force
#define	__force
#endif

#ifndef __user
#define	__user
#endif

#ifndef __kernel
#define	__kernel
#endif

#ifndef __packed
#define	__packed	__attribute__((__packed__))
#endif

#ifndef __malloc
#define	__malloc	__attribute__((__malloc__))
#endif

/*
 * GCC 14+ counted_by attribute; older toolchains ignore it.
 */
#ifndef __counted_by
#define	__counted_by(member)
#endif

#ifndef ___PASTE
#define	___PASTE(a, b)	a##b
#define	__PASTE(a, b)	___PASTE(a, b)
#endif

#ifndef fallthrough
#if defined(__GNUC_PREREQ__) && __GNUC_PREREQ__(7, 0)
#define	fallthrough	__attribute__((__fallthrough__))
#else
#define	fallthrough	do { } while (0) /* fallthrough */
#endif
#endif

#endif /* _LINUX_COMPILER_TYPES_H_ */
