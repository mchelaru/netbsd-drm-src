/*	$NetBSD$	*/

/*
 * amdgpu SI sources are always built into the amdgpu module; enable the
 * matching CONFIG_DRM_AMDGPU_SI pieces (atombios helpers, etc.).
 */
#ifndef AMDGPU_SI
#define	AMDGPU_SI
#endif
