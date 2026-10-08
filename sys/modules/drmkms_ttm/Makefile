# $NetBSD$

.include "../Makefile.inc"
.include "../drmkms/Makefile.inc"

KMOD=	drmkms_ttm

.PATH:	${S}/external/bsd/drm2/ttm
.PATH:	${S}/external/bsd/drm2/dist/drm/ttm

CPPFLAGS+=	-DCONFIG_AGP=1

WARNS=	3

COPTS.ttm_bo.c+=	${CC_WNO_IMPLICIT_FALLTHROUGH}

CWARNFLAGS+=		-Wno-missing-field-initializers
CWARNFLAGS+=		-Wno-shadow
CWARNFLAGS+=		-Wno-pointer-arith

SRCS+=	ttm_agp_backend.c
SRCS+=	ttm_tt.c
SRCS+=	ttm_bo.c
SRCS+=	ttm_bo_util.c
SRCS+=	ttm_bo_vm.c
SRCS+=	ttm_caching.c
SRCS+=	ttm_module.c
SRCS+=	ttm_execbuf_util.c
SRCS+=	ttm_range_manager.c
SRCS+=	ttm_resource.c
SRCS+=	ttm_pool.c
SRCS+=	ttm_device.c
SRCS+=	ttm_sys_manager.c
SRCS+=	ttm_bus_dma.c
SRCS+=	ttm_tt_nbsd.c

.include <bsd.kmodule.mk>
