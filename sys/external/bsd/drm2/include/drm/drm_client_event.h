/*
 * CONFIG_DRM_CLIENT stubs
 */

#ifndef _DRM_CLIENT_EVENT_H_
#define _DRM_CLIENT_EVENT_H_

#include <sys/stdbool.h>

struct drm_device;

static inline void
drm_client_dev_unregister(struct drm_device *dev)
{
	(void)dev;
}

static inline void
drm_client_dev_hotplug(struct drm_device *dev)
{
	(void)dev;
}

static inline void
drm_client_dev_restore(struct drm_device *dev)
{
	(void)dev;
}

static inline void
drm_client_dev_suspend(struct drm_device *dev, bool holds_console_lock)
{
	(void)dev;
	(void)holds_console_lock;
}

static inline void
drm_client_dev_resume(struct drm_device *dev, bool holds_console_lock)
{
	(void)dev;
	(void)holds_console_lock;
}

#endif /* _DRM_CLIENT_EVENT_H_ */
