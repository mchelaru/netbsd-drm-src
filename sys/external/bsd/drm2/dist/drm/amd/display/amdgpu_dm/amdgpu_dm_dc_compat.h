/* NetBSD: bridge older amdgpu_dm.c to Linux 6.14 Display Core APIs. */
#ifndef _AMDGPU_DM_DC_COMPAT_H_
#define _AMDGPU_DM_DC_COMPAT_H_

#include "dc.h"
#include "dc_state.h"
#include "dc_stream.h"
#include "core_types.h"

static inline struct dc_state *
dc_create_state(struct dc *dc)
{
	return dc_state_create(dc, NULL);
}

static inline void
dc_release_state(struct dc_state *state)
{
	dc_state_release(state);
}

static inline void
dc_retain_state(struct dc_state *state)
{
	dc_state_retain(state);
}

static inline struct dc_state *
dc_copy_state(struct dc_state *src)
{
	return dc_state_create_copy(src);
}

static inline void
dc_resource_state_construct(struct dc *dc, struct dc_state *state)
{
	dc_state_construct(dc, state);
}

static inline void
dc_resource_state_copy_construct_current(struct dc *dc, struct dc_state *dst)
{
	dc_state_copy_current(dc, dst);
}

static inline bool
dc_add_stream_to_ctx(struct dc *dc, struct dc_state *state,
		     struct dc_stream_state *stream)
{
	return dc_state_add_stream(dc, state, stream) == DC_OK;
}

static inline bool
dc_remove_stream_from_ctx(struct dc *dc, struct dc_state *state,
			  struct dc_stream_state *stream)
{
	return dc_state_remove_stream(dc, state, stream) == DC_OK;
}

static inline bool
dc_add_plane_to_context(struct dc *dc, struct dc_stream_state *stream,
			struct dc_plane_state *plane_state,
			struct dc_state *state)
{
	return dc_state_add_plane(dc, stream, plane_state, state);
}

static inline bool
dc_remove_plane_from_context(struct dc *dc, struct dc_stream_state *stream,
			     struct dc_plane_state *plane_state,
			     struct dc_state *state)
{
	return dc_state_remove_plane(dc, stream, plane_state, state);
}

static inline struct dc_stream_status *
dc_stream_get_status_from_state(struct dc_state *state,
				struct dc_stream_state *stream)
{
	return dc_stream_get_status(stream);
}

static inline bool
dc_commit_state(struct dc *dc, struct dc_state *state)
{
	struct dc_commit_streams_params params = {
		.streams = state->streams,
		.stream_count = state->stream_count,
	};

	return dc_commit_streams(dc, &params) == DC_OK;
}

#endif /* _AMDGPU_DM_DC_COMPAT_H_ */
