/*	$NetBSD$	*/

/* SPDX-License-Identifier: MIT */

/*
 * NetBSD: CONFIG_DRM_CLIENT_SETUP is not enabled; provide no-op stubs.
 */

#ifndef DRM_CLIENT_SETUP_H
#define DRM_CLIENT_SETUP_H

#include <linux/types.h>

struct drm_device;
struct drm_format_info;

static inline void
drm_client_setup(struct drm_device *dev, const struct drm_format_info *format)
{
	(void)dev;
	(void)format;
}

static inline void
drm_client_setup_with_fourcc(struct drm_device *dev, u32 fourcc)
{
	(void)dev;
	(void)fourcc;
}

static inline void
drm_client_setup_with_color_mode(struct drm_device *dev, unsigned int color_mode)
{
	(void)dev;
	(void)color_mode;
}

#endif /* DRM_CLIENT_SETUP_H */
