/*	$NetBSD: amdgpu_xgmi.c,v 1.1 2021/12/19 12:22:48 riastradh Exp $	*/

/*-
 * Copyright (c) 2021 The NetBSD Foundation, Inc.
 * All rights reserved.
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
__KERNEL_RCSID(0, "$NetBSD: amdgpu_xgmi.c,v 1.1 2021/12/19 12:22:48 riastradh Exp $");

#include <sys/types.h>

#include "amdgpu.h"
#include "amdgpu_xgmi.h"

struct amdgpu_xgmi_ras xgmi_ras;

struct amdgpu_hive_info *
amdgpu_get_xgmi_hive(struct amdgpu_device *adev)
{
	(void)adev;
	return NULL;
}

void
amdgpu_put_xgmi_hive(struct amdgpu_hive_info *hive)
{
	(void)hive;
}

int
amdgpu_xgmi_update_topology(struct amdgpu_hive_info *hive,
    struct amdgpu_device *adev)
{
	(void)hive;
	(void)adev;
	return -ENOSYS;
}

int
amdgpu_xgmi_add_device(struct amdgpu_device *adev)
{
	(void)adev;
	return -ENOSYS;
}

int
amdgpu_xgmi_remove_device(struct amdgpu_device *adev)
{
	(void)adev;
	return 0;
}

int
amdgpu_xgmi_set_pstate(struct amdgpu_device *adev, int pstate)
{
	(void)adev;
	(void)pstate;
	return -ENOSYS;
}

int
amdgpu_xgmi_get_hops_count(struct amdgpu_device *adev,
    struct amdgpu_device *peer_adev)
{
	(void)adev;
	(void)peer_adev;
	return -ENOSYS;
}

int
amdgpu_xgmi_get_num_links(struct amdgpu_device *adev,
    struct amdgpu_device *peer_adev)
{
	(void)adev;
	(void)peer_adev;
	return 0;
}

bool
amdgpu_xgmi_get_is_sharing_enabled(struct amdgpu_device *adev,
    struct amdgpu_device *peer_adev)
{
	(void)adev;
	(void)peer_adev;
	return false;
}

uint64_t
amdgpu_xgmi_get_relative_phy_addr(struct amdgpu_device *adev, uint64_t addr)
{
	(void)adev;
	return addr;
}

int
amdgpu_xgmi_ras_sw_init(struct amdgpu_device *adev)
{
	(void)adev;
	return 0;
}

int
amdgpu_xgmi_reset_on_init(struct amdgpu_device *adev)
{
	(void)adev;
	return 0;
}

int
amdgpu_xgmi_request_nps_change(struct amdgpu_device *adev,
    struct amdgpu_hive_info *hive, int req_nps_mode)
{
	(void)adev;
	(void)hive;
	(void)req_nps_mode;
	return -ENOSYS;
}

int
amdgpu_get_xgmi_link_status(struct amdgpu_device *adev, int global_link_num)
{
	(void)adev;
	(void)global_link_num;
	return 0;
}
