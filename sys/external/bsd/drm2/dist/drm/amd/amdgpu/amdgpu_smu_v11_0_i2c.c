/*	$NetBSD$	*/

/*
 * Copyright 2019 Advanced Micro Devices, Inc.
 * NetBSD: SMU v11 I2C EEPROM accessor not ported; provide stubs so the
 * module links. RAS/FRU EEPROM paths check for a NULL adapter.
 */

#include "smu_v11_0_i2c.h"
#include "amdgpu.h"

#ifdef __NetBSD__		/* XXX amdgpu smu i2c */

int
smu_v11_0_i2c_control_init(struct amdgpu_device *adev)
{
	(void)adev;
	return 0;
}

void
smu_v11_0_i2c_control_fini(struct amdgpu_device *adev)
{
	(void)adev;
}

#else

#error "Non-NetBSD build should use the upstream smu_v11_0_i2c.c"

#endif /* __NetBSD__ */
