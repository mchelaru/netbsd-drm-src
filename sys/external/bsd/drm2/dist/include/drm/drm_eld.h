#ifndef _DRM_ELD_H_
#define _DRM_ELD_H_
/* NetBSD stub: Linux 6.14 amdgpu_dm expects this; ELD helpers not ported yet. */
static inline int drm_eld_get_conn_type(const uint8_t *eld) { return 0; }
static inline int drm_eld_get_sad_count(const uint8_t *eld) { return 0; }
static inline int drm_eld_get_spk_alloc(const uint8_t *eld) { return 0; }
static inline int drm_eld_get_sad(const uint8_t *eld, int index, void *sad) { return -1; }
#endif
