/*	$NetBSD: amdgpu_dm_services.c,v 1.2 2021/12/18 23:45:00 riastradh Exp $	*/

/*
 * Copyright 2015 Advanced Micro Devices, Inc.
 *
 * Permission is hereby granted, free of charge, to any person obtaining a
 * copy of this software and associated documentation files (the "Software"),
 * to deal in the Software without restriction, including without limitation
 * the rights to use, copy, modify, merge, publish, distribute, sublicense,
 * and/or sell copies of the Software, and to permit persons to whom the
 * Software is furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.  IN NO EVENT SHALL
 * THE COPYRIGHT HOLDER(S) OR AUTHOR(S) BE LIABLE FOR ANY CLAIM, DAMAGES OR
 * OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE,
 * ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
 * OTHER DEALINGS IN THE SOFTWARE.
 *
 * Authors: AMD
 *
 */

#include <sys/cdefs.h>
__KERNEL_RCSID(0, "$NetBSD: amdgpu_dm_services.c,v 1.2 2021/12/18 23:45:00 riastradh Exp $");

#include <linux/string.h>
#include <linux/acpi.h>

#include <drm/drm_probe_helper.h>
#include <drm/amdgpu_drm.h>
#include "dm_services.h"
#include "amdgpu.h"
#include "amdgpu_dm.h"
#include "amdgpu_dm_irq.h"
#include "amdgpu_pm.h"
#include "dc.h"
#include "dc_dmub_srv.h"
#include "cgs_common.h"



unsigned long long dm_get_elapse_time_in_ns(struct dc_context *ctx,
		unsigned long long current_time_stamp,
		unsigned long long last_time_stamp)
{
	return current_time_stamp - last_time_stamp;
}

void dm_perf_trace_timestamp(const char *func_name, unsigned int line,
			     struct dc_context *ctx)
{
}

bool dm_write_persistent_data(struct dc_context *ctx,
		const struct dc_sink *sink,
		const char *module_name,
		const char *key_name,
		void *params,
		unsigned int size,
		struct persistent_data_flag *flag)
{
	/*TODO implement*/
	return false;
}

bool dm_read_persistent_data(struct dc_context *ctx,
				const struct dc_sink *sink,
				const char *module_name,
				const char *key_name,
				void *params,
				unsigned int size,
				struct persistent_data_flag *flag)
{
	/*TODO implement*/
	return false;
}

/**** power component interfaces ****/

uint32_t dm_read_reg_func(const struct dc_context *ctx, uint32_t address,
			  const char *func_name)
{
	uint32_t value;

	if (address == 0)
		return 0;

	if (ctx->dc)
		dc_exit_ips_for_hw_access(ctx->dc);

	value = cgs_read_register(ctx->cgs_device, address);
	return value;
}

void dm_write_reg_func(const struct dc_context *ctx, uint32_t address,
		       uint32_t value, const char *func_name)
{
	if (address == 0)
		return;

	if (ctx->dc)
		dc_exit_ips_for_hw_access(ctx->dc);

	cgs_write_register(ctx->cgs_device, address, value);
}

void dm_trace_smu_msg(uint32_t msg_id, uint32_t param_in, struct dc_context *ctx)
{
}

void dm_trace_smu_delay(uint32_t delay, struct dc_context *ctx)
{
}

bool dm_execute_dmub_cmd(const struct dc_context *ctx, union dmub_rb_cmd *cmd,
			 enum dm_dmub_wait_type wait_type)
{
	if (!ctx || !ctx->dmub_srv)
		return false;
	return dc_dmub_srv_cmd_run(ctx->dmub_srv, cmd, wait_type);
}

bool dm_execute_dmub_cmd_list(const struct dc_context *ctx, unsigned int count,
			      union dmub_rb_cmd *cmd,
			      enum dm_dmub_wait_type wait_type)
{
	if (!ctx || !ctx->dmub_srv)
		return false;
	return dc_dmub_srv_cmd_run_list(ctx->dmub_srv, count, cmd, wait_type);
}
