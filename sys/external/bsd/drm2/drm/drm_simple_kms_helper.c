#include <sys/cdefs.h>

#include <drm/drm_crtc.h>
#include <drm/drm_encoder.h>
#include <drm/drm_simple_kms_helper.h>

static const struct drm_encoder_funcs drm_simple_encoder_funcs_cleanup = {
	.destroy = drm_encoder_cleanup,
};

int
drm_simple_encoder_init(struct drm_device *dev,
			struct drm_encoder *encoder,
			int encoder_type)
{
	return drm_encoder_init(dev, encoder,
	    &drm_simple_encoder_funcs_cleanup, encoder_type, NULL);
}
