/* SPDX-License-Identifier: MIT */

/*
 * NetBSD: provide DRM_FBDEV_TTM_DRIVER_OPS.  CONFIG_DRM_FBDEV_EMULATION
 * may be set for build flags; NetBSD's drm_driver has no fbdev_probe
 * field yet, so the ops expand to nothing here.
 */

#ifndef DRM_FBDEV_TTM_H
#define DRM_FBDEV_TTM_H

#include <linux/stddef.h>

struct drm_fb_helper;
struct drm_fb_helper_surface_size;

#if defined(__NetBSD__)
#define DRM_FBDEV_TTM_DRIVER_OPS	/* nothing yet */
#elif defined(CONFIG_DRM_FBDEV_EMULATION) && CONFIG_DRM_FBDEV_EMULATION
int drm_fbdev_ttm_driver_fbdev_probe(struct drm_fb_helper *fb_helper,
    struct drm_fb_helper_surface_size *sizes);

#define DRM_FBDEV_TTM_DRIVER_OPS \
	.fbdev_probe = drm_fbdev_ttm_driver_fbdev_probe
#else
#define DRM_FBDEV_TTM_DRIVER_OPS \
	.fbdev_probe = NULL
#endif

#endif /* DRM_FBDEV_TTM_H */
