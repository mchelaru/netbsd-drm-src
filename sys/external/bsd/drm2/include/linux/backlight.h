/*	$NetBSD: backlight.h,v 1.3 2021/12/19 10:40:03 riastradh Exp $	*/

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

#ifndef _LINUX_BACKLIGHT_H_
#define _LINUX_BACKLIGHT_H_

#include <sys/types.h>
#include <sys/errno.h>
#include <sys/null.h>

struct device;
struct backlight_device;

enum backlight_type {
	BACKLIGHT_RAW = 1,
	BACKLIGHT_PLATFORM,
	BACKLIGHT_FIRMWARE,
};

/* Linux backlight power states (FB_BLANK_* compatible numbering). */
enum backlight_power {
	BACKLIGHT_POWER_ON = 0,
	BACKLIGHT_POWER_OFF = 4,
};

struct backlight_properties {
	int brightness;
	int max_brightness;
	int power;
	enum backlight_type type;
	unsigned int scale;
};

struct backlight_ops {
	unsigned int options;
	int (*update_status)(struct backlight_device *);
	int (*get_brightness)(struct backlight_device *);
};

struct backlight_device {
	struct backlight_properties props;
	const struct backlight_ops *ops;
	struct device *dev;
	void *data;
};

#define	bl_get_data(bd)	((bd)->data)

#define	backlight_disable	linux_backlight_disable
#define	backlight_enable	linux_backlight_enable

int	backlight_disable(struct backlight_device *);
int	backlight_enable(struct backlight_device *);

static inline struct backlight_device *
backlight_device_register(const char *name, struct device *dev, void *devdata,
    const struct backlight_ops *ops, const struct backlight_properties *props)
{
	return NULL;
}

static inline struct backlight_device *
devm_backlight_device_register(struct device *dev, const char *name,
    struct device *parent, void *devdata, const struct backlight_ops *ops,
    const struct backlight_properties *props)
{
	return NULL;
}

static inline void
backlight_device_unregister(struct backlight_device *bd)
{
}

static inline void
backlight_force_update(struct backlight_device *bd, int reason)
{
}

static inline int
backlight_get_brightness(struct backlight_device *bd)
{
	return bd ? bd->props.brightness : 0;
}

static inline int
backlight_update_status(struct backlight_device *bd)
{
	if (bd && bd->ops && bd->ops->update_status)
		return bd->ops->update_status(bd);
	return 0;
}

#endif  /* _LINUX_BACKLIGHT_H_ */
