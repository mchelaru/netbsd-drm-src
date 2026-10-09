/*	$NetBSD$	*/

/* SPDX-License-Identifier: GPL-2.0 */

/*
 * NetBSD: CONFIG_DYNAMIC_DEBUG is not enabled.  Provide enough of the
 * Linux dynamic_debug API for amdgpu (DECLARE_DYNDBG_CLASSMAP and the
 * CONFIG-off pr_debug wrappers).
 */

#ifndef _DYNAMIC_DEBUG_H
#define _DYNAMIC_DEBUG_H

#include <sys/types.h>

#include <linux/errno.h>
#include <linux/printk.h>

enum class_map_type {
	DD_CLASS_TYPE_DISJOINT_BITS,
	DD_CLASS_TYPE_LEVEL_NUM,
	DD_CLASS_TYPE_DISJOINT_NAMES,
	DD_CLASS_TYPE_LEVEL_NAMES,
};

/*
 * Discard the class map; NetBSD has no dyndbg ELF sections or module
 * param plumbing for it yet.
 */
#define DECLARE_DYNDBG_CLASSMAP(_var, _maptype, _base, ...)		\
	static const char * const _var##_classnames[] __unused = {	\
		__VA_ARGS__						\
	};								\
	static const enum class_map_type _var __unused = (_maptype)

#define DEFINE_DYNAMIC_DEBUG_METADATA(name, fmt)	/* nothing */
#define DYNAMIC_DEBUG_BRANCH(descriptor)		false

#ifndef no_printk
#define no_printk(fmt, ...)						\
	do {								\
		if (0)							\
			printk(fmt, ##__VA_ARGS__);			\
	} while (0)
#endif

#ifndef dev_no_printk
#define dev_no_printk(level, dev, fmt, ...)				\
	do {								\
		if (0)							\
			printk(fmt, ##__VA_ARGS__);			\
	} while (0)
#endif

#define dynamic_pr_debug(fmt, ...)					\
	no_printk(KERN_DEBUG pr_fmt(fmt), ##__VA_ARGS__)
#define dynamic_dev_dbg(dev, fmt, ...)					\
	dev_no_printk(KERN_DEBUG, dev, fmt, ##__VA_ARGS__)
#define dynamic_hex_dump(prefix_str, prefix_type, rowsize,		\
			 groupsize, buf, len, ascii)			\
	do {								\
		(void)(prefix_str);					\
		(void)(prefix_type);					\
		(void)(rowsize);					\
		(void)(groupsize);					\
		(void)(buf);						\
		(void)(len);						\
		(void)(ascii);						\
	} while (0)

static inline int
ddebug_dyndbg_module_param_cb(char *param, char *val, const char *modname)
{
	(void)val;
	(void)modname;
	if (param && param[0] == 'd' /* "dyndbg" */)
		return 0;
	return -EINVAL;
}

struct kernel_param;

static inline int
param_set_dyndbg_classes(const char *instr, const struct kernel_param *kp)
{
	(void)instr;
	(void)kp;
	return 0;
}

static inline int
param_get_dyndbg_classes(char *buffer, const struct kernel_param *kp)
{
	(void)buffer;
	(void)kp;
	return 0;
}

#endif /* _DYNAMIC_DEBUG_H */
