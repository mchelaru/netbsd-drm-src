/*	$NetBSD$	*/

/*
 * Minimal MST/DP connector stubs for NetBSD DCN314 bring-up.
 * Full Linux amdgpu_dm_mst_types.c needs a newer DRM MST core; keep
 * connector aux init working and no-op MST DSC until that is ported.
 */

#include <sys/cdefs.h>
__KERNEL_RCSID(0, "$NetBSD$");

#include <drm/drm_dp_helper.h>

#include "dm_services.h"
#include "amdgpu.h"
#include "amdgpu_dm.h"
#include "amdgpu_dm_mst_types.h"
#include "dc.h"

int
dm_mst_get_pbn_divider(struct dc_link *link)
{
	if (!link)
		return 0;
	/* Approx: link rate (Mbps) * lane_count / 54 (PBN unit) */
	return (link->reported_link_cap.link_rate *
		link->reported_link_cap.lane_count) / 54;
}

static ssize_t
dm_dp_aux_transfer(struct drm_dp_aux *aux, struct drm_dp_aux_msg *msg)
{
	/*
	 * Real transfer lives in older NetBSD dm helpers via dc_link_ddc.
	 * Return EIO until a full AUX path is wired; connector still probes
	 * via DC's own DDC for non-MST.
	 */
	return -EIO;
}

void
amdgpu_dm_initialize_dp_connector(struct amdgpu_display_manager *dm,
				  struct amdgpu_dm_connector *aconnector)
{
	if (!aconnector || !aconnector->dc_link)
		return;

	aconnector->dm_dp_aux.aux.name = "dmdc";
	aconnector->dm_dp_aux.aux.dev = aconnector->base.kdev;
	aconnector->dm_dp_aux.aux.transfer = dm_dp_aux_transfer;
	aconnector->dm_dp_aux.ddc_service = aconnector->dc_link->ddc;
}

#if defined(CONFIG_DRM_AMD_DC_DCN)
bool
compute_mst_dsc_configs_for_state(struct drm_atomic_state *state,
				  struct dc_state *dc_state)
{
	/* MST+DSC fairness not ported yet; allow atomic check to proceed. */
	return true;
}
#endif
