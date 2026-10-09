/*	$NetBSD: amdgpufb.c,v 1.5 2022/07/18 23:34:02 riastradh Exp $	*/

/*-
 * Copyright (c) 2018 The NetBSD Foundation, Inc.
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

#include <sys/cdefs.h>
__KERNEL_RCSID(0, "$NetBSD: amdgpufb.c,v 1.5 2022/07/18 23:34:02 riastradh Exp $");

#include <sys/param.h>
#include <sys/types.h>
#include <sys/bus.h>
#include <sys/device.h>
#include <sys/errno.h>
#include <sys/systm.h>

#include <machine/bootinfo.h>
#include <machine/bus_funcs.h>

#include <drm/drmfb.h>
#include <drm/drmfb_pci.h>
#include <drm/ttm/ttm_caching.h>
#include <drm/ttm/ttm_placement.h>
#include <drm/ttm/ttm_tt.h>
#include <drm/ttm/ttm_bo.h>

#include "amdgpu.h"
#include "amdgpu_task.h"
#include "amdgpufb.h"

#include "dc.h"
#include "dc_bios_types.h"

struct amdgpufb_softc;

static int	amdgpufb_match(device_t, cfdata_t, void *);
static void	amdgpufb_attach(device_t, device_t, void *);
static int	amdgpufb_detach(device_t, int);

static void	amdgpufb_attach_task(struct amdgpu_task *);

static bool	amdgpufb_shutdown(device_t, int);
static paddr_t	amdgpufb_drmfb_mmapfb(struct drmfb_softc *, off_t, int);
static bool	amdgpufb_fbdev_restore_ok(struct drmfb_softc *, bool);
static void	amdgpufb_fbdev_restored(struct drmfb_softc *);
static bool	amdgpufb_is_accelerated(struct amdgpufb_softc *);

struct amdgpufb_softc {
	struct drmfb_softc		sc_drmfb; /* XXX Must be first.  */
	device_t			sc_dev;
	struct amdgpufb_attach_args	sc_afa;
	struct amdgpu_task		sc_attach_task;
	bool				sc_attached:1;
	/* Bootloader GOP FB panel still scans this while KMS is deferred. */
	bool				sc_gop_console:1;
	bus_space_handle_t		sc_gop_bsh;
	uint64_t			sc_gop_phys;
	uint32_t			sc_gop_mapsz;
	uint16_t			sc_gop_stride;
	/* KMS console VA (VRAM aper kmap; rasops target after fbdev restore). */
	void				*sc_gtt_va;
	uint32_t			sc_gtt_linebytes;
};

static const struct drmfb_params amdgpufb_drmfb_params = {
	.dp_mmapfb = amdgpufb_drmfb_mmapfb,
	.dp_mmap = drmfb_pci_mmap,
	.dp_ioctl = drmfb_pci_ioctl,
	.dp_is_vga_console = drmfb_pci_is_vga_console,
	.dp_fbdev_restore_ok = amdgpufb_fbdev_restore_ok,
	.dp_fbdev_restored = amdgpufb_fbdev_restored,
};

CFATTACH_DECL_NEW(amdgpufb, sizeof(struct amdgpufb_softc),
    amdgpufb_match, amdgpufb_attach, amdgpufb_detach, NULL);

static int
amdgpufb_match(device_t parent, cfdata_t match, void *aux)
{

	return 1;
}

static void
amdgpufb_attach(device_t parent, device_t self, void *aux)
{
	struct amdgpufb_softc *const sc = device_private(self);
	const struct amdgpufb_attach_args *const afa = aux;

	sc->sc_dev = self;
	sc->sc_afa = *afa;
	sc->sc_attached = false;

	aprint_naive("\n");
	aprint_normal("\n");

	amdgpu_task_init(&sc->sc_attach_task, &amdgpufb_attach_task);
	amdgpu_task_schedule(parent, &sc->sc_attach_task);
	config_pending_incr(self);
}

static int
amdgpufb_detach(device_t self, int flags)
{
	struct amdgpufb_softc *const sc = device_private(self);
	int error;

	if (sc->sc_attached) {
		pmf_device_deregister(self);
		error = drmfb_detach(&sc->sc_drmfb, flags);
		if (error) {
			/* XXX Ugh.  */
			(void)pmf_device_register1(self, NULL, NULL,
			    &amdgpufb_shutdown);
			return error;
		}
		sc->sc_attached = false;
	}
	if (sc->sc_gop_console) {
		bus_space_unmap(x86_bus_space_mem, sc->sc_gop_bsh,
		    sc->sc_gop_mapsz);
		sc->sc_gop_console = false;
	}

	return 0;
}

static void
amdgpufb_attach_task(struct amdgpu_task *task)
{
	struct amdgpufb_softc *const sc = container_of(task,
	    struct amdgpufb_softc, sc_attach_task);
	struct amdgpufb_attach_args *const afa = &sc->sc_afa;
	struct drmfb_attach_args da;
	const struct btinfo_framebuffer *bi;
	void *gop_va = NULL;
	int error;

	/*
	 * Keep the KMS console VA for later rebind after fbdev restore
	 * Prefer bootloader GOP for drawing while VBIOS still owns scanout
	 */
	sc->sc_gtt_va = __UNVOLATILE(afa->afa_fb_ptr);
	sc->sc_gtt_linebytes = afa->afa_fb_linebytes;

	bi = lookup_bootinfo(BTINFO_FRAMEBUFFER);
	if (bi != NULL && bi->physaddr != 0 &&
	    bi->width == afa->afa_fb_sizes.surface_width &&
	    bi->height == afa->afa_fb_sizes.surface_height &&
	    bi->stride != 0) {
		sc->sc_gop_mapsz = (uint32_t)bi->height * bi->stride;
		error = bus_space_map(x86_bus_space_mem,
		    (bus_addr_t)bi->physaddr, sc->sc_gop_mapsz,
		    BUS_SPACE_MAP_LINEAR | BUS_SPACE_MAP_PREFETCHABLE,
		    &sc->sc_gop_bsh);
		if (error == 0) {
			gop_va = bus_space_vaddr(x86_bus_space_mem,
			    sc->sc_gop_bsh);
			if (gop_va != NULL) {
				sc->sc_gop_console = true;
				sc->sc_gop_phys = bi->physaddr;
				sc->sc_gop_stride = bi->stride;
				afa->afa_fb_ptr = gop_va;
				afa->afa_fb_linebytes = bi->stride;
				aprint_normal_dev(sc->sc_dev,
				    "console on GOP FB %ux%u@%u stride %u "
				    "phys 0x%jx (VBIOS scanout)\n",
				    bi->width, bi->height, bi->depth,
				    bi->stride, (uintmax_t)bi->physaddr);
			} else {
				bus_space_unmap(x86_bus_space_mem,
				    sc->sc_gop_bsh, sc->sc_gop_mapsz);
			}
		} else {
			aprint_error_dev(sc->sc_dev,
			    "GOP FB map failed (%d); using KMS FB\n",
			    error);
		}
	}

	da = (struct drmfb_attach_args) {
		.da_dev = sc->sc_dev,
		.da_fb_helper = afa->afa_fb_helper,
		.da_fb_sizes = &afa->afa_fb_sizes,
		.da_fb_vaddr = __UNVOLATILE(afa->afa_fb_ptr),
		.da_fb_linebytes = afa->afa_fb_linebytes,
		.da_params = &amdgpufb_drmfb_params,
	};

	error = drmfb_attach(&sc->sc_drmfb, &da);
	if (error) {
		aprint_error_dev(sc->sc_dev, "failed to attach drmfb: %d\n",
		    error);
		if (sc->sc_gop_console) {
			bus_space_unmap(x86_bus_space_mem, sc->sc_gop_bsh,
			    sc->sc_gop_mapsz);
			sc->sc_gop_console = false;
		}
		goto out;
	}

	if (!pmf_device_register1(sc->sc_dev, NULL, NULL, &amdgpufb_shutdown))
		aprint_error_dev(sc->sc_dev,
		    "failed to register shutdown handler\n");

	sc->sc_attached = true;
out:
	config_pending_decr(sc->sc_dev);
}

static bool
amdgpufb_shutdown(device_t self, int flags)
{
	struct amdgpufb_softc *const sc = device_private(self);

	return drmfb_shutdown(&sc->sc_drmfb, flags);
}

static bool
amdgpufb_is_accelerated(struct amdgpufb_softc *sc)
{
	struct drm_fb_helper *helper = sc->sc_afa.afa_fb_helper;
	struct amdgpu_device *adev;
	struct dc_bios *dcb;

	if (helper == NULL || helper->dev == NULL ||
	    helper->dev->dev_private == NULL)
		return false;

	adev = helper->dev->dev_private;
	if (adev->dm.dc == NULL || adev->dm.dc->ctx == NULL)
		return false;

	dcb = adev->dm.dc->ctx->dc_bios;
	if (dcb == NULL || dcb->funcs == NULL ||
	    dcb->funcs->is_accelerated_mode == NULL)
		return false;

	return dcb->funcs->is_accelerated_mode(dcb);
}

static bool
amdgpufb_fbdev_restore_ok(struct drmfb_softc *drmfb, bool cold_deferred)
{
	struct amdgpufb_softc *const sc = container_of(drmfb,
	    struct amdgpufb_softc, sc_drmfb);

	/* Keep VBIOS/GOP until X has entered accelerated mode */
	if (cold_deferred)
		return false;

	return amdgpufb_is_accelerated(sc);
}

static void
amdgpufb_fbdev_restored(struct drmfb_softc *drmfb)
{
	struct amdgpufb_softc *const sc = container_of(drmfb,
	    struct amdgpufb_softc, sc_drmfb);
	struct drm_fb_helper *helper = sc->sc_afa.afa_fb_helper;
	struct drm_framebuffer *fb;
	struct amdgpu_bo *abo;
	void *kms_va;
	void *gop_va;
	int stride;
	size_t copysz;

	/*
	 * Prefer the live fbdev BO kmap (VRAM aper).  Attach-time pointer is
	 * the fallback if the helper is not ready yet.
	 */
	kms_va = sc->sc_gtt_va;
	stride = sc->sc_gtt_linebytes > 0 ? (int)sc->sc_gtt_linebytes :
	    (int)sc->sc_drmfb.sc_genfb.sc_stride;
	if (helper != NULL && helper->fb != NULL &&
	    helper->fb->obj[0] != NULL) {
		fb = helper->fb;
		abo = gem_to_amdgpu_bo(fb->obj[0]);
		if (amdgpu_bo_kptr(abo) != NULL)
			kms_va = amdgpu_bo_kptr(abo);
		if (fb->pitches[0] > 0)
			stride = (int)fb->pitches[0];
		DRM_INFO("amdgpufb: restore kms va=%p pitch=%d gpu_addr=0x%llx\n",
		    kms_va, stride,
		    (unsigned long long)amdgpu_bo_gpu_offset(abo));
	}

	if (kms_va == NULL)
		return;

	sc->sc_gtt_va = kms_va;
	sc->sc_gtt_linebytes = stride > 0 ? (uint32_t)stride :
	    sc->sc_gtt_linebytes;

	/*
	 * Seed the KMS BO from the still-mapped GOP before unmap so the
	 * first scanned frame is not a cleared/white buffer if redraw
	 * races the flip.  Same pitch as GOP on this panel (15360).
	 */
	if (sc->sc_gop_console) {
		gop_va = bus_space_vaddr(x86_bus_space_mem, sc->sc_gop_bsh);
		if (gop_va != NULL && sc->sc_gop_mapsz > 0) {
			copysz = sc->sc_gop_mapsz;
			if (helper != NULL && helper->fb != NULL &&
			    helper->fb->obj[0] != NULL) {
				size_t bosz = amdgpu_bo_size(
				    gem_to_amdgpu_bo(helper->fb->obj[0]));
				if (copysz > bosz)
					copysz = bosz;
			}
			memcpy(kms_va, gop_va, copysz);
		}
	}

	/*
	 * KMS now scans the VRAM console BO (same aper path as X).
	 * Stop drawing through GOP and unmap it.
	 */
	genfb_rebind_framebuffer(&sc->sc_drmfb.sc_genfb, kms_va, stride);

	if (sc->sc_gop_console) {
		bus_space_unmap(x86_bus_space_mem, sc->sc_gop_bsh,
		    sc->sc_gop_mapsz);
		sc->sc_gop_console = false;
		aprint_normal_dev(sc->sc_dev,
		    "console rebound to KMS FB after modeset\n");
	}
}

void
amdgpu_fbdev_lastclose(struct drm_device *dev)
{
	struct drm_fb_helper *helper;
	device_t fbdev;
	struct amdgpufb_softc *sc;
	int ret;

	if (dev == NULL)
		return;
	helper = dev->fb_helper;
	if (helper == NULL)
		return;

	fbdev = helper->fbdev;
	if (fbdev == NULL) {
		drm_fb_helper_lastclose(dev);
		return;
	}
	sc = device_private(fbdev);

	/*
	 * Avoid cold first-modeset hang on lastclose of a stray DRM open
	 * before accelerated mode.  After X (or deferred restore) this is
	 * safe and required so the console is not left black.
	 */
	if (!amdgpufb_is_accelerated(sc)) {
		DRM_INFO("amdgpufb: lastclose restore skipped (not accelerated)\n");
		return;
	}

	ret = drm_fb_helper_restore_fbdev_mode_unlocked(helper);
	DRM_INFO("amdgpufb: lastclose fbdev restore ret=%d\n", ret);
	if (ret == 0)
		amdgpufb_fbdev_restored(&sc->sc_drmfb);
}

static paddr_t
amdgpufb_drmfb_mmapfb(struct drmfb_softc *drmfb, off_t offset, int prot)
{
	struct amdgpufb_softc *const sc = container_of(drmfb,
	    struct amdgpufb_softc, sc_drmfb);
	struct drm_fb_helper *const helper = sc->sc_afa.afa_fb_helper;
	struct drm_framebuffer *const fb = helper->fb;
	struct drm_gem_object *const gobj = fb->obj[0];
	struct amdgpu_bo *const rbo = gem_to_amdgpu_bo(gobj);
	struct ttm_resource *const res = rbo->tbo.resource;
	const unsigned num_pages __diagused =
	    (unsigned)(rbo->tbo.base.size >> PAGE_SHIFT);
	int flags = 0;

	/* udv_attach probes with cdev_mmap; must return -1, not KASSERT. */
	if (offset < 0)
		return (paddr_t)-1;

	if (sc->sc_gop_console) {
		if ((uintmax_t)offset >= sc->sc_gop_mapsz)
			return (paddr_t)-1;
		return bus_space_mmap(x86_bus_space_mem, 0,
		    (bus_addr_t)(sc->sc_gop_phys + offset), prot,
		    BUS_SPACE_MAP_PREFETCHABLE);
	}

	if ((uintmax_t)offset >= ((uintmax_t)num_pages << PAGE_SHIFT) ||
	    res == NULL)
		return (paddr_t)-1;

	/* System-RAM-backed BO (rare); VRAM console uses is_iomem aper path. */
	if (!res->bus.is_iomem) {
		struct ttm_tt *const ttm = rbo->tbo.ttm;
		unsigned long page = offset >> PAGE_SHIFT;

		if (ttm == NULL || ttm->pages == NULL ||
		    page >= ttm->num_pages || ttm->pages[page] == NULL)
			return (paddr_t)-1;
		/* System RAM: return page frame directly (no bus tag). */
		return atop(page_to_phys(ttm->pages[page]));
	}

	if (rbo->tbo.bdev->memt == NULL)
		return (paddr_t)-1;

	if (res->bus.caching == ttm_write_combined ||
	    ISSET(res->placement, TTM_PL_FLAG_WC))
		flags |= BUS_SPACE_MAP_PREFETCHABLE;

	/* New TTM: bus.offset is the full physical address. */
	return bus_space_mmap(rbo->tbo.bdev->memt, 0,
	    res->bus.offset + offset, prot, flags);
}
