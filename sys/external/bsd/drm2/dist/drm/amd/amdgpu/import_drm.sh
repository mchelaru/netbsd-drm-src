#!/usr/pkg/bin/bash

export AMDGPU_HOME=/home/kefren/code/drm-work/netbsd-drm-src/sys/external/bsd/drm2/amdgpu
bash ${AMDGPU_HOME}/amdgpu2netbsd /home/kefren/code/linux-all/linux/drivers/gpu/drm/amd/amdgpu/ > ${AMDGPU_HOME}/files-new.amdgpu

