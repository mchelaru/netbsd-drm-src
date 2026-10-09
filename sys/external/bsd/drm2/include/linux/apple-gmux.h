/*	$NetBSD$	*/

/* SPDX-License-Identifier: GPL-2.0-only */

/*
 * NetBSD: CONFIG_APPLE_GMUX is not enabled; provide the CONFIG-off stubs
 * so amdgpu can call apple_gmux_detect() / apple_gmux_present().
 */

#ifndef LINUX_APPLE_GMUX_H
#define LINUX_APPLE_GMUX_H

#include <sys/stdbool.h>

#include <linux/types.h>

struct pnp_dev;

enum apple_gmux_type {
	APPLE_GMUX_TYPE_PIO,
	APPLE_GMUX_TYPE_INDEXED,
	APPLE_GMUX_TYPE_MMIO,
};

static inline bool
apple_gmux_present(void)
{
	return false;
}

static inline bool
apple_gmux_detect(struct pnp_dev *pnp_dev, enum apple_gmux_type *type_ret)
{
	(void)pnp_dev;
	(void)type_ret;
	return false;
}

#endif /* LINUX_APPLE_GMUX_H */
