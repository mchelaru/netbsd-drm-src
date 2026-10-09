/*	$NetBSD$	*/

/* SPDX-License-Identifier: GPL-2.0 or MIT */

/*
 * Copyright (c) 2024 Intel
 * Copyright (c) 2024 Red Hat
 *
 * NetBSD: CONFIG_DRM_PANIC is not enabled; provide the type and no-op locks
 * so amdgpu can compile get_scanout_buffer callbacks.
 */

#ifndef __DRM_PANIC_H__
#define __DRM_PANIC_H__

#include <linux/types.h>
#include <linux/iosys-map.h>

#include <drm/drm_device.h>
#include <drm/drm_fourcc.h>

struct drm_scanout_buffer {
	const struct drm_format_info *format;
	struct iosys_map map[DRM_FORMAT_MAX_PLANES];
	unsigned int width;
	unsigned int height;
	unsigned int pitch[DRM_FORMAT_MAX_PLANES];
	void (*set_pixel)(struct drm_scanout_buffer *sb, unsigned int x,
			  unsigned int y, u32 color);
};

static inline bool
drm_panic_trylock(struct drm_device *dev, unsigned long flags)
{
	(void)dev;
	(void)flags;
	return true;
}

static inline void
drm_panic_lock(struct drm_device *dev, unsigned long flags)
{
	(void)dev;
	(void)flags;
}

static inline void
drm_panic_unlock(struct drm_device *dev, unsigned long flags)
{
	(void)dev;
	(void)flags;
}

#endif /* __DRM_PANIC_H__ */
