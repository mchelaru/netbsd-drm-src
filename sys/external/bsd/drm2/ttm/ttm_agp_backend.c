/*-
 * Stubbed AGP backend for the post-ttm_resource TTM API
 */

#include <sys/cdefs.h>
__KERNEL_RCSID(0, "$NetBSD$");

#include <sys/types.h>
#include <sys/errno.h>
#include <sys/stdbool.h>

#include <drm/ttm/ttm_tt.h>
#include <drm/ttm/ttm_resource.h>

#include <linux/agp_backend.h>

struct ttm_buffer_object;

struct ttm_tt *
ttm_agp_tt_create(struct ttm_buffer_object *bo, struct agp_bridge_data *bridge,
    uint32_t page_flags)
{
	(void)bo;
	(void)bridge;
	(void)page_flags;
	return NULL;
}

int
ttm_agp_bind(struct ttm_tt *ttm, struct ttm_resource *bo_mem)
{
	(void)ttm;
	(void)bo_mem;
	return -ENODEV;
}

void
ttm_agp_unbind(struct ttm_tt *ttm)
{
	(void)ttm;
}

void
ttm_agp_destroy(struct ttm_tt *ttm)
{
	(void)ttm;
}

bool
ttm_agp_is_bound(struct ttm_tt *ttm)
{
	(void)ttm;
	return false;
}

int
ttm_agp_tt_populate(struct ttm_tt *ttm, struct ttm_operation_ctx *ctx)
{
	(void)ttm;
	(void)ctx;
	return -ENODEV;
}

void
ttm_agp_tt_unpopulate(struct ttm_tt *ttm)
{
	(void)ttm;
}
