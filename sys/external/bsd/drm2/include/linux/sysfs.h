/*	$NetBSD: sysfs.h,v 1.2 2014/03/18 18:20:43 riastradh Exp $	*/

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

#ifndef _LINUX_SYSFS_H_
#define _LINUX_SYSFS_H_

#include <sys/types.h>
#include <sys/systm.h>

struct kobject;
struct device;

struct attribute {
	const char	*name;
	mode_t		mode;
};

struct bin_attribute {
	struct attribute	attr;
	size_t			size;
	void			*private;
};

#ifndef umode_t
typedef mode_t umode_t;
#endif

struct attribute_group {
	const char		*name;
	umode_t			(*is_visible)(struct kobject *,
				    struct attribute *, int);
	struct attribute	**attrs;
	struct bin_attribute	**bin_attrs;
};

struct device_attribute {
	struct attribute	attr;
	ssize_t (*show)(struct device *, struct device_attribute *, char *);
	ssize_t (*store)(struct device *, struct device_attribute *,
	    const char *, size_t);
};

#define	__ATTR(_name, _mode, _show, _store) {				\
	.attr = { .name = __STRING(_name), .mode = (_mode) },		\
	.show = (_show),						\
	.store = (_store),						\
}

#define	DEVICE_ATTR(_name, _mode, _show, _store)			\
	struct device_attribute dev_attr_##_name =			\
	    __ATTR(_name, _mode, _show, _store)

#define	DEVICE_ATTR_RO(_name)						\
	DEVICE_ATTR(_name, 0444, _name##_show, NULL)

#define	DEVICE_ATTR_RW(_name)						\
	DEVICE_ATTR(_name, 0644, _name##_show, _name##_store)

#define	DEVICE_ATTR_WO(_name)						\
	DEVICE_ATTR(_name, 0200, NULL, _name##_store)

#define	ATTRIBUTE_GROUPS(_name)						\
static const struct attribute_group *_name##_groups[] = {		\
	&_name##_group,							\
	NULL,								\
}

static inline int
sysfs_create_file(void *kobj, const struct attribute *attr)
{
	return 0;
}

static inline void
sysfs_remove_file(void *kobj, const struct attribute *attr)
{
}

static inline int
sysfs_create_group(void *kobj, const struct attribute_group *grp)
{
	return 0;
}

static inline void
sysfs_remove_group(void *kobj, const struct attribute_group *grp)
{
}

static inline int
sysfs_emit(char *buf, const char *fmt, ...)
{
	return 0;
}

static inline void
sysfs_attr_init(struct attribute *attr)
{
	(void)attr;
}

static inline int
sysfs_add_file_to_group(void *kobj, const struct attribute *attr,
    const char *name)
{
	(void)kobj;
	(void)attr;
	(void)name;
	return 0;
}

static inline void
sysfs_remove_file_from_group(void *kobj, const struct attribute *attr,
    const char *name)
{
	(void)kobj;
	(void)attr;
	(void)name;
}

static inline int
devm_device_add_group(struct device *dev, const struct attribute_group *grp)
{
	return 0;
}

static inline int
device_create_file(struct device *dev, const struct device_attribute *attr)
{
	return 0;
}

static inline void
device_remove_file(struct device *dev, const struct device_attribute *attr)
{
}

/* XXX amdgpu sysfs stubs */
static inline int
sysfs_create_bin_file(void *kobj, const struct bin_attribute *attr)
{
	(void)kobj;
	(void)attr;
	return 0;
}

static inline void
sysfs_remove_bin_file(void *kobj, const struct bin_attribute *attr)
{
	(void)kobj;
	(void)attr;
}

static inline int
sysfs_create_files(void *kobj, const struct attribute * const *attrs)
{
	(void)kobj;
	(void)attrs;
	return 0;
}

static inline void
sysfs_remove_files(void *kobj, const struct attribute * const *attrs)
{
	(void)kobj;
	(void)attrs;
}

static inline int
sysfs_emit_at(char *buf, int at, const char *fmt, ...)
{
	(void)buf;
	(void)at;
	(void)fmt;
	return 0;
}

#ifndef S_IRUGO
#define S_IRUGO		0444
#endif

#endif  /* _LINUX_SYSFS_H_ */
