# $NetBSD: Makefile,v 1.9 2024/04/16 14:34:03 riastradh Exp $

# nbctfmerge / per-obj .SUNW_ctf balloons this module to ~160MB and
# stalls the bootloader at "Loading .../amdgpu.kmod".  Disable CTF
# before Makefile.inc / bsd.own.mk define CTFCONVERT/CTFMERGE.
MKCTF=	no
NOCTF=	# defined

.include "../Makefile.inc"
.include "../drmkms/Makefile.inc"

KMOD=	amdgpu
IOCONF=	amdgpu.ioconf
MKLDSCRIPT=yes

WARNS=	3

.if ${MACHINE_ARCH} == "x86_64"
COPTS.amdgpu_float+=	${${ACTIVE_CC} == "gcc" :? -mhard-float :} -msse -msse2
.elif !empty(MACHINE_ARCH:Maarch64*)
COPTS.amdgpu_float+=	-march=armv8-a
.endif

.if ${MACHINE_ARCH} == "i386" || ${MACHINE_ARCH} == "x86_64" || \
	${MACHINE_CPU} == "arm" || ${MACHINE_CPU} == "aarch64"
CPPFLAGS+=	-DNACPICA=1
.endif

# sed -ne 's,^makeoptions	amdgpu	"\([^.]*\)\.amdgpu"+="\(.*\)",\1+=	\2,gp' <files.amdgpu | sed -e 's,\$S,${S},g'
# Note: order of includes is significant.
CPPFLAGS+=	-I${S}/external/bsd/drm2/dist/drm/amd/include/asic_reg
CPPFLAGS+=	-I${S}/external/bsd/drm2/dist/drm/amd/include
CPPFLAGS+=	-I${S}/external/bsd/drm2/dist/drm/amd/pm/inc
CPPFLAGS+=	-I${S}/external/bsd/drm2/dist/drm/amd/amdgpu
CPPFLAGS+=	-I${S}/external/bsd/drm2/dist/drm/amd/pm/swsmu
CPPFLAGS+=	-I${S}/external/bsd/drm2/dist/drm/amd/pm/swsmu/inc
CPPFLAGS+=	-I${S}/external/bsd/drm2/dist/drm/amd/pm/swsmu/inc/pmfw_if
CPPFLAGS+=	-I${S}/external/bsd/drm2/dist/drm/amd/pm/swsmu/smu11
CPPFLAGS+=	-I${S}/external/bsd/drm2/dist/drm/amd/pm/swsmu/smu12
CPPFLAGS+=	-I${S}/external/bsd/drm2/dist/drm/amd/pm/swsmu/smu13
CPPFLAGS+=	-I${S}/external/bsd/drm2/dist/drm/amd/pm/swsmu/smu14
CPPFLAGS+=	-I${S}/external/bsd/drm2/dist/drm/amd/pm/powerplay/inc
CPPFLAGS+=	-I${S}/external/bsd/drm2/dist/drm/amd/pm/powerplay/smumgr
CPPFLAGS+=	-I${S}/external/bsd/drm2/dist/drm/amd/pm/powerplay/hwmgr
CPPFLAGS+=	-I${S}/external/bsd/drm2/dist/drm/amd/acp/include
CPPFLAGS+=	-I${S}/external/bsd/drm2/dist/drm/amd/display
CPPFLAGS+=	-I${S}/external/bsd/drm2/dist/drm/amd/display/include
CPPFLAGS+=	-I${S}/external/bsd/drm2/dist/drm/amd/display/dc
CPPFLAGS+=	-I${S}/external/bsd/drm2/dist/drm/amd/display/dc/inc
CPPFLAGS+=	-I${S}/external/bsd/drm2/dist/drm/amd/display/dc/inc/hw
CPPFLAGS+=	-I${S}/external/bsd/drm2/dist/drm/amd/display/dc/hwss
CPPFLAGS+=	-I${S}/external/bsd/drm2/dist/drm/amd/display/dc/resource
CPPFLAGS+=	-I${S}/external/bsd/drm2/dist/drm/amd/display/dc/dsc
CPPFLAGS+=	-I${S}/external/bsd/drm2/dist/drm/amd/display/dc/link
CPPFLAGS+=	-I${S}/external/bsd/drm2/dist/drm/amd/display/dc/dml
CPPFLAGS+=	-I${S}/external/bsd/drm2/dist/drm/amd/display/dc/clk_mgr
CPPFLAGS+=	-I${S}/external/bsd/drm2/dist/drm/amd/display/modules/inc
CPPFLAGS+=	-I${S}/external/bsd/drm2/dist/drm/amd/display/modules/hdcp
CPPFLAGS+=	-I${S}/external/bsd/drm2/dist/drm/amd/display/amdgpu_dm
CPPFLAGS+=	-I${S}/external/bsd/drm2/dist/drm/amd/display/dmub/inc
CPPFLAGS+=	-I${S}/external/bsd/drm2/dist/drm/amd/display/dc/dml2
CPPFLAGS+=	-I${S}/external/bsd/drm2/dist/drm/amd/display/dc/dml2/dml21
CPPFLAGS+=	-I${S}/external/bsd/drm2/dist/drm/amd/display/dc/dml2/dml21/inc
CPPFLAGS+=	-I${S}/external/bsd/drm2/dist/drm/amd/display/dc/dml2/dml21/src/inc
CPPFLAGS+=	-I${S}/external/bsd/drm2/dist/drm/amd/display/dc/spl
CPPFLAGS+=	-DCONFIG_DRM_AMD_ACP=1
CPPFLAGS+=	-DCONFIG_DRM_AMD_DC=1
CPPFLAGS+=	-DCONFIG_DRM_AMD_DC_DCN=1
CPPFLAGS+=	-DCONFIG_DRM_AMD_DC_FP=1
# NetBSD -Werror vs Linux DC: soften common noise during DCN314 bring-up
COPTS+=		-Wno-unused-variable -Wno-cast-qual -Wno-discarded-qualifiers
COPTS+=		-Wno-unused-but-set-variable -Wno-error=unused-variable
COPTS+=		-Wno-error=cast-qual -Wno-error=discarded-qualifiers
CPPFLAGS+=	-DCONFIG_PERF_EVENTS=0
CWARNFLAGS+=	-Wno-missing-field-initializers
CWARNFLAGS+=	-Wno-missing-prototypes
CWARNFLAGS+=	-Wno-shadow
CWARNFLAGS+=	-Wno-pointer-arith
CWARNFLAGS+=	-Wno-override-init

# sed -ne 's,^makeoptions	amdgpu	"\([^"]*\.c\)"+="\(.*\)",\1+=	\2,gp' <files.amdgpu | sed -e 's,\$S,${S},g'
CWARNFLAGS.amdgpu_arct_reg_init.c+=	-Wno-cast-qual
CWARNFLAGS.amdgpu_aldebaran_reg_init.c+=	-Wno-cast-qual
CWARNFLAGS.amdgpu_aca.c+=	-Wno-cast-qual -Wno-format ${CC_WNO_MAYBE_UNINITIALIZED} -Wno-overflow
CWARNFLAGS.amdgpu_aqua_vanjaram.c+=	-Wno-cast-qual -Wno-format ${CC_WNO_MAYBE_UNINITIALIZED} -Wno-overflow
CWARNFLAGS.amdgpu_athub_v2_1.c+=	-Wno-cast-qual -Wno-format ${CC_WNO_MAYBE_UNINITIALIZED} -Wno-overflow
CWARNFLAGS.amdgpu_dev_coredump.c+=	-Wno-cast-qual -Wno-format ${CC_WNO_MAYBE_UNINITIALIZED} -Wno-overflow
CWARNFLAGS.amdgpu_df_v4_15.c+=	-Wno-cast-qual -Wno-format ${CC_WNO_MAYBE_UNINITIALIZED} -Wno-overflow
CWARNFLAGS.amdgpu_df_v4_3.c+=	-Wno-cast-qual -Wno-format ${CC_WNO_MAYBE_UNINITIALIZED} -Wno-overflow
CWARNFLAGS.amdgpu_df_v4_6_2.c+=	-Wno-cast-qual -Wno-format ${CC_WNO_MAYBE_UNINITIALIZED} -Wno-overflow
CWARNFLAGS.amdgpu_doorbell_mgr.c+=	-Wno-cast-qual -Wno-format ${CC_WNO_MAYBE_UNINITIALIZED} -Wno-overflow
CWARNFLAGS.amdgpu_eeprom.c+=	-Wno-cast-qual -Wno-format ${CC_WNO_MAYBE_UNINITIALIZED} -Wno-overflow
CWARNFLAGS.amdgpu_gfxhub_v1_2.c+=	-Wno-cast-qual -Wno-format ${CC_WNO_MAYBE_UNINITIALIZED} -Wno-overflow
CWARNFLAGS.amdgpu_gfxhub_v2_1.c+=	-Wno-cast-qual -Wno-format ${CC_WNO_MAYBE_UNINITIALIZED} -Wno-overflow
CWARNFLAGS.amdgpu_gmc_v11_0.c+=	-Wno-cast-qual -Wno-format ${CC_WNO_MAYBE_UNINITIALIZED} -Wno-overflow
CWARNFLAGS.amdgpu_gmc_v12_0.c+=	-Wno-cast-qual -Wno-format ${CC_WNO_MAYBE_UNINITIALIZED} -Wno-overflow
CWARNFLAGS.amdgpu_hdp.c+=	-Wno-cast-qual -Wno-format ${CC_WNO_MAYBE_UNINITIALIZED} -Wno-overflow
CWARNFLAGS.amdgpu_hdp_v4_0.c+=	-Wno-cast-qual -Wno-format ${CC_WNO_MAYBE_UNINITIALIZED} -Wno-overflow
CWARNFLAGS.amdgpu_hdp_v5_0.c+=	-Wno-cast-qual -Wno-format ${CC_WNO_MAYBE_UNINITIALIZED} -Wno-overflow
CWARNFLAGS.amdgpu_hdp_v5_2.c+=	-Wno-cast-qual -Wno-format ${CC_WNO_MAYBE_UNINITIALIZED} -Wno-overflow
CWARNFLAGS.amdgpu_hdp_v6_0.c+=	-Wno-cast-qual -Wno-format ${CC_WNO_MAYBE_UNINITIALIZED} -Wno-overflow
CWARNFLAGS.amdgpu_hdp_v7_0.c+=	-Wno-cast-qual -Wno-format ${CC_WNO_MAYBE_UNINITIALIZED} -Wno-overflow
CWARNFLAGS.amdgpu_ih_v6_0.c+=	-Wno-cast-qual -Wno-format ${CC_WNO_MAYBE_UNINITIALIZED} -Wno-overflow
CWARNFLAGS.amdgpu_ih_v6_1.c+=	-Wno-cast-qual -Wno-format ${CC_WNO_MAYBE_UNINITIALIZED} -Wno-overflow
CWARNFLAGS.amdgpu_ih_v7_0.c+=	-Wno-cast-qual -Wno-format ${CC_WNO_MAYBE_UNINITIALIZED} -Wno-overflow
CWARNFLAGS.amdgpu_jpeg_v3_0.c+=	-Wno-cast-qual -Wno-format ${CC_WNO_MAYBE_UNINITIALIZED} -Wno-overflow
CWARNFLAGS.amdgpu_jpeg_v4_0.c+=	-Wno-cast-qual -Wno-format ${CC_WNO_MAYBE_UNINITIALIZED} -Wno-overflow
CWARNFLAGS.amdgpu_jpeg_v4_0_3.c+=	-Wno-cast-qual -Wno-format ${CC_WNO_MAYBE_UNINITIALIZED} -Wno-overflow
CWARNFLAGS.amdgpu_jpeg_v4_0_5.c+=	-Wno-cast-qual -Wno-format ${CC_WNO_MAYBE_UNINITIALIZED} -Wno-overflow
CWARNFLAGS.amdgpu_jpeg_v5_0_0.c+=	-Wno-cast-qual -Wno-format ${CC_WNO_MAYBE_UNINITIALIZED} -Wno-overflow
CWARNFLAGS.amdgpu_jpeg_v5_0_1.c+=	-Wno-cast-qual -Wno-format ${CC_WNO_MAYBE_UNINITIALIZED} -Wno-overflow
CWARNFLAGS.amdgpu_lsdma_v6_0.c+=	-Wno-cast-qual -Wno-format ${CC_WNO_MAYBE_UNINITIALIZED} -Wno-overflow
CWARNFLAGS.amdgpu_lsdma_v7_0.c+=	-Wno-cast-qual -Wno-format ${CC_WNO_MAYBE_UNINITIALIZED} -Wno-overflow
CWARNFLAGS.amdgpu_mca.c+=	-Wno-cast-qual -Wno-format ${CC_WNO_MAYBE_UNINITIALIZED} -Wno-overflow
CWARNFLAGS.amdgpu_mca_v3_0.c+=	-Wno-cast-qual -Wno-format ${CC_WNO_MAYBE_UNINITIALIZED} -Wno-overflow
CWARNFLAGS.amdgpu_mes.c+=	-Wno-cast-qual -Wno-format ${CC_WNO_MAYBE_UNINITIALIZED} -Wno-overflow
CWARNFLAGS.amdgpu_mes_v11_0.c+=	-Wno-cast-qual -Wno-format ${CC_WNO_MAYBE_UNINITIALIZED} -Wno-overflow
CWARNFLAGS.amdgpu_mes_v12_0.c+=	-Wno-cast-qual -Wno-format ${CC_WNO_MAYBE_UNINITIALIZED} -Wno-overflow
CWARNFLAGS.amdgpu_mmhub_v1_7.c+=	-Wno-cast-qual -Wno-format ${CC_WNO_MAYBE_UNINITIALIZED} -Wno-overflow
CWARNFLAGS.amdgpu_mmhub_v1_8.c+=	-Wno-cast-qual -Wno-format ${CC_WNO_MAYBE_UNINITIALIZED} -Wno-overflow
CWARNFLAGS.amdgpu_mmhub_v2_3.c+=	-Wno-cast-qual -Wno-format ${CC_WNO_MAYBE_UNINITIALIZED} -Wno-overflow
CWARNFLAGS.amdgpu_nbif_v6_3_1.c+=	-Wno-cast-qual -Wno-format ${CC_WNO_MAYBE_UNINITIALIZED} -Wno-overflow
CWARNFLAGS.amdgpu_nbio_v4_3.c+=	-Wno-cast-qual -Wno-format ${CC_WNO_MAYBE_UNINITIALIZED} -Wno-overflow
CWARNFLAGS.amdgpu_nbio_v7_11.c+=	-Wno-cast-qual -Wno-format ${CC_WNO_MAYBE_UNINITIALIZED} -Wno-overflow
CWARNFLAGS.amdgpu_nbio_v7_2.c+=	-Wno-cast-qual -Wno-format ${CC_WNO_MAYBE_UNINITIALIZED} -Wno-overflow
CWARNFLAGS.amdgpu_nbio_v7_7.c+=	-Wno-cast-qual -Wno-format ${CC_WNO_MAYBE_UNINITIALIZED} -Wno-overflow
CWARNFLAGS.amdgpu_nbio_v7_9.c+=	-Wno-cast-qual -Wno-format ${CC_WNO_MAYBE_UNINITIALIZED} -Wno-overflow
CWARNFLAGS.amdgpu_preempt_mgr.c+=	-Wno-cast-qual -Wno-format ${CC_WNO_MAYBE_UNINITIALIZED} -Wno-overflow
CWARNFLAGS.amdgpu_psp_v11_0_8.c+=	-Wno-cast-qual -Wno-format ${CC_WNO_MAYBE_UNINITIALIZED} -Wno-overflow
CWARNFLAGS.amdgpu_psp_v13_0.c+=	-Wno-cast-qual -Wno-format ${CC_WNO_MAYBE_UNINITIALIZED} -Wno-overflow
CWARNFLAGS.amdgpu_psp_v13_0_4.c+=	-Wno-cast-qual -Wno-format ${CC_WNO_MAYBE_UNINITIALIZED} -Wno-overflow
CWARNFLAGS.amdgpu_psp_v14_0.c+=	-Wno-cast-qual -Wno-format ${CC_WNO_MAYBE_UNINITIALIZED} -Wno-overflow
CWARNFLAGS.amdgpu_reset.c+=	-Wno-cast-qual -Wno-format ${CC_WNO_MAYBE_UNINITIALIZED} -Wno-overflow
CWARNFLAGS.amdgpu_ring_mux.c+=	-Wno-cast-qual -Wno-format ${CC_WNO_MAYBE_UNINITIALIZED} -Wno-overflow
CWARNFLAGS.amdgpu_sdma_v4_4.c+=	-Wno-cast-qual -Wno-format ${CC_WNO_MAYBE_UNINITIALIZED} -Wno-overflow
CWARNFLAGS.amdgpu_sdma_v4_4_2.c+=	-Wno-cast-qual -Wno-format ${CC_WNO_MAYBE_UNINITIALIZED} -Wno-overflow
CWARNFLAGS.amdgpu_sdma_v5_2.c+=	-Wno-cast-qual -Wno-format ${CC_WNO_MAYBE_UNINITIALIZED} -Wno-overflow
CWARNFLAGS.amdgpu_sdma_v6_0.c+=	-Wno-cast-qual -Wno-format ${CC_WNO_MAYBE_UNINITIALIZED} -Wno-overflow
CWARNFLAGS.amdgpu_sdma_v7_0.c+=	-Wno-cast-qual -Wno-format ${CC_WNO_MAYBE_UNINITIALIZED} -Wno-overflow
CWARNFLAGS.amdgpu_securedisplay.c+=	-Wno-cast-qual -Wno-format ${CC_WNO_MAYBE_UNINITIALIZED} -Wno-overflow
CWARNFLAGS.amdgpu_dc_spl_isharp_filters.c+=	-Wno-cast-qual
CWARNFLAGS.amdgpu_vg_clk_mgr.c+=	-Wno-discarded-qualifiers
CWARNFLAGS.amdgpu_bios_parser.c+=	-Wno-unused-but-set-variable
CWARNFLAGS.amdgpu_bios_parser2.c+=	-Wno-implicit-fallthrough
CWARNFLAGS.amdgpu_dc_debug.c+=	-Wno-format -Wno-discarded-qualifiers
CWARNFLAGS.amdgpu_dc_helper.c+=	-Wno-discarded-qualifiers
CWARNFLAGS.amdgpu_dce110_hwseq.c+=	-Wno-unused-variable
CWARNFLAGS.amdgpu_dce112_hwseq.c+=	-Wno-unused-variable

CWARNFLAGS.amdgpu_dc_resource.c+=	-Wno-unused-variable -Wno-cast-qual
CWARNFLAGS.amdgpu_dmub_abm_lcd.c+=	-Wno-cast-qual

CWARNFLAGS.amdgpu_link_ddc.c+=	-Wno-unused-variable -Wno-cast-qual -Wno-format -Wno-discarded-qualifiers
CWARNFLAGS.amdgpu_link_detection.c+=	-Wno-unused-variable -Wno-cast-qual -Wno-format -Wno-discarded-qualifiers
CWARNFLAGS.amdgpu_link_dp_capability.c+=	-Wno-unused-variable -Wno-cast-qual -Wno-format -Wno-discarded-qualifiers
CWARNFLAGS.amdgpu_link_dp_cts.c+=	-Wno-unused-variable -Wno-cast-qual -Wno-format -Wno-discarded-qualifiers
CWARNFLAGS.amdgpu_link_dp_dpia.c+=	-Wno-unused-variable -Wno-cast-qual -Wno-format -Wno-discarded-qualifiers
CWARNFLAGS.amdgpu_link_dp_dpia_bw.c+=	-Wno-unused-variable -Wno-cast-qual -Wno-format -Wno-discarded-qualifiers
CWARNFLAGS.amdgpu_link_dp_irq_handler.c+=	-Wno-unused-variable -Wno-cast-qual -Wno-format -Wno-discarded-qualifiers
CWARNFLAGS.amdgpu_link_dp_phy.c+=	-Wno-unused-variable -Wno-cast-qual -Wno-format -Wno-discarded-qualifiers
CWARNFLAGS.amdgpu_link_dp_trace.c+=	-Wno-unused-variable -Wno-cast-qual -Wno-format -Wno-discarded-qualifiers
CWARNFLAGS.amdgpu_link_dp_training.c+=	-Wno-unused-variable -Wno-cast-qual -Wno-format -Wno-discarded-qualifiers
CWARNFLAGS.amdgpu_link_dp_training_128b_132b.c+=	-Wno-unused-variable -Wno-cast-qual -Wno-format -Wno-discarded-qualifiers
CWARNFLAGS.amdgpu_link_dp_training_8b_10b.c+=	-Wno-unused-variable -Wno-cast-qual -Wno-format -Wno-discarded-qualifiers
CWARNFLAGS.amdgpu_link_dp_training_auxless.c+=	-Wno-unused-variable -Wno-cast-qual -Wno-format -Wno-discarded-qualifiers
CWARNFLAGS.amdgpu_link_dp_training_dpia.c+=	-Wno-unused-variable -Wno-cast-qual -Wno-format -Wno-discarded-qualifiers
CWARNFLAGS.amdgpu_link_dp_training_fixed_vs_pe_retimer.c+=	-Wno-unused-variable -Wno-cast-qual -Wno-format -Wno-discarded-qualifiers
CWARNFLAGS.amdgpu_link_dpcd.c+=	-Wno-unused-variable -Wno-cast-qual -Wno-format -Wno-discarded-qualifiers
CWARNFLAGS.amdgpu_link_dpms.c+=	-Wno-unused-variable -Wno-cast-qual -Wno-format -Wno-discarded-qualifiers
CWARNFLAGS.amdgpu_link_edp_panel_control.c+=	-Wno-unused-variable -Wno-cast-qual -Wno-format -Wno-discarded-qualifiers
CWARNFLAGS.amdgpu_link_factory.c+=	-Wno-unused-variable -Wno-cast-qual -Wno-format -Wno-discarded-qualifiers
CWARNFLAGS.amdgpu_link_hpd.c+=	-Wno-unused-variable -Wno-cast-qual -Wno-format -Wno-discarded-qualifiers
CWARNFLAGS.amdgpu_link_hwss_dio.c+=	-Wno-unused-variable -Wno-cast-qual -Wno-format -Wno-discarded-qualifiers
CWARNFLAGS.amdgpu_link_hwss_dio_fixed_vs_pe_retimer.c+=	-Wno-unused-variable -Wno-cast-qual -Wno-format -Wno-discarded-qualifiers
CWARNFLAGS.amdgpu_link_hwss_dpia.c+=	-Wno-unused-variable -Wno-cast-qual -Wno-format -Wno-discarded-qualifiers
CWARNFLAGS.amdgpu_link_hwss_hpo_dp.c+=	-Wno-unused-variable -Wno-cast-qual -Wno-format -Wno-discarded-qualifiers
CWARNFLAGS.amdgpu_link_hwss_hpo_fixed_vs_pe_retimer_dp.c+=	-Wno-unused-variable -Wno-cast-qual -Wno-format -Wno-discarded-qualifiers
CWARNFLAGS.amdgpu_link_resource.c+=	-Wno-unused-variable -Wno-cast-qual -Wno-format -Wno-discarded-qualifiers
CWARNFLAGS.amdgpu_link_validation.c+=	-Wno-unused-variable -Wno-cast-qual -Wno-format -Wno-discarded-qualifiers
CWARNFLAGS.amdgpu_link_dp_cts.c+=	-Wno-unused-variable -Wno-cast-qual
CWARNFLAGS.amdgpu_link_ddc.c+=	-Wno-cast-qual
CWARNFLAGS.amdgpu_link_dp_capability.c+=	-Wno-format

# Newer DC sources: silence NetBSD -Werror noise for unused/format until cleaned
CWARNFLAGS.amdgpu_dcn32_hwseq.c+=	-Wno-unused-variable
CWARNFLAGS.amdgpu_dcn32_clk_mgr.c+=	-Wno-format
CWARNFLAGS.amdgpu_dcn321_resource.c+=	-Wno-unused-variable -Wno-format
CWARNFLAGS.amdgpu_dcn35_clk_mgr.c+=	-Wno-format
CWARNFLAGS.amdgpu_dcn35_hwseq.c+=	-Wno-unused-variable
CWARNFLAGS.amdgpu_dcn351_hwseq.c+=	-Wno-unused-variable
CWARNFLAGS.amdgpu_dcn401_hwseq.c+=	-Wno-unused-variable
CWARNFLAGS.amdgpu_dcn401_resource.c+=	-Wno-unused-variable -Wno-format
CWARNFLAGS.amdgpu_seq64.c+=	-Wno-cast-qual -Wno-format ${CC_WNO_MAYBE_UNINITIALIZED} -Wno-overflow
CWARNFLAGS.amdgpu_smuio_v11_0.c+=	-Wno-cast-qual -Wno-format ${CC_WNO_MAYBE_UNINITIALIZED} -Wno-overflow
CWARNFLAGS.amdgpu_smuio_v11_0_6.c+=	-Wno-cast-qual -Wno-format ${CC_WNO_MAYBE_UNINITIALIZED} -Wno-overflow
CWARNFLAGS.amdgpu_smuio_v13_0.c+=	-Wno-cast-qual -Wno-format ${CC_WNO_MAYBE_UNINITIALIZED} -Wno-overflow
CWARNFLAGS.amdgpu_smuio_v13_0_3.c+=	-Wno-cast-qual -Wno-format ${CC_WNO_MAYBE_UNINITIALIZED} -Wno-overflow
CWARNFLAGS.amdgpu_smuio_v13_0_6.c+=	-Wno-cast-qual -Wno-format ${CC_WNO_MAYBE_UNINITIALIZED} -Wno-overflow
CWARNFLAGS.amdgpu_smuio_v14_0_2.c+=	-Wno-cast-qual -Wno-format ${CC_WNO_MAYBE_UNINITIALIZED} -Wno-overflow
CWARNFLAGS.amdgpu_smuio_v9_0.c+=	-Wno-cast-qual -Wno-format ${CC_WNO_MAYBE_UNINITIALIZED} -Wno-overflow
CWARNFLAGS.amdgpu_soc21.c+=	-Wno-cast-qual -Wno-format ${CC_WNO_MAYBE_UNINITIALIZED} -Wno-overflow
CWARNFLAGS.amdgpu_soc24.c+=	-Wno-cast-qual -Wno-format ${CC_WNO_MAYBE_UNINITIALIZED} -Wno-overflow
CWARNFLAGS.amdgpu_umc_v12_0.c+=	-Wno-cast-qual -Wno-format ${CC_WNO_MAYBE_UNINITIALIZED} -Wno-overflow
CWARNFLAGS.amdgpu_umc_v6_7.c+=	-Wno-cast-qual -Wno-format ${CC_WNO_MAYBE_UNINITIALIZED} -Wno-overflow
CWARNFLAGS.amdgpu_umc_v8_7.c+=	-Wno-cast-qual -Wno-format ${CC_WNO_MAYBE_UNINITIALIZED} -Wno-overflow
CWARNFLAGS.amdgpu_umsch_mm.c+=	-Wno-cast-qual -Wno-format ${CC_WNO_MAYBE_UNINITIALIZED} -Wno-overflow
CWARNFLAGS.amdgpu_uvd_v3_1.c+=	-Wno-cast-qual -Wno-format ${CC_WNO_MAYBE_UNINITIALIZED} -Wno-overflow
CWARNFLAGS.amdgpu_vcn_v3_0.c+=	-Wno-cast-qual -Wno-format ${CC_WNO_MAYBE_UNINITIALIZED} -Wno-overflow
CWARNFLAGS.amdgpu_vcn_v4_0.c+=	-Wno-cast-qual -Wno-format ${CC_WNO_MAYBE_UNINITIALIZED} -Wno-overflow
CWARNFLAGS.amdgpu_vcn_v4_0_3.c+=	-Wno-cast-qual -Wno-format ${CC_WNO_MAYBE_UNINITIALIZED} -Wno-overflow
CWARNFLAGS.amdgpu_vcn_v4_0_5.c+=	-Wno-cast-qual -Wno-format ${CC_WNO_MAYBE_UNINITIALIZED} -Wno-overflow
CWARNFLAGS.amdgpu_vcn_v5_0_0.c+=	-Wno-cast-qual -Wno-format ${CC_WNO_MAYBE_UNINITIALIZED} -Wno-overflow
CWARNFLAGS.amdgpu_vcn_v5_0_1.c+=	-Wno-cast-qual -Wno-format ${CC_WNO_MAYBE_UNINITIALIZED} -Wno-overflow
CWARNFLAGS.amdgpu_vega20_ih.c+=	-Wno-cast-qual -Wno-format ${CC_WNO_MAYBE_UNINITIALIZED} -Wno-overflow
CWARNFLAGS.amdgpu_vkms.c+=	-Wno-cast-qual -Wno-format ${CC_WNO_MAYBE_UNINITIALIZED} -Wno-overflow
CWARNFLAGS.amdgpu_imu_v11_0_3.c+=	-Wno-cast-qual -Wno-format ${CC_WNO_MAYBE_UNINITIALIZED} -Wno-overflow
CWARNFLAGS.amdgpu_vpe_v6_1.c+=	-Wno-cast-qual -Wno-format ${CC_WNO_MAYBE_UNINITIALIZED} -Wno-overflow
CWARNFLAGS.amdgpu_umsch_mm_v4_0.c+=	-Wno-cast-qual -Wno-format ${CC_WNO_MAYBE_UNINITIALIZED} -Wno-overflow
CWARNFLAGS.amdgpu_umc_v8_14.c+=	-Wno-cast-qual -Wno-format ${CC_WNO_MAYBE_UNINITIALIZED} -Wno-overflow
CWARNFLAGS.amdgpu_umc_v8_10.c+=	-Wno-cast-qual -Wno-format ${CC_WNO_MAYBE_UNINITIALIZED} -Wno-overflow
CWARNFLAGS.amdgpu_smu_v13_0_10.c+=	-Wno-cast-qual -Wno-format ${CC_WNO_MAYBE_UNINITIALIZED} -Wno-overflow
CWARNFLAGS.amdgpu_sienna_cichlid.c+=	-Wno-cast-qual -Wno-format ${CC_WNO_MAYBE_UNINITIALIZED} -Wno-overflow
CWARNFLAGS.amdgpu_mmhub_v4_1_0.c+=	-Wno-cast-qual -Wno-format ${CC_WNO_MAYBE_UNINITIALIZED} -Wno-overflow
CWARNFLAGS.amdgpu_mmhub_v3_3.c+=	-Wno-cast-qual -Wno-format ${CC_WNO_MAYBE_UNINITIALIZED} -Wno-overflow
CWARNFLAGS.amdgpu_mmhub_v3_0_2.c+=	-Wno-cast-qual -Wno-format ${CC_WNO_MAYBE_UNINITIALIZED} -Wno-overflow
CWARNFLAGS.amdgpu_mmhub_v3_0_1.c+=	-Wno-cast-qual -Wno-format ${CC_WNO_MAYBE_UNINITIALIZED} -Wno-overflow
CWARNFLAGS.amdgpu_mmhub_v3_0.c+=	-Wno-cast-qual -Wno-format ${CC_WNO_MAYBE_UNINITIALIZED} -Wno-overflow
CWARNFLAGS.amdgpu_lsdma.c+=	-Wno-cast-qual -Wno-format ${CC_WNO_MAYBE_UNINITIALIZED} -Wno-overflow
CWARNFLAGS.amdgpu_imu_v12_0.c+=	-Wno-cast-qual -Wno-format ${CC_WNO_MAYBE_UNINITIALIZED} -Wno-overflow
CWARNFLAGS.amdgpu_imu_v11_0.c+=	-Wno-cast-qual -Wno-format ${CC_WNO_MAYBE_UNINITIALIZED} -Wno-overflow
CWARNFLAGS.amdgpu_gfxhub_v12_0.c+=	-Wno-cast-qual -Wno-format ${CC_WNO_MAYBE_UNINITIALIZED} -Wno-overflow
CWARNFLAGS.amdgpu_gfxhub_v11_5_0.c+=	-Wno-cast-qual -Wno-format ${CC_WNO_MAYBE_UNINITIALIZED} -Wno-overflow
CWARNFLAGS.amdgpu_gfxhub_v3_0_3.c+=	-Wno-cast-qual -Wno-format ${CC_WNO_MAYBE_UNINITIALIZED} -Wno-overflow
CWARNFLAGS.amdgpu_gfxhub_v3_0.c+=	-Wno-cast-qual -Wno-format ${CC_WNO_MAYBE_UNINITIALIZED} -Wno-overflow
CWARNFLAGS.amdgpu_gfx_v11_0_3.c+=	-Wno-cast-qual -Wno-format ${CC_WNO_MAYBE_UNINITIALIZED} -Wno-overflow
CWARNFLAGS.amdgpu_athub_v4_1_0.c+=	-Wno-cast-qual -Wno-format ${CC_WNO_MAYBE_UNINITIALIZED} -Wno-overflow
CWARNFLAGS.amdgpu_athub_v3_0.c+=	-Wno-cast-qual -Wno-format ${CC_WNO_MAYBE_UNINITIALIZED} -Wno-overflow
CWARNFLAGS.amdgpu_aldebaran.c+=	-Wno-cast-qual -Wno-format ${CC_WNO_MAYBE_UNINITIALIZED} -Wno-overflow
CWARNFLAGS.amdgpu_vm_pt.c+=	-Wno-cast-qual -Wno-format ${CC_WNO_MAYBE_UNINITIALIZED} -Wno-overflow
CWARNFLAGS.amdgpu_vm_tlb_fence.c+=	-Wno-cast-qual -Wno-format ${CC_WNO_MAYBE_UNINITIALIZED} -Wno-overflow
CWARNFLAGS.amdgpu_vpe.c+=	-Wno-cast-qual -Wno-format ${CC_WNO_MAYBE_UNINITIALIZED} -Wno-overflow
CWARNFLAGS.amdgpu_xcp.c+=	-Wno-cast-qual -Wno-format ${CC_WNO_MAYBE_UNINITIALIZED} -Wno-overflow
CWARNFLAGS.amdgpu_bo_list.c+=	-Wno-type-limits -Wno-tautological-constant-out-of-range-compare
CWARNFLAGS.fiji_smumgr.c+=	-Wno-cast-qual
CWARNFLAGS.amdgpu_hw_ddc.c+=	-Wno-type-limits
CWARNFLAGS.amdgpu_hw_generic.c+=	-Wno-type-limits
CWARNFLAGS.amdgpu_hw_hpd.c+=	-Wno-type-limits
CWARNFLAGS.navi10_ppt.c+=	-Wno-type-limits
CWARNFLAGS.polaris10_smumgr.c+=	-Wno-cast-qual
CWARNFLAGS.process_pptables_v1_0.c+=	-Wno-cast-qual
CWARNFLAGS.amdgpu_rlc.c+=	${CC_WNO_MAYBE_UNINITIALIZED}
CWARNFLAGS.amdgpu_rn_clk_mgr.c+=	-Wno-type-limits
CWARNFLAGS.amdgpu_vega10_reg_init.c+=	-Wno-cast-qual
CWARNFLAGS.amdgpu_vega20_reg_init.c+=	-Wno-cast-qual
CWARNFLAGS.smu_cmn.c+=	-Wno-cast-qual -Wno-format ${CC_WNO_MAYBE_UNINITIALIZED} -Wno-overflow -Wno-discarded-qualifiers
CWARNFLAGS.cyan_skillfish_ppt.c+=	-Wno-cast-qual -Wno-format ${CC_WNO_MAYBE_UNINITIALIZED} -Wno-overflow
CWARNFLAGS.sienna_cichlid_ppt.c+=	-Wno-cast-qual -Wno-format ${CC_WNO_MAYBE_UNINITIALIZED} -Wno-overflow
CWARNFLAGS.vangogh_ppt.c+=	-Wno-cast-qual -Wno-format ${CC_WNO_MAYBE_UNINITIALIZED} -Wno-overflow
CWARNFLAGS.aldebaran_ppt.c+=	-Wno-cast-qual -Wno-format ${CC_WNO_MAYBE_UNINITIALIZED} -Wno-overflow
CWARNFLAGS.smu_v13_0.c+=	-Wno-cast-qual -Wno-format ${CC_WNO_MAYBE_UNINITIALIZED} -Wno-overflow
CWARNFLAGS.smu_v13_0_0_ppt.c+=	-Wno-cast-qual -Wno-format ${CC_WNO_MAYBE_UNINITIALIZED} -Wno-overflow
CWARNFLAGS.smu_v13_0_4_ppt.c+=	-Wno-cast-qual -Wno-format ${CC_WNO_MAYBE_UNINITIALIZED} -Wno-overflow
CWARNFLAGS.smu_v13_0_5_ppt.c+=	-Wno-cast-qual -Wno-format ${CC_WNO_MAYBE_UNINITIALIZED} -Wno-overflow
CWARNFLAGS.smu_v13_0_6_ppt.c+=	-Wno-cast-qual -Wno-format ${CC_WNO_MAYBE_UNINITIALIZED} -Wno-overflow
CWARNFLAGS.smu_v13_0_7_ppt.c+=	-Wno-cast-qual -Wno-format ${CC_WNO_MAYBE_UNINITIALIZED} -Wno-overflow
CWARNFLAGS.yellow_carp_ppt.c+=	-Wno-cast-qual -Wno-format ${CC_WNO_MAYBE_UNINITIALIZED} -Wno-overflow
CWARNFLAGS.smu_v14_0.c+=	-Wno-cast-qual -Wno-format ${CC_WNO_MAYBE_UNINITIALIZED} -Wno-overflow
CWARNFLAGS.smu_v14_0_0_ppt.c+=	-Wno-cast-qual -Wno-format ${CC_WNO_MAYBE_UNINITIALIZED} -Wno-overflow
CWARNFLAGS.smu_v14_0_2_ppt.c+=	-Wno-cast-qual -Wno-format ${CC_WNO_MAYBE_UNINITIALIZED} -Wno-overflow
CWARNFLAGS.amdgpu_dpm_internal.c+=	-Wno-cast-qual -Wno-format ${CC_WNO_MAYBE_UNINITIALIZED} -Wno-overflow
CWARNFLAGS.amdgpu_smu.c+=	-Wno-cast-qual -Wno-format ${CC_WNO_MAYBE_UNINITIALIZED} -Wno-overflow
CWARNFLAGS.smu_v11_0.c+=	-Wno-cast-qual -Wno-format ${CC_WNO_MAYBE_UNINITIALIZED} -Wno-overflow
CWARNFLAGS.smu_v12_0.c+=	-Wno-cast-qual -Wno-format ${CC_WNO_MAYBE_UNINITIALIZED} -Wno-overflow
CWARNFLAGS.arcturus_ppt.c+=	-Wno-cast-qual -Wno-format ${CC_WNO_MAYBE_UNINITIALIZED} -Wno-overflow
CWARNFLAGS.renoir_ppt.c+=	-Wno-cast-qual -Wno-format ${CC_WNO_MAYBE_UNINITIALIZED} -Wno-overflow
CWARNFLAGS.amdgpu_dpm.c+=	-Wno-cast-qual -Wno-format ${CC_WNO_MAYBE_UNINITIALIZED} -Wno-overflow
CWARNFLAGS.legacy_dpm.c+=	-Wno-cast-qual -Wno-format ${CC_WNO_MAYBE_UNINITIALIZED} -Wno-overflow
CWARNFLAGS.amdgpu_pm.c+=	-Wno-cast-qual -Wno-format ${CC_WNO_MAYBE_UNINITIALIZED} -Wno-overflow
CWARNFLAGS.amdgpu_gfx_v11_0.c+=	-Wno-cast-qual -Wno-format ${CC_WNO_MAYBE_UNINITIALIZED} -Wno-overflow
CWARNFLAGS.amdgpu_gfx_v12_0.c+=	-Wno-cast-qual -Wno-format ${CC_WNO_MAYBE_UNINITIALIZED} -Wno-overflow
CWARNFLAGS.amdgpu_gfx_v9_4_2.c+=	-Wno-cast-qual -Wno-format
CWARNFLAGS.amdgpu_gfx_v9_4_3.c+=	-Wno-cast-qual -Wno-format ${CC_WNO_MAYBE_UNINITIALIZED} -Wno-overflow -Wno-overflow
CWARNFLAGS.amdgpu_fru_eeprom.c+=	-Wno-format
CWARNFLAGS.amdgpu_uvd.c+=	-Wno-format
CWARNFLAGS.amdgpu_vcn.c+=	-Wno-format
COPTS.amdgpu_dcn10_resource.c+=	${COPTS.amdgpu_float}
COPTS.amdgpu_dcn20_resource.c+=	${COPTS.amdgpu_float}
COPTS.amdgpu_dcn21_resource.c+=	${COPTS.amdgpu_float}
COPTS.amdgpu_dcn_calc_auto.c+=	${COPTS.amdgpu_float}
COPTS.amdgpu_dcn_calc_math.c+=	${COPTS.amdgpu_float}
COPTS.amdgpu_dcn_calcs.c+=	${COPTS.amdgpu_float}
COPTS.amdgpu_display_mode_vba.c+=	${COPTS.amdgpu_float}
COPTS.amdgpu_display_mode_vba_20.c+=	${COPTS.amdgpu_float}
COPTS.amdgpu_display_mode_vba_20v2.c+=	${COPTS.amdgpu_float}
COPTS.amdgpu_display_mode_vba_21.c+=	${COPTS.amdgpu_float}
COPTS.amdgpu_display_rq_dlg_calc_20.c+=	${COPTS.amdgpu_float}
COPTS.amdgpu_display_rq_dlg_calc_20v2.c+=	${COPTS.amdgpu_float}
COPTS.amdgpu_display_rq_dlg_calc_21.c+=	${COPTS.amdgpu_float}
COPTS.amdgpu_display_rq_dlg_helpers.c+=	${COPTS.amdgpu_float}
COPTS.amdgpu_dml1_display_rq_dlg_calc.c+=	${COPTS.amdgpu_float}
COPTS.amdgpu_dml_common_defs.c+=	${COPTS.amdgpu_float}
COPTS.amdgpu_rc_calc.c+=	${COPTS.amdgpu_float}
COPTS.amdgpu_rc_calc_dpi.c+=	${COPTS.amdgpu_float}
COPTS.amdgpu_rn_clk_mgr.c+=	${COPTS.amdgpu_float}

# sed -ne 's,^file	\(external/bsd/drm2/.*\)/[^/ 	]*	.*,.PATH:	\1,gp' <files.amdgpu | sort -u
# pm/swsmu before amdgpu and powerplay so overlapping basenames resolve from swsmu.
.PATH:	${S}/external/bsd/drm2/dist/drm/amd/amdgpu/../pm
.PATH:	${S}/external/bsd/drm2/dist/drm/amd/amdgpu/../pm/legacy-dpm
.PATH:	${S}/external/bsd/drm2/dist/drm/amd/amdgpu/../pm/swsmu
.PATH:	${S}/external/bsd/drm2/dist/drm/amd/amdgpu/../pm/swsmu/smu11
.PATH:	${S}/external/bsd/drm2/dist/drm/amd/amdgpu/../pm/swsmu/smu12
.PATH:	${S}/external/bsd/drm2/dist/drm/amd/amdgpu/../pm/swsmu/smu13
.PATH:	${S}/external/bsd/drm2/dist/drm/amd/amdgpu/../pm/swsmu/smu14
.PATH:	${S}/external/bsd/drm2/amdgpu
.PATH:	${S}/external/bsd/drm2/dist/drm/amd/amdgpu
.PATH:	${S}/external/bsd/drm2/dist/drm/amd/amdgpu/../acp
.PATH:	${S}/external/bsd/drm2/dist/drm/amd/amdgpu/../display/amdgpu_dm
.PATH:	${S}/external/bsd/drm2/dist/drm/amd/amdgpu/../display/dc
.PATH:	${S}/external/bsd/drm2/dist/drm/amd/amdgpu/../display/dc/basics
.PATH:	${S}/external/bsd/drm2/dist/drm/amd/amdgpu/../display/dc/bios
.PATH:	${S}/external/bsd/drm2/dist/drm/amd/amdgpu/../display/dc/bios/dce110
.PATH:	${S}/external/bsd/drm2/dist/drm/amd/amdgpu/../display/dc/bios/dce112
.PATH:	${S}/external/bsd/drm2/dist/drm/amd/amdgpu/../display/dc/bios/dce80
.PATH:	${S}/external/bsd/drm2/dist/drm/amd/amdgpu/../display/dc/calcs
.PATH:	${S}/external/bsd/drm2/dist/drm/amd/amdgpu/../display/dc/clk_mgr
.PATH:	${S}/external/bsd/drm2/dist/drm/amd/amdgpu/../display/dc/clk_mgr/dce100
.PATH:	${S}/external/bsd/drm2/dist/drm/amd/amdgpu/../display/dc/clk_mgr/dce110
.PATH:	${S}/external/bsd/drm2/dist/drm/amd/amdgpu/../display/dc/clk_mgr/dce112
.PATH:	${S}/external/bsd/drm2/dist/drm/amd/amdgpu/../display/dc/clk_mgr/dce120
.PATH:	${S}/external/bsd/drm2/dist/drm/amd/amdgpu/../display/dc/clk_mgr/dcn10
.PATH:	${S}/external/bsd/drm2/dist/drm/amd/amdgpu/../display/dc/clk_mgr/dcn20
.PATH:	${S}/external/bsd/drm2/dist/drm/amd/amdgpu/../display/dc/clk_mgr/dcn21
.PATH:	${S}/external/bsd/drm2/dist/drm/amd/amdgpu/../display/dc/core
.PATH:	${S}/external/bsd/drm2/dist/drm/amd/amdgpu/../display/dc/dce
.PATH:	${S}/external/bsd/drm2/dist/drm/amd/amdgpu/../display/dc/dce100
.PATH:	${S}/external/bsd/drm2/dist/drm/amd/amdgpu/../display/dc/dce110
.PATH:	${S}/external/bsd/drm2/dist/drm/amd/amdgpu/../display/dc/dce112
.PATH:	${S}/external/bsd/drm2/dist/drm/amd/amdgpu/../display/dc/dce120
.PATH:	${S}/external/bsd/drm2/dist/drm/amd/amdgpu/../display/dc/dce80
.PATH:	${S}/external/bsd/drm2/dist/drm/amd/amdgpu/../display/dc/dcn10
.PATH:	${S}/external/bsd/drm2/dist/drm/amd/amdgpu/../display/dc/dcn20
.PATH:	${S}/external/bsd/drm2/dist/drm/amd/amdgpu/../display/dc/dcn21
.PATH:	${S}/external/bsd/drm2/dist/drm/amd/amdgpu/../display/dc/dml
.PATH:	${S}/external/bsd/drm2/dist/drm/amd/amdgpu/../display/dc/dml/dcn20
.PATH:	${S}/external/bsd/drm2/dist/drm/amd/amdgpu/../display/dc/dml/dcn21
.PATH:	${S}/external/bsd/drm2/dist/drm/amd/amdgpu/../display/dc/dsc
.PATH:	${S}/external/bsd/drm2/dist/drm/amd/amdgpu/../display/dc/gpio
.PATH:	${S}/external/bsd/drm2/dist/drm/amd/amdgpu/../display/dc/gpio/dce110
.PATH:	${S}/external/bsd/drm2/dist/drm/amd/amdgpu/../display/dc/gpio/dce120
.PATH:	${S}/external/bsd/drm2/dist/drm/amd/amdgpu/../display/dc/gpio/dce80
.PATH:	${S}/external/bsd/drm2/dist/drm/amd/amdgpu/../display/dc/gpio/dcn10
.PATH:	${S}/external/bsd/drm2/dist/drm/amd/amdgpu/../display/dc/gpio/dcn20
.PATH:	${S}/external/bsd/drm2/dist/drm/amd/amdgpu/../display/dc/gpio/dcn21
.PATH:	${S}/external/bsd/drm2/dist/drm/amd/amdgpu/../display/dc/gpio/dcn30
.PATH:	${S}/external/bsd/drm2/dist/drm/amd/amdgpu/../display/dc/gpio/diagnostics
.PATH:	${S}/external/bsd/drm2/dist/drm/amd/amdgpu/../display/dc/hdcp
.PATH:	${S}/external/bsd/drm2/dist/drm/amd/amdgpu/../display/dc/irq
.PATH:	${S}/external/bsd/drm2/dist/drm/amd/amdgpu/../display/dc/irq/dce110
.PATH:	${S}/external/bsd/drm2/dist/drm/amd/amdgpu/../display/dc/irq/dce120
.PATH:	${S}/external/bsd/drm2/dist/drm/amd/amdgpu/../display/dc/irq/dce80
.PATH:	${S}/external/bsd/drm2/dist/drm/amd/amdgpu/../display/dc/irq/dcn10
.PATH:	${S}/external/bsd/drm2/dist/drm/amd/amdgpu/../display/dc/irq/dcn20
.PATH:	${S}/external/bsd/drm2/dist/drm/amd/amdgpu/../display/dc/irq/dcn21
.PATH:	${S}/external/bsd/drm2/dist/drm/amd/amdgpu/../display/dc/virtual
.PATH:	${S}/external/bsd/drm2/dist/drm/amd/amdgpu/../display/dmub/src
.PATH:	${S}/external/bsd/drm2/dist/drm/amd/amdgpu/../display/modules/color
.PATH:	${S}/external/bsd/drm2/dist/drm/amd/amdgpu/../display/modules/freesync
.PATH:	${S}/external/bsd/drm2/dist/drm/amd/amdgpu/../display/modules/hdcp
.PATH:	${S}/external/bsd/drm2/dist/drm/amd/amdgpu/../display/modules/info_packet
.PATH:	${S}/external/bsd/drm2/dist/drm/amd/amdgpu/../display/modules/power
.PATH:	${S}/external/bsd/drm2/dist/drm/amd/amdgpu/../pm/powerplay
.PATH:	${S}/external/bsd/drm2/dist/drm/amd/amdgpu/../pm/powerplay/hwmgr
.PATH:	${S}/external/bsd/drm2/dist/drm/amd/amdgpu/../pm/powerplay/smumgr

# sed -ne 's,^file	external/bsd/drm2/.*/\([^/ 	]*\)	.*,SRCS+=	\1,gp' <files.amdgpu | sort -u
SRCS+=	amdgpu_aca.c
SRCS+=	amdgpu_acp.c
SRCS+=	amdgpu_acp_hw.c
SRCS+=	amdgpu_acpi.c
SRCS+=	amdgpu_afmt.c
SRCS+=	aldebaran_ppt.c
SRCS+=	amdgpu_aldebaran_reg_init.c
SRCS+=	amd_powerplay.c
SRCS+=	amdgpu_amdkfd.c
SRCS+=	amdgpu_aqua_vanjaram.c
SRCS+=	amdgpu_arct_reg_init.c
SRCS+=	arcturus_ppt.c
SRCS+=	amdgpu_athub_v1_0.c
SRCS+=	amdgpu_athub_v2_0.c
SRCS+=	amdgpu_athub_v2_1.c
SRCS+=	amdgpu_atom.c
SRCS+=	amdgpu_atombios.c
SRCS+=	amdgpu_atombios_crtc.c
SRCS+=	amdgpu_atombios_dp.c
SRCS+=	amdgpu_atombios_encoders.c
SRCS+=	amdgpu_atombios_i2c.c
SRCS+=	amdgpu_atomfirmware.c
SRCS+=	amdgpu_benchmark.c
SRCS+=	amdgpu_bios.c
SRCS+=	amdgpu_bios_parser.c
SRCS+=	amdgpu_bios_parser2.c
SRCS+=	amdgpu_bios_parser_common.c
SRCS+=	amdgpu_bios_parser_helper.c
SRCS+=	amdgpu_bios_parser_interface.c
SRCS+=	amdgpu_bo_list.c
SRCS+=	amdgpu_bw_fixed.c
SRCS+=	amdgpu_cgs.c
SRCS+=	ci_baco.c
SRCS+=	ci_smumgr.c
SRCS+=	amdgpu_cik.c
SRCS+=	amdgpu_cik_ih.c
SRCS+=	amdgpu_cik_sdma.c
SRCS+=	amdgpu_clk_mgr.c
SRCS+=	amdgpu_color_gamma.c
SRCS+=	amdgpu_command_table.c
SRCS+=	amdgpu_command_table2.c
SRCS+=	amdgpu_command_table_helper.c
SRCS+=	amdgpu_command_table_helper2.c
SRCS+=	amdgpu_command_table_helper2_dce112.c
SRCS+=	amdgpu_command_table_helper_dce110.c
SRCS+=	amdgpu_command_table_helper_dce112.c
SRCS+=	amdgpu_command_table_helper_dce80.c
SRCS+=	common_baco.c
SRCS+=	amdgpu_connectors.c
SRCS+=	amdgpu_conversion.c
SRCS+=	amdgpu_cs.c
SRCS+=	amdgpu_csa.c
SRCS+=	amdgpu_ctx.c
SRCS+=	amdgpu_custom_float.c
SRCS+=	cyan_skillfish_ppt.c
SRCS+=	amdgpu_cz_ih.c
SRCS+=	amdgpu_dc.c
SRCS+=	amdgpu_dc_common.c
SRCS+=	amdgpu_dc_debug.c
SRCS+=	amdgpu_dc_dmub_srv.c
SRCS+=	amdgpu_dc_dsc.c
SRCS+=	amdgpu_dc_helper.c
SRCS+=	amdgpu_dc_hw_sequencer.c
SRCS+=	amdgpu_dc_resource.c
SRCS+=	amdgpu_dc_sink.c
SRCS+=	amdgpu_dc_stream.c
SRCS+=	amdgpu_dc_surface.c
SRCS+=	amdgpu_dc_vm_helper.c
SRCS+=	amdgpu_dce100_hwseq.c
SRCS+=	amdgpu_dce100_resource.c
SRCS+=	amdgpu_dce110_clk_mgr.c
SRCS+=	amdgpu_dce110_compressor.c
SRCS+=	amdgpu_dce110_hwseq.c
SRCS+=	amdgpu_dce110_mem_input_v.c
SRCS+=	amdgpu_dce110_opp_csc_v.c
SRCS+=	amdgpu_dce110_opp_regamma_v.c
SRCS+=	amdgpu_dce110_opp_v.c
SRCS+=	amdgpu_dce110_resource.c
SRCS+=	amdgpu_dce110_timing_generator.c
SRCS+=	amdgpu_dce110_timing_generator_v.c
SRCS+=	amdgpu_dce110_transform_v.c
SRCS+=	amdgpu_dce112_clk_mgr.c
SRCS+=	amdgpu_dce112_compressor.c
SRCS+=	amdgpu_dce112_hwseq.c
SRCS+=	amdgpu_dce112_resource.c
SRCS+=	amdgpu_dce120_clk_mgr.c
SRCS+=	amdgpu_dce120_hwseq.c
SRCS+=	amdgpu_dce120_resource.c
SRCS+=	amdgpu_dce120_timing_generator.c
SRCS+=	amdgpu_dce80_hwseq.c
SRCS+=	amdgpu_dce80_resource.c
SRCS+=	amdgpu_dce80_timing_generator.c
SRCS+=	amdgpu_dce_abm.c
SRCS+=	amdgpu_dce_audio.c
SRCS+=	amdgpu_dce_aux.c
SRCS+=	amdgpu_dce_calcs.c
SRCS+=	amdgpu_dce_clk_mgr.c
SRCS+=	amdgpu_dce_clock_source.c
SRCS+=	amdgpu_dce_dmcu.c
SRCS+=	amdgpu_dce_hwseq.c
SRCS+=	amdgpu_dce_i2c.c
SRCS+=	amdgpu_dce_i2c_hw.c
SRCS+=	amdgpu_dce_i2c_sw.c
SRCS+=	amdgpu_dce_ipp.c
SRCS+=	amdgpu_dce_link_encoder.c
SRCS+=	amdgpu_dce_mem_input.c
SRCS+=	amdgpu_dce_opp.c
SRCS+=	amdgpu_dce_scl_filters.c
SRCS+=	amdgpu_dce_stream_encoder.c
SRCS+=	amdgpu_dce_transform.c
SRCS+=	amdgpu_dce_v10_0.c
SRCS+=	amdgpu_dce_v11_0.c
SRCS+=	amdgpu_dce_v6_0.c
SRCS+=	amdgpu_dce_v8_0.c
SRCS+=	amdgpu_dce_virtual.c
SRCS+=	amdgpu_dcn10_cm_common.c
SRCS+=	amdgpu_dcn10_dpp.c
SRCS+=	amdgpu_dcn10_dpp_cm.c
SRCS+=	amdgpu_dcn10_dpp_dscl.c
SRCS+=	amdgpu_dcn10_hubbub.c
SRCS+=	amdgpu_dcn10_hubp.c
SRCS+=	amdgpu_dcn10_hwseq.c
SRCS+=	amdgpu_dcn10_hw_sequencer_debug.c
SRCS+=	amdgpu_dcn10_init.c
SRCS+=	amdgpu_dcn10_ipp.c
SRCS+=	amdgpu_dcn10_link_encoder.c
SRCS+=	amdgpu_dcn10_mpc.c
SRCS+=	amdgpu_dcn10_opp.c
SRCS+=	amdgpu_dcn10_optc.c
SRCS+=	amdgpu_dcn10_resource.c
SRCS+=	amdgpu_dcn10_stream_encoder.c
SRCS+=	amdgpu_dcn20_clk_mgr.c
SRCS+=	amdgpu_dcn20_dccg.c
SRCS+=	amdgpu_dcn20_dpp.c
SRCS+=	amdgpu_dcn20_dpp_cm.c
SRCS+=	amdgpu_dcn20_dsc.c
SRCS+=	amdgpu_dcn20_dwb.c
SRCS+=	amdgpu_dcn20_dwb_scl.c
SRCS+=	amdgpu_dcn20_hubbub.c
SRCS+=	amdgpu_dcn20_hubp.c
SRCS+=	amdgpu_dcn20_hwseq.c
SRCS+=	amdgpu_dcn20_init.c
SRCS+=	amdgpu_dcn20_link_encoder.c
SRCS+=	amdgpu_dcn20_mmhubbub.c
SRCS+=	amdgpu_dcn20_mpc.c
SRCS+=	amdgpu_dcn20_opp.c
SRCS+=	amdgpu_dcn20_optc.c
SRCS+=	amdgpu_dcn20_resource.c
SRCS+=	amdgpu_dcn20_stream_encoder.c
SRCS+=	amdgpu_dcn20_vmid.c
SRCS+=	amdgpu_dcn21_hubbub.c
SRCS+=	amdgpu_dcn21_hubp.c
SRCS+=	amdgpu_dcn21_hwseq.c
SRCS+=	amdgpu_dcn21_init.c
SRCS+=	amdgpu_dcn21_link_encoder.c
SRCS+=	amdgpu_dcn21_resource.c
SRCS+=	amdgpu_dcn_calc_auto.c
SRCS+=	amdgpu_dcn_calc_math.c
SRCS+=	amdgpu_dcn_calcs.c
SRCS+=	amdgpu_debugfs.c
SRCS+=	amdgpu_dev_coredump.c
SRCS+=	amdgpu_device.c
SRCS+=	amdgpu_df_v1_7.c
SRCS+=	amdgpu_df_v3_6.c
SRCS+=	amdgpu_df_v4_15.c
SRCS+=	amdgpu_df_v4_3.c
SRCS+=	amdgpu_df_v4_6_2.c
SRCS+=	amdgpu_discovery.c
SRCS+=	amdgpu_display.c
SRCS+=	amdgpu_display_mode_lib.c
SRCS+=	amdgpu_display_mode_vba.c
SRCS+=	amdgpu_display_mode_vba_20.c
SRCS+=	amdgpu_display_mode_vba_20v2.c
SRCS+=	amdgpu_display_mode_vba_21.c
SRCS+=	amdgpu_display_rq_dlg_calc_20.c
SRCS+=	amdgpu_display_rq_dlg_calc_20v2.c
SRCS+=	amdgpu_display_rq_dlg_calc_21.c
SRCS+=	amdgpu_display_rq_dlg_helpers.c
SRCS+=	amdgpu_dm.c
SRCS+=	amdgpu_dm_color.c
# HDCP skipped: CONFIG_DRM_AMD_DC_HDCP off (simpler Phoenix bring-up)
SRCS+=	amdgpu_dm_helpers.c
SRCS+=	amdgpu_dm_irq.c
SRCS+=	amdgpu_dm_mst_types.c
SRCS+=	amdgpu_dm_pp_smu.c
SRCS+=	amdgpu_dm_services.c
SRCS+=	amdgpu_dc_stubs.c
SRCS+=	amdgpu_dma_buf.c
SRCS+=	amdgpu_dml1_display_rq_dlg_calc.c
SRCS+=	amdgpu_dmub_dcn20.c
SRCS+=	amdgpu_dmub_dcn21.c
SRCS+=	amdgpu_dmub_reg.c
SRCS+=	amdgpu_dmub_srv.c
SRCS+=	amdgpu_doorbell_mgr.c
SRCS+=	amdgpu_dpm.c
SRCS+=	amdgpu_dpm_internal.c
SRCS+=	legacy_dpm.c
SRCS+=	amdgpu_drv.c
SRCS+=	amdgpu_eeprom.c
SRCS+=	amdgpu_emu_soc.c
SRCS+=	amdgpu_encoders.c
SRCS+=	amdgpu_fb.c
SRCS+=	amdgpu_fence.c
SRCS+=	fiji_baco.c
SRCS+=	fiji_smumgr.c
SRCS+=	amdgpu_fixpt31_32.c
SRCS+=	amdgpu_freesync.c
SRCS+=	amdgpu_fru_eeprom.c
SRCS+=	amdgpu_gart.c
SRCS+=	amdgpu_gem.c
SRCS+=	amdgpu_gfx.c
SRCS+=	amdgpu_gfx_v10_0.c
SRCS+=	amdgpu_gfx_v11_0.c
SRCS+=	amdgpu_gfx_v12_0.c
SRCS+=	amdgpu_gfx_v6_0.c
SRCS+=	amdgpu_gfx_v7_0.c
SRCS+=	amdgpu_gfx_v8_0.c
SRCS+=	amdgpu_gfx_v9_0.c
SRCS+=	amdgpu_gfx_v9_4.c
SRCS+=	amdgpu_gfx_v9_4_2.c
SRCS+=	amdgpu_gfx_v9_4_3.c
SRCS+=	amdgpu_gfxhub_v1_0.c
SRCS+=	amdgpu_gfxhub_v1_1.c
SRCS+=	amdgpu_gfxhub_v1_2.c
SRCS+=	amdgpu_gfxhub_v2_0.c
SRCS+=	amdgpu_gfxhub_v2_1.c
SRCS+=	amdgpu_gmc.c
SRCS+=	amdgpu_gmc_v10_0.c
SRCS+=	amdgpu_gmc_v11_0.c
SRCS+=	amdgpu_gmc_v12_0.c
SRCS+=	amdgpu_gmc_v6_0.c
SRCS+=	amdgpu_gmc_v7_0.c
SRCS+=	amdgpu_gmc_v8_0.c
SRCS+=	amdgpu_gmc_v9_0.c
SRCS+=	amdgpu_gpio_base.c
SRCS+=	amdgpu_gpio_service.c
SRCS+=	amdgpu_gtt_mgr.c
SRCS+=	hardwaremanager.c
# modules/hdcp skipped with CONFIG_DRM_AMD_DC_HDCP off
SRCS+=	amdgpu_hdp.c
SRCS+=	amdgpu_hdp_v4_0.c
SRCS+=	amdgpu_hdp_v5_0.c
SRCS+=	amdgpu_hdp_v5_2.c
SRCS+=	amdgpu_hdp_v6_0.c
SRCS+=	amdgpu_hdp_v7_0.c
SRCS+=	amdgpu_hw_ddc.c
SRCS+=	amdgpu_hw_factory.c
SRCS+=	amdgpu_hw_factory_dce110.c
SRCS+=	amdgpu_hw_factory_dce120.c
SRCS+=	amdgpu_hw_factory_dce80.c
SRCS+=	amdgpu_hw_factory_dcn10.c
SRCS+=	amdgpu_hw_factory_dcn20.c
SRCS+=	amdgpu_hw_factory_dcn21.c
SRCS+=	amdgpu_hw_factory_dcn30.c
SRCS+=	amdgpu_hw_generic.c
SRCS+=	amdgpu_hw_gpio.c
SRCS+=	amdgpu_hw_hpd.c
SRCS+=	amdgpu_hw_translate.c
SRCS+=	amdgpu_hw_translate_dce110.c
SRCS+=	amdgpu_hw_translate_dce120.c
SRCS+=	amdgpu_hw_translate_dce80.c
SRCS+=	amdgpu_hw_translate_dcn10.c
SRCS+=	amdgpu_hw_translate_dcn20.c
SRCS+=	amdgpu_hw_translate_dcn21.c
SRCS+=	amdgpu_hw_translate_dcn30.c
SRCS+=	hwmgr.c
SRCS+=	amdgpu_i2c.c
SRCS+=	amdgpu_ib.c
SRCS+=	amdgpu_iceland_ih.c
SRCS+=	iceland_smumgr.c
SRCS+=	amdgpu_ids.c
SRCS+=	amdgpu_ih.c
SRCS+=	amdgpu_ih_v6_0.c
SRCS+=	amdgpu_ih_v6_1.c
SRCS+=	amdgpu_ih_v7_0.c
SRCS+=	amdgpu_info_packet.c
SRCS+=	amdgpu_irq.c
SRCS+=	amdgpu_irq_service.c
SRCS+=	amdgpu_irq_service_dce110.c
SRCS+=	amdgpu_irq_service_dce120.c
SRCS+=	amdgpu_irq_service_dce80.c
SRCS+=	amdgpu_irq_service_dcn10.c
SRCS+=	amdgpu_irq_service_dcn20.c
SRCS+=	amdgpu_irq_service_dcn21.c
SRCS+=	amdgpu_job.c
SRCS+=	amdgpu_jpeg.c
SRCS+=	amdgpu_jpeg_v1_0.c
SRCS+=	amdgpu_jpeg_v2_0.c
SRCS+=	amdgpu_jpeg_v2_5.c
SRCS+=	amdgpu_jpeg_v3_0.c
SRCS+=	amdgpu_jpeg_v4_0.c
SRCS+=	amdgpu_jpeg_v4_0_3.c
SRCS+=	amdgpu_jpeg_v4_0_5.c
SRCS+=	amdgpu_jpeg_v5_0_0.c
SRCS+=	amdgpu_jpeg_v5_0_1.c
SRCS+=	amdgpu_kms.c
SRCS+=	amdgpu_kv_dpm.c
SRCS+=	amdgpu_kv_smc.c
SRCS+=	amdgpu_lsdma_v6_0.c
SRCS+=	amdgpu_lsdma_v7_0.c
SRCS+=	amdgpu_mca.c
SRCS+=	amdgpu_mca_v3_0.c
SRCS+=	amdgpu_mes.c
SRCS+=	amdgpu_mes_v10_1.c
SRCS+=	amdgpu_mes_v11_0.c
SRCS+=	amdgpu_mes_v12_0.c
SRCS+=	amdgpu_mmhub.c
SRCS+=	amdgpu_mmhub_v1_0.c
SRCS+=	amdgpu_mmhub_v1_7.c
SRCS+=	amdgpu_mmhub_v1_8.c
SRCS+=	amdgpu_mmhub_v2_0.c
SRCS+=	amdgpu_mmhub_v2_3.c
SRCS+=	amdgpu_mmhub_v9_4.c
SRCS+=	amdgpu_module.c
SRCS+=	amdgpu_mxgpu_ai.c
SRCS+=	amdgpu_mxgpu_nv.c
SRCS+=	amdgpu_mxgpu_vi.c
SRCS+=	amdgpu_navi10_ih.c
SRCS+=	navi10_ppt.c
SRCS+=	amdgpu_navi10_reg_init.c
SRCS+=	amdgpu_navi12_reg_init.c
SRCS+=	amdgpu_navi14_reg_init.c
SRCS+=	amdgpu_nbif_v6_3_1.c
SRCS+=	amdgpu_nbio.c
SRCS+=	amdgpu_nbio_v2_3.c
SRCS+=	amdgpu_nbio_v4_3.c
SRCS+=	amdgpu_nbio_v6_1.c
SRCS+=	amdgpu_nbio_v7_0.c
SRCS+=	amdgpu_nbio_v7_11.c
SRCS+=	amdgpu_nbio_v7_2.c
SRCS+=	amdgpu_nbio_v7_4.c
SRCS+=	amdgpu_nbio_v7_7.c
SRCS+=	amdgpu_nbio_v7_9.c
SRCS+=	amdgpu_nv.c
SRCS+=	amdgpu_object.c
SRCS+=	amdgpu_pci.c
SRCS+=	amdgpu_pll.c
SRCS+=	amdgpu_pm.c
SRCS+=	polaris10_smumgr.c
SRCS+=	polaris_baco.c
SRCS+=	amdgpu_power_helpers.c
SRCS+=	pp_overdriver.c
SRCS+=	pp_psm.c
SRCS+=	ppatomctrl.c
SRCS+=	ppatomfwctrl.c
SRCS+=	pppcielanes.c
SRCS+=	amdgpu_preempt_mgr.c
SRCS+=	process_pptables_v1_0.c
SRCS+=	processpptables.c
SRCS+=	amdgpu_psp.c
SRCS+=	amdgpu_psp_v10_0.c
SRCS+=	amdgpu_psp_v11_0.c
SRCS+=	amdgpu_psp_v11_0_8.c
SRCS+=	amdgpu_psp_v12_0.c
SRCS+=	amdgpu_psp_v13_0.c
SRCS+=	amdgpu_psp_v13_0_4.c
SRCS+=	amdgpu_psp_v14_0.c
SRCS+=	amdgpu_psp_v3_1.c
SRCS+=	amdgpu_ras.c
SRCS+=	amdgpu_ras_eeprom.c
SRCS+=	amdgpu_rc_calc.c
SRCS+=	amdgpu_rc_calc_dpi.c
SRCS+=	renoir_ppt.c
SRCS+=	amdgpu_reset.c
SRCS+=	amdgpu_ring.c
SRCS+=	amdgpu_ring_mux.c
SRCS+=	amdgpu_rlc.c
SRCS+=	amdgpu_rn_clk_mgr.c
SRCS+=	amdgpu_rn_clk_mgr_vbios_smu.c
SRCS+=	amdgpu_rv1_clk_mgr.c
SRCS+=	amdgpu_rv1_clk_mgr_vbios_smu.c
SRCS+=	amdgpu_rv2_clk_mgr.c
SRCS+=	amdgpu_sa.c
SRCS+=	amdgpu_sched.c
SRCS+=	amdgpu_sdma.c
SRCS+=	amdgpu_sdma_v2_4.c
SRCS+=	amdgpu_sdma_v3_0.c
SRCS+=	amdgpu_sdma_v4_0.c
SRCS+=	amdgpu_sdma_v4_4.c
SRCS+=	amdgpu_sdma_v4_4_2.c
SRCS+=	amdgpu_sdma_v5_0.c
SRCS+=	amdgpu_sdma_v5_2.c
SRCS+=	amdgpu_sdma_v6_0.c
SRCS+=	amdgpu_sdma_v7_0.c
SRCS+=	amdgpu_securedisplay.c
SRCS+=	amdgpu_seq64.c
SRCS+=	amdgpu_si.c
SRCS+=	amdgpu_si_dma.c
SRCS+=	amdgpu_si_dpm.c
SRCS+=	amdgpu_si_ih.c
SRCS+=	amdgpu_si_smc.c
SRCS+=	sienna_cichlid_ppt.c
SRCS+=	amdgpu_smu.c
SRCS+=	smu10_hwmgr.c
SRCS+=	smu10_smumgr.c
SRCS+=	smu7_baco.c
SRCS+=	smu7_clockpowergating.c
SRCS+=	smu7_hwmgr.c
SRCS+=	smu7_powertune.c
SRCS+=	smu7_smumgr.c
SRCS+=	smu7_thermal.c
SRCS+=	smu8_hwmgr.c
SRCS+=	smu8_smumgr.c
SRCS+=	smu9_baco.c
SRCS+=	smu9_smumgr.c
SRCS+=	smu_cmn.c
SRCS+=	smu_helper.c
SRCS+=	smu_v11_0.c
SRCS+=	amdgpu_smu_v11_0_i2c.c
SRCS+=	smu_v12_0.c
SRCS+=	smu_v13_0.c
SRCS+=	smu_v13_0_0_ppt.c
SRCS+=	smu_v13_0_4_ppt.c
SRCS+=	smu_v13_0_5_ppt.c
SRCS+=	smu_v13_0_6_ppt.c
SRCS+=	smu_v13_0_7_ppt.c
SRCS+=	smu_v14_0.c
SRCS+=	smu_v14_0_0_ppt.c
SRCS+=	smu_v14_0_2_ppt.c
SRCS+=	amdgpu_smuio_v11_0.c
SRCS+=	amdgpu_smuio_v11_0_6.c
SRCS+=	amdgpu_smuio_v13_0.c
SRCS+=	amdgpu_smuio_v13_0_3.c
SRCS+=	amdgpu_smuio_v13_0_6.c
SRCS+=	amdgpu_smuio_v14_0_2.c
SRCS+=	amdgpu_smuio_v9_0.c
SRCS+=	smumgr.c
SRCS+=	amdgpu_soc15.c
SRCS+=	amdgpu_soc21.c
SRCS+=	amdgpu_soc24.c
SRCS+=	amdgpu_sync.c
SRCS+=	amdgpu_test.c
SRCS+=	tonga_baco.c
SRCS+=	amdgpu_tonga_ih.c
SRCS+=	tonga_smumgr.c
SRCS+=	amdgpu_trace_points.c
SRCS+=	amdgpu_ttm.c
SRCS+=	amdgpu_ucode.c
SRCS+=	amdgpu_umc.c
SRCS+=	amdgpu_umc_v12_0.c
SRCS+=	amdgpu_umc_v6_0.c
SRCS+=	amdgpu_umc_v6_1.c
SRCS+=	amdgpu_umc_v6_7.c
SRCS+=	amdgpu_umc_v8_7.c
SRCS+=	amdgpu_umsch_mm.c
SRCS+=	amdgpu_uvd.c
SRCS+=	amdgpu_uvd_v3_1.c
SRCS+=	amdgpu_uvd_v4_2.c
SRCS+=	amdgpu_uvd_v5_0.c
SRCS+=	amdgpu_uvd_v6_0.c
SRCS+=	amdgpu_uvd_v7_0.c
SRCS+=	vangogh_ppt.c
SRCS+=	amdgpu_vce.c
SRCS+=	amdgpu_vce_v2_0.c
SRCS+=	amdgpu_vce_v3_0.c
SRCS+=	amdgpu_vce_v4_0.c
SRCS+=	amdgpu_vcn.c
SRCS+=	amdgpu_vcn_v1_0.c
SRCS+=	amdgpu_vcn_v2_0.c
SRCS+=	amdgpu_vcn_v2_5.c
SRCS+=	amdgpu_vcn_v3_0.c
SRCS+=	amdgpu_vcn_v4_0.c
SRCS+=	amdgpu_vcn_v4_0_3.c
SRCS+=	amdgpu_vcn_v4_0_5.c
SRCS+=	amdgpu_vcn_v5_0_0.c
SRCS+=	amdgpu_vcn_v5_0_1.c
SRCS+=	amdgpu_vector.c
SRCS+=	vega10_baco.c
SRCS+=	vega10_hwmgr.c
SRCS+=	amdgpu_vega10_ih.c
SRCS+=	vega10_powertune.c
SRCS+=	vega10_processpptables.c
SRCS+=	amdgpu_vega10_reg_init.c
SRCS+=	vega10_smumgr.c
SRCS+=	vega10_thermal.c
SRCS+=	vega12_baco.c
SRCS+=	vega12_hwmgr.c
SRCS+=	vega12_processpptables.c
SRCS+=	vega12_smumgr.c
SRCS+=	vega12_thermal.c
SRCS+=	vega20_baco.c
SRCS+=	vega20_hwmgr.c
SRCS+=	amdgpu_vega20_ih.c
SRCS+=	vega20_powertune.c
SRCS+=	vega20_processpptables.c
SRCS+=	amdgpu_vega20_reg_init.c
SRCS+=	vega20_smumgr.c
SRCS+=	vega20_thermal.c
SRCS+=	vegam_smumgr.c
SRCS+=	amdgpu_vf_error.c
SRCS+=	amdgpu_vi.c
SRCS+=	amdgpu_virt.c
SRCS+=	amdgpu_virtual_link_encoder.c
SRCS+=	amdgpu_virtual_stream_encoder.c
SRCS+=	amdgpu_vkms.c
SRCS+=	amdgpu_vm.c
SRCS+=	amdgpu_vm_cpu.c
SRCS+=	amdgpu_vm_pt.c
SRCS+=	amdgpu_vm_sdma.c
SRCS+=	amdgpu_vm_tlb_fence.c
SRCS+=	amdgpu_vpe.c
SRCS+=	amdgpu_vram_mgr.c
SRCS+=	amdgpu_xcp.c
SRCS+=	amdgpu_xgmi.c
SRCS+=	yellow_carp_ppt.c
SRCS+=	amdgpu_aldebaran.c
SRCS+=	amdgpu_athub_v3_0.c
SRCS+=	amdgpu_athub_v4_1_0.c
SRCS+=	amdgpu_gfx_v11_0_3.c
SRCS+=	amdgpu_gfxhub_v3_0.c
SRCS+=	amdgpu_gfxhub_v3_0_3.c
SRCS+=	amdgpu_gfxhub_v11_5_0.c
SRCS+=	amdgpu_gfxhub_v12_0.c
SRCS+=	amdgpu_imu_v11_0.c
SRCS+=	amdgpu_imu_v12_0.c
SRCS+=	amdgpu_lsdma.c
SRCS+=	amdgpu_mmhub_v3_0.c
SRCS+=	amdgpu_mmhub_v3_0_1.c
SRCS+=	amdgpu_mmhub_v3_0_2.c
SRCS+=	amdgpu_mmhub_v3_3.c
SRCS+=	amdgpu_mmhub_v4_1_0.c
SRCS+=	amdgpu_sienna_cichlid.c
SRCS+=	amdgpu_smu_v13_0_10.c
SRCS+=	amdgpu_umc_v8_10.c
SRCS+=	amdgpu_umc_v8_14.c
SRCS+=	amdgpu_umsch_mm_v4_0.c
SRCS+=	amdgpu_vpe_v6_1.c
SRCS+=	amdgpu_imu_v11_0_3.c
SRCS+=	amdgpufb.c

# format: Linux uses %llu with uint64_t (unsigned long on LP64 NetBSD)
CWARNFLAGS.amdgpu_dce110_hwseq.c+=	-Wno-format
CWARNFLAGS.amdgpu_dce100_hwseq.c+=	-Wno-format
CWARNFLAGS.amdgpu_dce80_hwseq.c+=	-Wno-format
CWARNFLAGS.amdgpu_dce112_hwseq.c+=	-Wno-format
CWARNFLAGS.amdgpu_dce120_hwseq.c+=	-Wno-format
CWARNFLAGS.amdgpu_dcn10_hwseq.c+=	-Wno-format
CWARNFLAGS.amdgpu_dcn20_hwseq.c+=	-Wno-format
CWARNFLAGS.amdgpu_dcn21_hwseq.c+=	-Wno-format
CWARNFLAGS.amdgpu_dcn30_hwseq.c+=	-Wno-format
CWARNFLAGS.amdgpu_dcn31_hwseq.c+=	-Wno-format
CWARNFLAGS.amdgpu_dcn314_hwseq.c+=	-Wno-format

# --- DCN1x/2x split-layout paths (Linux 6.14) ---
.PATH:	${S}/external/bsd/drm2/dist/drm/amd/amdgpu/../display/dc/dccg/dcn20
.PATH:	${S}/external/bsd/drm2/dist/drm/amd/amdgpu/../display/dc/dccg/dcn201
.PATH:	${S}/external/bsd/drm2/dist/drm/amd/amdgpu/../display/dc/dccg/dcn21
.PATH:	${S}/external/bsd/drm2/dist/drm/amd/amdgpu/../display/dc/dpp/dcn10
.PATH:	${S}/external/bsd/drm2/dist/drm/amd/amdgpu/../display/dc/dpp/dcn20
.PATH:	${S}/external/bsd/drm2/dist/drm/amd/amdgpu/../display/dc/dsc/dcn20
.PATH:	${S}/external/bsd/drm2/dist/drm/amd/amdgpu/../display/dc/hubbub/dcn10
.PATH:	${S}/external/bsd/drm2/dist/drm/amd/amdgpu/../display/dc/hubbub/dcn20
.PATH:	${S}/external/bsd/drm2/dist/drm/amd/amdgpu/../display/dc/hubbub/dcn21
.PATH:	${S}/external/bsd/drm2/dist/drm/amd/amdgpu/../display/dc/hubp/dcn10
.PATH:	${S}/external/bsd/drm2/dist/drm/amd/amdgpu/../display/dc/hubp/dcn20
.PATH:	${S}/external/bsd/drm2/dist/drm/amd/amdgpu/../display/dc/hubp/dcn21
.PATH:	${S}/external/bsd/drm2/dist/drm/amd/amdgpu/../display/dc/hwss/dcn10
.PATH:	${S}/external/bsd/drm2/dist/drm/amd/amdgpu/../display/dc/hwss/dcn20
.PATH:	${S}/external/bsd/drm2/dist/drm/amd/amdgpu/../display/dc/hwss/dcn21
.PATH:	${S}/external/bsd/drm2/dist/drm/amd/amdgpu/../display/dc/dio/dcn10
.PATH:	${S}/external/bsd/drm2/dist/drm/amd/amdgpu/../display/dc/dio/dcn20
.PATH:	${S}/external/bsd/drm2/dist/drm/amd/amdgpu/../display/dc/mmhubbub/dcn20
.PATH:	${S}/external/bsd/drm2/dist/drm/amd/amdgpu/../display/dc/mpc/dcn10
.PATH:	${S}/external/bsd/drm2/dist/drm/amd/amdgpu/../display/dc/mpc/dcn20
.PATH:	${S}/external/bsd/drm2/dist/drm/amd/amdgpu/../display/dc/opp/dcn10
.PATH:	${S}/external/bsd/drm2/dist/drm/amd/amdgpu/../display/dc/opp/dcn20
.PATH:	${S}/external/bsd/drm2/dist/drm/amd/amdgpu/../display/dc/optc/dcn10
.PATH:	${S}/external/bsd/drm2/dist/drm/amd/amdgpu/../display/dc/optc/dcn20
.PATH:	${S}/external/bsd/drm2/dist/drm/amd/amdgpu/../display/dc/resource/dcn10
.PATH:	${S}/external/bsd/drm2/dist/drm/amd/amdgpu/../display/dc/resource/dcn20
.PATH:	${S}/external/bsd/drm2/dist/drm/amd/amdgpu/../display/dc/resource/dcn21
.PATH:	${S}/external/bsd/drm2/dist/drm/amd/amdgpu/../display/dc/dwb/dcn20
.PATH:	${S}/external/bsd/drm2/dist/drm/amd/amdgpu/../display/dc/dwb/dcn30

# --- Phoenix / DCN 3.1.4 (synced from Linux 6.14) ---
CPPFLAGS+=	-I${S}/external/bsd/drm2/dist/drm/amd/display/dc/resource
CPPFLAGS+=	-I${S}/external/bsd/drm2/dist/drm/amd/display/dc/hwss
CPPFLAGS+=	-I${S}/external/bsd/drm2/dist/drm/amd/display/dc/dsc
CPPFLAGS+=	-I${S}/external/bsd/drm2/dist/drm/amd/display/dc/optc
CPPFLAGS+=	-I${S}/external/bsd/drm2/dist/drm/amd/display/dc/dpp
CPPFLAGS+=	-I${S}/external/bsd/drm2/dist/drm/amd/display/dc/hubbub
CPPFLAGS+=	-I${S}/external/bsd/drm2/dist/drm/amd/display/dc/dccg
CPPFLAGS+=	-I${S}/external/bsd/drm2/dist/drm/amd/display/dc/hubp
CPPFLAGS+=	-I${S}/external/bsd/drm2/dist/drm/amd/display/dc/dio
CPPFLAGS+=	-I${S}/external/bsd/drm2/dist/drm/amd/display/dc/dwb
CPPFLAGS+=	-I${S}/external/bsd/drm2/dist/drm/amd/display/dc/hpo
CPPFLAGS+=	-I${S}/external/bsd/drm2/dist/drm/amd/display/dc/mmhubbub
CPPFLAGS+=	-I${S}/external/bsd/drm2/dist/drm/amd/display/dc/mpc
CPPFLAGS+=	-I${S}/external/bsd/drm2/dist/drm/amd/display/dc/opp
CPPFLAGS+=	-I${S}/external/bsd/drm2/dist/drm/amd/display/dc/pg
CPPFLAGS+=	-I${S}/external/bsd/drm2/dist/drm/amd/display/dc/irq
CPPFLAGS+=	-I${S}/external/bsd/drm2/dist/drm/amd/display/dc/dml
CPPFLAGS+=	-I${S}/external/bsd/drm2/dist/drm/amd/display/dc/link
CPPFLAGS+=	-I${S}/external/bsd/drm2/dist/drm/amd/display/dc/gpio
CPPFLAGS+=	-I${S}/external/bsd/drm2/dist/drm/amd/display

.PATH:	${S}/external/bsd/drm2/dist/drm/amd/amdgpu/../display/dc/clk_mgr/dcn30
.PATH:	${S}/external/bsd/drm2/dist/drm/amd/amdgpu/../display/dc/clk_mgr/dcn31
.PATH:	${S}/external/bsd/drm2/dist/drm/amd/amdgpu/../display/dc/clk_mgr/dcn314
.PATH:	${S}/external/bsd/drm2/dist/drm/amd/amdgpu/../display/dc/dccg/dcn30
.PATH:	${S}/external/bsd/drm2/dist/drm/amd/amdgpu/../display/dc/dccg/dcn31
.PATH:	${S}/external/bsd/drm2/dist/drm/amd/amdgpu/../display/dc/dccg/dcn314
.PATH:	${S}/external/bsd/drm2/dist/drm/amd/amdgpu/../display/dc/dcn30
.PATH:	${S}/external/bsd/drm2/dist/drm/amd/amdgpu/../display/dc/dcn31
.PATH:	${S}/external/bsd/drm2/dist/drm/amd/amdgpu/../display/dc/dio/dcn30
.PATH:	${S}/external/bsd/drm2/dist/drm/amd/amdgpu/../display/dc/dio/dcn31
.PATH:	${S}/external/bsd/drm2/dist/drm/amd/amdgpu/../display/dc/dio/dcn314
.PATH:	${S}/external/bsd/drm2/dist/drm/amd/amdgpu/../display/dc/dml/dcn30
.PATH:	${S}/external/bsd/drm2/dist/drm/amd/amdgpu/../display/dc/dml/dcn31
.PATH:	${S}/external/bsd/drm2/dist/drm/amd/amdgpu/../display/dc/dml/dcn314
.PATH:	${S}/external/bsd/drm2/dist/drm/amd/amdgpu/../display/dc/dpp/dcn30
.PATH:	${S}/external/bsd/drm2/dist/drm/amd/amdgpu/../display/dc/dwb/dcn30
.PATH:	${S}/external/bsd/drm2/dist/drm/amd/amdgpu/../display/dc/hpo/dcn31
.PATH:	${S}/external/bsd/drm2/dist/drm/amd/amdgpu/../display/dc/hubbub/dcn30
.PATH:	${S}/external/bsd/drm2/dist/drm/amd/amdgpu/../display/dc/hubbub/dcn31
.PATH:	${S}/external/bsd/drm2/dist/drm/amd/amdgpu/../display/dc/hubp/dcn30
.PATH:	${S}/external/bsd/drm2/dist/drm/amd/amdgpu/../display/dc/hubp/dcn31
.PATH:	${S}/external/bsd/drm2/dist/drm/amd/amdgpu/../display/dc/hwss/dcn30
.PATH:	${S}/external/bsd/drm2/dist/drm/amd/amdgpu/../display/dc/hwss/dcn31
.PATH:	${S}/external/bsd/drm2/dist/drm/amd/amdgpu/../display/dc/hwss/dcn314
.PATH:	${S}/external/bsd/drm2/dist/drm/amd/amdgpu/../display/dc/irq/dcn30
.PATH:	${S}/external/bsd/drm2/dist/drm/amd/amdgpu/../display/dc/irq/dcn31
.PATH:	${S}/external/bsd/drm2/dist/drm/amd/amdgpu/../display/dc/irq/dcn314
.PATH:	${S}/external/bsd/drm2/dist/drm/amd/amdgpu/../display/dc/mpc/dcn30
.PATH:	${S}/external/bsd/drm2/dist/drm/amd/amdgpu/../display/dc/optc/dcn30
.PATH:	${S}/external/bsd/drm2/dist/drm/amd/amdgpu/../display/dc/optc/dcn31
.PATH:	${S}/external/bsd/drm2/dist/drm/amd/amdgpu/../display/dc/optc/dcn314
.PATH:	${S}/external/bsd/drm2/dist/drm/amd/amdgpu/../display/dc/resource/dcn30
.PATH:	${S}/external/bsd/drm2/dist/drm/amd/amdgpu/../display/dc/resource/dcn31
.PATH:	${S}/external/bsd/drm2/dist/drm/amd/amdgpu/../display/dc/resource/dcn314

COPTS.amdgpu_dcn30_resource.c+=	${COPTS.amdgpu_float}
COPTS.amdgpu_dcn31_resource.c+=	${COPTS.amdgpu_float}
COPTS.amdgpu_dcn314_resource.c+=	${COPTS.amdgpu_float}
COPTS.amdgpu_dcn30_fpu.c+=	${COPTS.amdgpu_float}
COPTS.amdgpu_dcn31_fpu.c+=	${COPTS.amdgpu_float}
COPTS.amdgpu_dcn314_fpu.c+=	${COPTS.amdgpu_float}
COPTS.amdgpu_dc_fpu.c+=	${COPTS.amdgpu_float}
COPTS.amdgpu_dcn10_fpu.c+=	${COPTS.amdgpu_float}
COPTS.amdgpu_dcn20_fpu.c+=	${COPTS.amdgpu_float}
COPTS.amdgpu_dcn301_fpu.c+=	${COPTS.amdgpu_float}
COPTS.amdgpu_dcn302_fpu.c+=	${COPTS.amdgpu_float}
COPTS.amdgpu_dcn303_fpu.c+=	${COPTS.amdgpu_float}
COPTS.amdgpu_dcn321_fpu.c+=	${COPTS.amdgpu_float}
COPTS.amdgpu_dcn32_fpu.c+=	${COPTS.amdgpu_float}
COPTS.amdgpu_dcn351_fpu.c+=	${COPTS.amdgpu_float}
COPTS.amdgpu_dcn35_fpu.c+=	${COPTS.amdgpu_float}
COPTS.amdgpu_dcn401_fpu.c+=	${COPTS.amdgpu_float}
COPTS.amdgpu_rc_calc_fpu.c+=	${COPTS.amdgpu_float}
COPTS.amdgpu_display_mode_vba_30.c+=	${COPTS.amdgpu_float}
COPTS.amdgpu_display_mode_vba_31.c+=	${COPTS.amdgpu_float}
COPTS.amdgpu_display_mode_vba_314.c+=	${COPTS.amdgpu_float}
COPTS.amdgpu_display_rq_dlg_calc_30.c+=	${COPTS.amdgpu_float}
COPTS.amdgpu_display_rq_dlg_calc_31.c+=	${COPTS.amdgpu_float}
COPTS.amdgpu_display_rq_dlg_calc_314.c+=	${COPTS.amdgpu_float}

SRCS+=	amdgpu_dc_fpu.c
SRCS+=	amdgpu_dcn30_afmt.c
SRCS+=	amdgpu_dcn30_clk_mgr.c
SRCS+=	amdgpu_dcn30_clk_mgr_smu_msg.c
SRCS+=	amdgpu_dcn30_cm_common.c
SRCS+=	amdgpu_dcn30_dccg.c
SRCS+=	amdgpu_dcn30_dio_link_encoder.c
SRCS+=	amdgpu_dcn30_dio_stream_encoder.c
SRCS+=	amdgpu_dcn30_dpp.c
SRCS+=	amdgpu_dcn30_dpp_cm.c
SRCS+=	amdgpu_dcn30_dwb.c
SRCS+=	amdgpu_dcn30_dwb_cm.c
SRCS+=	amdgpu_dcn30_fpu.c
SRCS+=	amdgpu_dcn30_hubbub.c
SRCS+=	amdgpu_dcn30_hubp.c
SRCS+=	amdgpu_dcn30_hwseq.c
SRCS+=	amdgpu_dcn30_init.c
SRCS+=	amdgpu_dcn30_mmhubbub.c
SRCS+=	amdgpu_dcn30_mpc.c
SRCS+=	amdgpu_dcn30_optc.c
SRCS+=	amdgpu_dcn30_resource.c
SRCS+=	amdgpu_dcn30_vpg.c
SRCS+=	amdgpu_dcn314_clk_mgr.c
SRCS+=	amdgpu_dcn314_dccg.c
SRCS+=	amdgpu_dcn314_dio_stream_encoder.c
SRCS+=	amdgpu_dcn314_fpu.c
SRCS+=	amdgpu_dcn314_hwseq.c
SRCS+=	amdgpu_dcn314_init.c
SRCS+=	amdgpu_dcn314_optc.c
SRCS+=	amdgpu_dcn314_resource.c
SRCS+=	amdgpu_dcn314_smu.c
SRCS+=	amdgpu_dcn31_afmt.c
SRCS+=	amdgpu_dcn31_apg.c
SRCS+=	amdgpu_dcn31_clk_mgr.c
SRCS+=	amdgpu_dcn31_dccg.c
SRCS+=	amdgpu_dcn31_dio_link_encoder.c
SRCS+=	amdgpu_dcn31_fpu.c
SRCS+=	amdgpu_dcn31_hpo_dp_link_encoder.c
SRCS+=	amdgpu_dcn31_hpo_dp_stream_encoder.c
SRCS+=	amdgpu_dcn31_hubbub.c
SRCS+=	amdgpu_dcn31_hubp.c
SRCS+=	amdgpu_dcn31_hwseq.c
SRCS+=	amdgpu_dcn31_init.c
SRCS+=	amdgpu_dcn31_optc.c
SRCS+=	amdgpu_dcn31_panel_cntl.c
SRCS+=	amdgpu_dcn31_resource.c
SRCS+=	amdgpu_dcn31_smu.c
SRCS+=	amdgpu_dcn31_vpg.c
SRCS+=	amdgpu_display_mode_vba_30.c
SRCS+=	amdgpu_display_mode_vba_31.c
SRCS+=	amdgpu_display_mode_vba_314.c
SRCS+=	amdgpu_display_rq_dlg_calc_30.c
SRCS+=	amdgpu_display_rq_dlg_calc_31.c
SRCS+=	amdgpu_display_rq_dlg_calc_314.c
SRCS+=	amdgpu_irq_service_dcn30.c
SRCS+=	amdgpu_irq_service_dcn31.c
SRCS+=	amdgpu_irq_service_dcn314.c
SRCS+=	amdgpu_dmub_dcn30.c
SRCS+=	amdgpu_dmub_dcn31.c
SRCS+=	amdgpu_dmub_dcn314.c
SRCS+=	amdgpu_dmub_dcn32.c
SRCS+=	amdgpu_dmub_dcn35.c
SRCS+=	amdgpu_dmub_dcn351.c
SRCS+=	amdgpu_dmub_dcn401.c
# --- end DCN 3.1.4 ---
# --- display sync from files.amdgpu ---
.PATH:	${S}/external/bsd/drm2/dist/drm/amd/amdgpu/../display/amdgpu_dm
.PATH:	${S}/external/bsd/drm2/dist/drm/amd/amdgpu/../display/dc
.PATH:	${S}/external/bsd/drm2/dist/drm/amd/amdgpu/../display/dc/basics
.PATH:	${S}/external/bsd/drm2/dist/drm/amd/amdgpu/../display/dc/bios
.PATH:	${S}/external/bsd/drm2/dist/drm/amd/amdgpu/../display/dc/bios/dce110
.PATH:	${S}/external/bsd/drm2/dist/drm/amd/amdgpu/../display/dc/bios/dce112
.PATH:	${S}/external/bsd/drm2/dist/drm/amd/amdgpu/../display/dc/bios/dce60
.PATH:	${S}/external/bsd/drm2/dist/drm/amd/amdgpu/../display/dc/bios/dce80
.PATH:	${S}/external/bsd/drm2/dist/drm/amd/amdgpu/../display/dc/clk_mgr
.PATH:	${S}/external/bsd/drm2/dist/drm/amd/amdgpu/../display/dc/clk_mgr/dce100
.PATH:	${S}/external/bsd/drm2/dist/drm/amd/amdgpu/../display/dc/clk_mgr/dce110
.PATH:	${S}/external/bsd/drm2/dist/drm/amd/amdgpu/../display/dc/clk_mgr/dce112
.PATH:	${S}/external/bsd/drm2/dist/drm/amd/amdgpu/../display/dc/clk_mgr/dce120
.PATH:	${S}/external/bsd/drm2/dist/drm/amd/amdgpu/../display/dc/clk_mgr/dce60
.PATH:	${S}/external/bsd/drm2/dist/drm/amd/amdgpu/../display/dc/clk_mgr/dcn10
.PATH:	${S}/external/bsd/drm2/dist/drm/amd/amdgpu/../display/dc/clk_mgr/dcn20
.PATH:	${S}/external/bsd/drm2/dist/drm/amd/amdgpu/../display/dc/clk_mgr/dcn201
.PATH:	${S}/external/bsd/drm2/dist/drm/amd/amdgpu/../display/dc/clk_mgr/dcn21
.PATH:	${S}/external/bsd/drm2/dist/drm/amd/amdgpu/../display/dc/clk_mgr/dcn30
.PATH:	${S}/external/bsd/drm2/dist/drm/amd/amdgpu/../display/dc/clk_mgr/dcn301
.PATH:	${S}/external/bsd/drm2/dist/drm/amd/amdgpu/../display/dc/clk_mgr/dcn31
.PATH:	${S}/external/bsd/drm2/dist/drm/amd/amdgpu/../display/dc/clk_mgr/dcn314
.PATH:	${S}/external/bsd/drm2/dist/drm/amd/amdgpu/../display/dc/clk_mgr/dcn315
.PATH:	${S}/external/bsd/drm2/dist/drm/amd/amdgpu/../display/dc/clk_mgr/dcn316
.PATH:	${S}/external/bsd/drm2/dist/drm/amd/amdgpu/../display/dc/clk_mgr/dcn32
.PATH:	${S}/external/bsd/drm2/dist/drm/amd/amdgpu/../display/dc/clk_mgr/dcn35
.PATH:	${S}/external/bsd/drm2/dist/drm/amd/amdgpu/../display/dc/clk_mgr/dcn401
.PATH:	${S}/external/bsd/drm2/dist/drm/amd/amdgpu/../display/dc/core
.PATH:	${S}/external/bsd/drm2/dist/drm/amd/amdgpu/../display/dc/dccg/dcn20
.PATH:	${S}/external/bsd/drm2/dist/drm/amd/amdgpu/../display/dc/dccg/dcn201
.PATH:	${S}/external/bsd/drm2/dist/drm/amd/amdgpu/../display/dc/dccg/dcn21
.PATH:	${S}/external/bsd/drm2/dist/drm/amd/amdgpu/../display/dc/dccg/dcn30
.PATH:	${S}/external/bsd/drm2/dist/drm/amd/amdgpu/../display/dc/dccg/dcn301
.PATH:	${S}/external/bsd/drm2/dist/drm/amd/amdgpu/../display/dc/dccg/dcn31
.PATH:	${S}/external/bsd/drm2/dist/drm/amd/amdgpu/../display/dc/dccg/dcn314
.PATH:	${S}/external/bsd/drm2/dist/drm/amd/amdgpu/../display/dc/dccg/dcn32
.PATH:	${S}/external/bsd/drm2/dist/drm/amd/amdgpu/../display/dc/dccg/dcn35
.PATH:	${S}/external/bsd/drm2/dist/drm/amd/amdgpu/../display/dc/dccg/dcn401
.PATH:	${S}/external/bsd/drm2/dist/drm/amd/amdgpu/../display/dc/dce
.PATH:	${S}/external/bsd/drm2/dist/drm/amd/amdgpu/../display/dc/dce110
.PATH:	${S}/external/bsd/drm2/dist/drm/amd/amdgpu/../display/dc/dce112
.PATH:	${S}/external/bsd/drm2/dist/drm/amd/amdgpu/../display/dc/dce120
.PATH:	${S}/external/bsd/drm2/dist/drm/amd/amdgpu/../display/dc/dce60
.PATH:	${S}/external/bsd/drm2/dist/drm/amd/amdgpu/../display/dc/dce80
.PATH:	${S}/external/bsd/drm2/dist/drm/amd/amdgpu/../display/dc/dcn10
.PATH:	${S}/external/bsd/drm2/dist/drm/amd/amdgpu/../display/dc/dcn20
.PATH:	${S}/external/bsd/drm2/dist/drm/amd/amdgpu/../display/dc/dcn201
.PATH:	${S}/external/bsd/drm2/dist/drm/amd/amdgpu/../display/dc/dcn21
.PATH:	${S}/external/bsd/drm2/dist/drm/amd/amdgpu/../display/dc/dcn30
.PATH:	${S}/external/bsd/drm2/dist/drm/amd/amdgpu/../display/dc/dcn301
.PATH:	${S}/external/bsd/drm2/dist/drm/amd/amdgpu/../display/dc/dcn31
.PATH:	${S}/external/bsd/drm2/dist/drm/amd/amdgpu/../display/dc/dio/dcn10
.PATH:	${S}/external/bsd/drm2/dist/drm/amd/amdgpu/../display/dc/dio/dcn20
.PATH:	${S}/external/bsd/drm2/dist/drm/amd/amdgpu/../display/dc/dio/dcn30
.PATH:	${S}/external/bsd/drm2/dist/drm/amd/amdgpu/../display/dc/dio/dcn301
.PATH:	${S}/external/bsd/drm2/dist/drm/amd/amdgpu/../display/dc/dio/dcn31
.PATH:	${S}/external/bsd/drm2/dist/drm/amd/amdgpu/../display/dc/dio/dcn314
.PATH:	${S}/external/bsd/drm2/dist/drm/amd/amdgpu/../display/dc/dio/dcn32
.PATH:	${S}/external/bsd/drm2/dist/drm/amd/amdgpu/../display/dc/dio/dcn321
.PATH:	${S}/external/bsd/drm2/dist/drm/amd/amdgpu/../display/dc/dio/dcn35
.PATH:	${S}/external/bsd/drm2/dist/drm/amd/amdgpu/../display/dc/dio/dcn401
.PATH:	${S}/external/bsd/drm2/dist/drm/amd/amdgpu/../display/dc/dml
.PATH:	${S}/external/bsd/drm2/dist/drm/amd/amdgpu/../display/dc/dml/calcs
.PATH:	${S}/external/bsd/drm2/dist/drm/amd/amdgpu/../display/dc/dml/dcn10
.PATH:	${S}/external/bsd/drm2/dist/drm/amd/amdgpu/../display/dc/dml/dcn20
.PATH:	${S}/external/bsd/drm2/dist/drm/amd/amdgpu/../display/dc/dml/dcn21
.PATH:	${S}/external/bsd/drm2/dist/drm/amd/amdgpu/../display/dc/dml/dcn30
.PATH:	${S}/external/bsd/drm2/dist/drm/amd/amdgpu/../display/dc/dml/dcn301
.PATH:	${S}/external/bsd/drm2/dist/drm/amd/amdgpu/../display/dc/dml/dcn302
.PATH:	${S}/external/bsd/drm2/dist/drm/amd/amdgpu/../display/dc/dml/dcn303
.PATH:	${S}/external/bsd/drm2/dist/drm/amd/amdgpu/../display/dc/dml/dcn31
.PATH:	${S}/external/bsd/drm2/dist/drm/amd/amdgpu/../display/dc/dml/dcn314
.PATH:	${S}/external/bsd/drm2/dist/drm/amd/amdgpu/../display/dc/dml/dcn32
.PATH:	${S}/external/bsd/drm2/dist/drm/amd/amdgpu/../display/dc/dml/dcn321
.PATH:	${S}/external/bsd/drm2/dist/drm/amd/amdgpu/../display/dc/dml/dcn35
.PATH:	${S}/external/bsd/drm2/dist/drm/amd/amdgpu/../display/dc/dml/dcn351
.PATH:	${S}/external/bsd/drm2/dist/drm/amd/amdgpu/../display/dc/dml/dcn401
.PATH:	${S}/external/bsd/drm2/dist/drm/amd/amdgpu/../display/dc/dml/dsc
.PATH:	${S}/external/bsd/drm2/dist/drm/amd/amdgpu/../display/dc/dml2
.PATH:	${S}/external/bsd/drm2/dist/drm/amd/amdgpu/../display/dc/dml2/dml21
.PATH:	${S}/external/bsd/drm2/dist/drm/amd/amdgpu/../display/dc/dml2/dml21/src/dml2_core
.PATH:	${S}/external/bsd/drm2/dist/drm/amd/amdgpu/../display/dc/dml2/dml21/src/dml2_dpmm
.PATH:	${S}/external/bsd/drm2/dist/drm/amd/amdgpu/../display/dc/dml2/dml21/src/dml2_mcg
.PATH:	${S}/external/bsd/drm2/dist/drm/amd/amdgpu/../display/dc/dml2/dml21/src/dml2_pmo
.PATH:	${S}/external/bsd/drm2/dist/drm/amd/amdgpu/../display/dc/dml2/dml21/src/dml2_standalone_libraries
.PATH:	${S}/external/bsd/drm2/dist/drm/amd/amdgpu/../display/dc/dml2/dml21/src/dml2_top
.PATH:	${S}/external/bsd/drm2/dist/drm/amd/amdgpu/../display/dc/dml2/dml21/src/inc
.PATH:	${S}/external/bsd/drm2/dist/drm/amd/amdgpu/../display/dc/dpp/dcn10
.PATH:	${S}/external/bsd/drm2/dist/drm/amd/amdgpu/../display/dc/dpp/dcn20
.PATH:	${S}/external/bsd/drm2/dist/drm/amd/amdgpu/../display/dc/dpp/dcn201
.PATH:	${S}/external/bsd/drm2/dist/drm/amd/amdgpu/../display/dc/dpp/dcn30
.PATH:	${S}/external/bsd/drm2/dist/drm/amd/amdgpu/../display/dc/dpp/dcn32
.PATH:	${S}/external/bsd/drm2/dist/drm/amd/amdgpu/../display/dc/dpp/dcn35
.PATH:	${S}/external/bsd/drm2/dist/drm/amd/amdgpu/../display/dc/dpp/dcn401
.PATH:	${S}/external/bsd/drm2/dist/drm/amd/amdgpu/../display/dc/dsc
.PATH:	${S}/external/bsd/drm2/dist/drm/amd/amdgpu/../display/dc/dsc/dcn20
.PATH:	${S}/external/bsd/drm2/dist/drm/amd/amdgpu/../display/dc/dsc/dcn35
.PATH:	${S}/external/bsd/drm2/dist/drm/amd/amdgpu/../display/dc/dsc/dcn401
.PATH:	${S}/external/bsd/drm2/dist/drm/amd/amdgpu/../display/dc/dwb/dcn30
.PATH:	${S}/external/bsd/drm2/dist/drm/amd/amdgpu/../display/dc/dwb/dcn35
.PATH:	${S}/external/bsd/drm2/dist/drm/amd/amdgpu/../display/dc/gpio
.PATH:	${S}/external/bsd/drm2/dist/drm/amd/amdgpu/../display/dc/gpio/dce110
.PATH:	${S}/external/bsd/drm2/dist/drm/amd/amdgpu/../display/dc/gpio/dce120
.PATH:	${S}/external/bsd/drm2/dist/drm/amd/amdgpu/../display/dc/gpio/dce60
.PATH:	${S}/external/bsd/drm2/dist/drm/amd/amdgpu/../display/dc/gpio/dce80
.PATH:	${S}/external/bsd/drm2/dist/drm/amd/amdgpu/../display/dc/gpio/dcn10
.PATH:	${S}/external/bsd/drm2/dist/drm/amd/amdgpu/../display/dc/gpio/dcn20
.PATH:	${S}/external/bsd/drm2/dist/drm/amd/amdgpu/../display/dc/gpio/dcn21
.PATH:	${S}/external/bsd/drm2/dist/drm/amd/amdgpu/../display/dc/gpio/dcn30
.PATH:	${S}/external/bsd/drm2/dist/drm/amd/amdgpu/../display/dc/gpio/dcn315
.PATH:	${S}/external/bsd/drm2/dist/drm/amd/amdgpu/../display/dc/gpio/dcn32
.PATH:	${S}/external/bsd/drm2/dist/drm/amd/amdgpu/../display/dc/gpio/dcn401
.PATH:	${S}/external/bsd/drm2/dist/drm/amd/amdgpu/../display/dc/hdcp
.PATH:	${S}/external/bsd/drm2/dist/drm/amd/amdgpu/../display/dc/hpo/dcn31
.PATH:	${S}/external/bsd/drm2/dist/drm/amd/amdgpu/../display/dc/hpo/dcn32
.PATH:	${S}/external/bsd/drm2/dist/drm/amd/amdgpu/../display/dc/hubbub/dcn10
.PATH:	${S}/external/bsd/drm2/dist/drm/amd/amdgpu/../display/dc/hubbub/dcn20
.PATH:	${S}/external/bsd/drm2/dist/drm/amd/amdgpu/../display/dc/hubbub/dcn201
.PATH:	${S}/external/bsd/drm2/dist/drm/amd/amdgpu/../display/dc/hubbub/dcn21
.PATH:	${S}/external/bsd/drm2/dist/drm/amd/amdgpu/../display/dc/hubbub/dcn30
.PATH:	${S}/external/bsd/drm2/dist/drm/amd/amdgpu/../display/dc/hubbub/dcn301
.PATH:	${S}/external/bsd/drm2/dist/drm/amd/amdgpu/../display/dc/hubbub/dcn31
.PATH:	${S}/external/bsd/drm2/dist/drm/amd/amdgpu/../display/dc/hubbub/dcn32
.PATH:	${S}/external/bsd/drm2/dist/drm/amd/amdgpu/../display/dc/hubbub/dcn35
.PATH:	${S}/external/bsd/drm2/dist/drm/amd/amdgpu/../display/dc/hubbub/dcn401
.PATH:	${S}/external/bsd/drm2/dist/drm/amd/amdgpu/../display/dc/hubp/dcn10
.PATH:	${S}/external/bsd/drm2/dist/drm/amd/amdgpu/../display/dc/hubp/dcn20
.PATH:	${S}/external/bsd/drm2/dist/drm/amd/amdgpu/../display/dc/hubp/dcn201
.PATH:	${S}/external/bsd/drm2/dist/drm/amd/amdgpu/../display/dc/hubp/dcn21
.PATH:	${S}/external/bsd/drm2/dist/drm/amd/amdgpu/../display/dc/hubp/dcn30
.PATH:	${S}/external/bsd/drm2/dist/drm/amd/amdgpu/../display/dc/hubp/dcn31
.PATH:	${S}/external/bsd/drm2/dist/drm/amd/amdgpu/../display/dc/hubp/dcn32
.PATH:	${S}/external/bsd/drm2/dist/drm/amd/amdgpu/../display/dc/hubp/dcn35
.PATH:	${S}/external/bsd/drm2/dist/drm/amd/amdgpu/../display/dc/hubp/dcn401
.PATH:	${S}/external/bsd/drm2/dist/drm/amd/amdgpu/../display/dc/hwss/dce
.PATH:	${S}/external/bsd/drm2/dist/drm/amd/amdgpu/../display/dc/hwss/dce100
.PATH:	${S}/external/bsd/drm2/dist/drm/amd/amdgpu/../display/dc/hwss/dce110
.PATH:	${S}/external/bsd/drm2/dist/drm/amd/amdgpu/../display/dc/hwss/dce112
.PATH:	${S}/external/bsd/drm2/dist/drm/amd/amdgpu/../display/dc/hwss/dce120
.PATH:	${S}/external/bsd/drm2/dist/drm/amd/amdgpu/../display/dc/hwss/dce80
.PATH:	${S}/external/bsd/drm2/dist/drm/amd/amdgpu/../display/dc/hwss/dcn10
.PATH:	${S}/external/bsd/drm2/dist/drm/amd/amdgpu/../display/dc/hwss/dcn20
.PATH:	${S}/external/bsd/drm2/dist/drm/amd/amdgpu/../display/dc/hwss/dcn201
.PATH:	${S}/external/bsd/drm2/dist/drm/amd/amdgpu/../display/dc/hwss/dcn21
.PATH:	${S}/external/bsd/drm2/dist/drm/amd/amdgpu/../display/dc/hwss/dcn30
.PATH:	${S}/external/bsd/drm2/dist/drm/amd/amdgpu/../display/dc/hwss/dcn301
.PATH:	${S}/external/bsd/drm2/dist/drm/amd/amdgpu/../display/dc/hwss/dcn302
.PATH:	${S}/external/bsd/drm2/dist/drm/amd/amdgpu/../display/dc/hwss/dcn303
.PATH:	${S}/external/bsd/drm2/dist/drm/amd/amdgpu/../display/dc/hwss/dcn31
.PATH:	${S}/external/bsd/drm2/dist/drm/amd/amdgpu/../display/dc/hwss/dcn314
.PATH:	${S}/external/bsd/drm2/dist/drm/amd/amdgpu/../display/dc/hwss/dcn32
.PATH:	${S}/external/bsd/drm2/dist/drm/amd/amdgpu/../display/dc/hwss/dcn35
.PATH:	${S}/external/bsd/drm2/dist/drm/amd/amdgpu/../display/dc/hwss/dcn351
.PATH:	${S}/external/bsd/drm2/dist/drm/amd/amdgpu/../display/dc/hwss/dcn401
.PATH:	${S}/external/bsd/drm2/dist/drm/amd/amdgpu/../display/dc/irq
.PATH:	${S}/external/bsd/drm2/dist/drm/amd/amdgpu/../display/dc/irq/dce110
.PATH:	${S}/external/bsd/drm2/dist/drm/amd/amdgpu/../display/dc/irq/dce120
.PATH:	${S}/external/bsd/drm2/dist/drm/amd/amdgpu/../display/dc/irq/dce60
.PATH:	${S}/external/bsd/drm2/dist/drm/amd/amdgpu/../display/dc/irq/dce80
.PATH:	${S}/external/bsd/drm2/dist/drm/amd/amdgpu/../display/dc/irq/dcn10
.PATH:	${S}/external/bsd/drm2/dist/drm/amd/amdgpu/../display/dc/irq/dcn20
.PATH:	${S}/external/bsd/drm2/dist/drm/amd/amdgpu/../display/dc/irq/dcn201
.PATH:	${S}/external/bsd/drm2/dist/drm/amd/amdgpu/../display/dc/irq/dcn21
.PATH:	${S}/external/bsd/drm2/dist/drm/amd/amdgpu/../display/dc/irq/dcn30
.PATH:	${S}/external/bsd/drm2/dist/drm/amd/amdgpu/../display/dc/irq/dcn302
.PATH:	${S}/external/bsd/drm2/dist/drm/amd/amdgpu/../display/dc/irq/dcn303
.PATH:	${S}/external/bsd/drm2/dist/drm/amd/amdgpu/../display/dc/irq/dcn31
.PATH:	${S}/external/bsd/drm2/dist/drm/amd/amdgpu/../display/dc/irq/dcn314
.PATH:	${S}/external/bsd/drm2/dist/drm/amd/amdgpu/../display/dc/irq/dcn315
.PATH:	${S}/external/bsd/drm2/dist/drm/amd/amdgpu/../display/dc/irq/dcn32
.PATH:	${S}/external/bsd/drm2/dist/drm/amd/amdgpu/../display/dc/irq/dcn35
.PATH:	${S}/external/bsd/drm2/dist/drm/amd/amdgpu/../display/dc/irq/dcn351
.PATH:	${S}/external/bsd/drm2/dist/drm/amd/amdgpu/../display/dc/irq/dcn401
.PATH:	${S}/external/bsd/drm2/dist/drm/amd/amdgpu/../display/dc/link
.PATH:	${S}/external/bsd/drm2/dist/drm/amd/amdgpu/../display/dc/link/accessories
.PATH:	${S}/external/bsd/drm2/dist/drm/amd/amdgpu/../display/dc/link/hwss
.PATH:	${S}/external/bsd/drm2/dist/drm/amd/amdgpu/../display/dc/link/protocols
.PATH:	${S}/external/bsd/drm2/dist/drm/amd/amdgpu/../display/dc/mmhubbub/dcn20
.PATH:	${S}/external/bsd/drm2/dist/drm/amd/amdgpu/../display/dc/mmhubbub/dcn32
.PATH:	${S}/external/bsd/drm2/dist/drm/amd/amdgpu/../display/dc/mmhubbub/dcn35
.PATH:	${S}/external/bsd/drm2/dist/drm/amd/amdgpu/../display/dc/mpc/dcn10
.PATH:	${S}/external/bsd/drm2/dist/drm/amd/amdgpu/../display/dc/mpc/dcn20
.PATH:	${S}/external/bsd/drm2/dist/drm/amd/amdgpu/../display/dc/mpc/dcn30
.PATH:	${S}/external/bsd/drm2/dist/drm/amd/amdgpu/../display/dc/mpc/dcn32
.PATH:	${S}/external/bsd/drm2/dist/drm/amd/amdgpu/../display/dc/mpc/dcn401
.PATH:	${S}/external/bsd/drm2/dist/drm/amd/amdgpu/../display/dc/opp/dcn10
.PATH:	${S}/external/bsd/drm2/dist/drm/amd/amdgpu/../display/dc/opp/dcn20
.PATH:	${S}/external/bsd/drm2/dist/drm/amd/amdgpu/../display/dc/opp/dcn35
.PATH:	${S}/external/bsd/drm2/dist/drm/amd/amdgpu/../display/dc/optc/dcn10
.PATH:	${S}/external/bsd/drm2/dist/drm/amd/amdgpu/../display/dc/optc/dcn20
.PATH:	${S}/external/bsd/drm2/dist/drm/amd/amdgpu/../display/dc/optc/dcn201
.PATH:	${S}/external/bsd/drm2/dist/drm/amd/amdgpu/../display/dc/optc/dcn30
.PATH:	${S}/external/bsd/drm2/dist/drm/amd/amdgpu/../display/dc/optc/dcn301
.PATH:	${S}/external/bsd/drm2/dist/drm/amd/amdgpu/../display/dc/optc/dcn31
.PATH:	${S}/external/bsd/drm2/dist/drm/amd/amdgpu/../display/dc/optc/dcn314
.PATH:	${S}/external/bsd/drm2/dist/drm/amd/amdgpu/../display/dc/optc/dcn32
.PATH:	${S}/external/bsd/drm2/dist/drm/amd/amdgpu/../display/dc/optc/dcn35
.PATH:	${S}/external/bsd/drm2/dist/drm/amd/amdgpu/../display/dc/optc/dcn401
.PATH:	${S}/external/bsd/drm2/dist/drm/amd/amdgpu/../display/dc/pg/dcn35
.PATH:	${S}/external/bsd/drm2/dist/drm/amd/amdgpu/../display/dc/resource/dce100
.PATH:	${S}/external/bsd/drm2/dist/drm/amd/amdgpu/../display/dc/resource/dce110
.PATH:	${S}/external/bsd/drm2/dist/drm/amd/amdgpu/../display/dc/resource/dce112
.PATH:	${S}/external/bsd/drm2/dist/drm/amd/amdgpu/../display/dc/resource/dce120
.PATH:	${S}/external/bsd/drm2/dist/drm/amd/amdgpu/../display/dc/resource/dce80
.PATH:	${S}/external/bsd/drm2/dist/drm/amd/amdgpu/../display/dc/resource/dcn10
.PATH:	${S}/external/bsd/drm2/dist/drm/amd/amdgpu/../display/dc/resource/dcn20
.PATH:	${S}/external/bsd/drm2/dist/drm/amd/amdgpu/../display/dc/resource/dcn201
.PATH:	${S}/external/bsd/drm2/dist/drm/amd/amdgpu/../display/dc/resource/dcn21
.PATH:	${S}/external/bsd/drm2/dist/drm/amd/amdgpu/../display/dc/resource/dcn30
.PATH:	${S}/external/bsd/drm2/dist/drm/amd/amdgpu/../display/dc/resource/dcn301
.PATH:	${S}/external/bsd/drm2/dist/drm/amd/amdgpu/../display/dc/resource/dcn302
.PATH:	${S}/external/bsd/drm2/dist/drm/amd/amdgpu/../display/dc/resource/dcn303
.PATH:	${S}/external/bsd/drm2/dist/drm/amd/amdgpu/../display/dc/resource/dcn31
.PATH:	${S}/external/bsd/drm2/dist/drm/amd/amdgpu/../display/dc/resource/dcn314
.PATH:	${S}/external/bsd/drm2/dist/drm/amd/amdgpu/../display/dc/resource/dcn315
.PATH:	${S}/external/bsd/drm2/dist/drm/amd/amdgpu/../display/dc/resource/dcn316
.PATH:	${S}/external/bsd/drm2/dist/drm/amd/amdgpu/../display/dc/resource/dcn32
.PATH:	${S}/external/bsd/drm2/dist/drm/amd/amdgpu/../display/dc/resource/dcn321
.PATH:	${S}/external/bsd/drm2/dist/drm/amd/amdgpu/../display/dc/resource/dcn35
.PATH:	${S}/external/bsd/drm2/dist/drm/amd/amdgpu/../display/dc/resource/dcn351
.PATH:	${S}/external/bsd/drm2/dist/drm/amd/amdgpu/../display/dc/resource/dcn401
.PATH:	${S}/external/bsd/drm2/dist/drm/amd/amdgpu/../display/dc/spl
.PATH:	${S}/external/bsd/drm2/dist/drm/amd/amdgpu/../display/dc/virtual
.PATH:	${S}/external/bsd/drm2/dist/drm/amd/amdgpu/../display/dmub/src
.PATH:	${S}/external/bsd/drm2/dist/drm/amd/amdgpu/../display/modules/color
.PATH:	${S}/external/bsd/drm2/dist/drm/amd/amdgpu/../display/modules/freesync
.PATH:	${S}/external/bsd/drm2/dist/drm/amd/amdgpu/../display/modules/hdcp
.PATH:	${S}/external/bsd/drm2/dist/drm/amd/amdgpu/../display/modules/info_packet
.PATH:	${S}/external/bsd/drm2/dist/drm/amd/amdgpu/../display/modules/power

SRCS+=	amdgpu_color_table.c
SRCS+=	amdgpu_dc_edid_parser.c
SRCS+=	amdgpu_dc_link_enc_cfg.c
SRCS+=	amdgpu_dc_link_exports.c
SRCS+=	amdgpu_dc_spl.c
SRCS+=	amdgpu_dc_spl_filters.c
SRCS+=	amdgpu_dc_spl_isharp_filters.c
SRCS+=	amdgpu_dc_spl_scl_easf_filters.c
SRCS+=	amdgpu_dc_spl_scl_filters.c
SRCS+=	amdgpu_dc_spl_translate.c
SRCS+=	amdgpu_dc_stat.c
SRCS+=	amdgpu_dc_state.c
SRCS+=	amdgpu_dce_panel_cntl.c
SRCS+=	amdgpu_dcn10_fpu.c
SRCS+=	amdgpu_dcn201_clk_mgr.c
SRCS+=	amdgpu_dcn201_dccg.c
SRCS+=	amdgpu_dcn201_dpp.c
SRCS+=	amdgpu_dcn201_hubbub.c
SRCS+=	amdgpu_dcn201_hubp.c
SRCS+=	amdgpu_dcn201_hwseq.c
SRCS+=	amdgpu_dcn201_init.c
SRCS+=	amdgpu_dcn201_link_encoder.c
SRCS+=	amdgpu_dcn201_mpc.c
SRCS+=	amdgpu_dcn201_opp.c
SRCS+=	amdgpu_dcn201_optc.c
SRCS+=	amdgpu_dcn201_resource.c
SRCS+=	amdgpu_dcn20_fpu.c
SRCS+=	amdgpu_dcn21_dccg.c
SRCS+=	amdgpu_dcn301_dccg.c
SRCS+=	amdgpu_dcn301_dio_link_encoder.c
SRCS+=	amdgpu_dcn301_fpu.c
SRCS+=	amdgpu_dcn301_hubbub.c
SRCS+=	amdgpu_dcn301_hwseq.c
SRCS+=	amdgpu_dcn301_init.c
SRCS+=	amdgpu_dcn301_optc.c
SRCS+=	amdgpu_dcn301_panel_cntl.c
SRCS+=	amdgpu_dcn301_resource.c
SRCS+=	amdgpu_dcn301_smu.c
SRCS+=	amdgpu_dcn302_fpu.c
SRCS+=	amdgpu_dcn302_hwseq.c
SRCS+=	amdgpu_dcn302_init.c
SRCS+=	amdgpu_dcn302_resource.c
SRCS+=	amdgpu_dcn303_fpu.c
SRCS+=	amdgpu_dcn303_hwseq.c
SRCS+=	amdgpu_dcn303_init.c
SRCS+=	amdgpu_dcn303_resource.c
SRCS+=	amdgpu_dcn315_clk_mgr.c
SRCS+=	amdgpu_dcn315_resource.c
SRCS+=	amdgpu_dcn315_smu.c
SRCS+=	amdgpu_dcn316_clk_mgr.c
SRCS+=	amdgpu_dcn316_resource.c
SRCS+=	amdgpu_dcn316_smu.c
SRCS+=	amdgpu_dmub_abm.c
SRCS+=	amdgpu_dmub_abm_lcd.c
SRCS+=	amdgpu_dmub_dcn301.c
SRCS+=	amdgpu_dmub_dcn302.c
SRCS+=	amdgpu_dmub_dcn303.c
SRCS+=	amdgpu_dmub_dcn315.c
SRCS+=	amdgpu_dmub_dcn316.c
SRCS+=	amdgpu_dmub_hw_lock_mgr.c
SRCS+=	amdgpu_dmub_outbox.c
SRCS+=	amdgpu_dmub_psr.c
SRCS+=	amdgpu_dmub_replay.c
SRCS+=	amdgpu_dmub_srv_stat.c
SRCS+=	amdgpu_hw_factory_dcn315.c
SRCS+=	amdgpu_hw_translate_dcn315.c
SRCS+=	amdgpu_irq_service_dcn201.c
SRCS+=	amdgpu_irq_service_dcn302.c
SRCS+=	amdgpu_irq_service_dcn303.c
SRCS+=	amdgpu_irq_service_dcn315.c
SRCS+=	amdgpu_link_ddc.c
SRCS+=	amdgpu_link_detection.c
SRCS+=	amdgpu_link_dp_capability.c
SRCS+=	amdgpu_link_dp_cts.c
SRCS+=	amdgpu_link_dp_dpia.c
SRCS+=	amdgpu_link_dp_dpia_bw.c
SRCS+=	amdgpu_link_dp_irq_handler.c
SRCS+=	amdgpu_link_dp_phy.c
SRCS+=	amdgpu_link_dp_trace.c
SRCS+=	amdgpu_link_dp_training.c
SRCS+=	amdgpu_link_dp_training_128b_132b.c
SRCS+=	amdgpu_link_dp_training_8b_10b.c
SRCS+=	amdgpu_link_dp_training_auxless.c
SRCS+=	amdgpu_link_dp_training_dpia.c
SRCS+=	amdgpu_link_dp_training_fixed_vs_pe_retimer.c
SRCS+=	amdgpu_link_dpcd.c
SRCS+=	amdgpu_link_dpms.c
SRCS+=	amdgpu_link_edp_panel_control.c
SRCS+=	amdgpu_link_factory.c
SRCS+=	amdgpu_link_hpd.c
SRCS+=	amdgpu_link_hwss_dio.c
SRCS+=	amdgpu_link_hwss_dio_fixed_vs_pe_retimer.c
SRCS+=	amdgpu_link_hwss_dpia.c
SRCS+=	amdgpu_link_hwss_hpo_dp.c
SRCS+=	amdgpu_link_hwss_hpo_fixed_vs_pe_retimer_dp.c
SRCS+=	amdgpu_link_resource.c
SRCS+=	amdgpu_link_validation.c
SRCS+=	amdgpu_rc_calc_fpu.c
SRCS+=	amdgpu_spl_custom_float.c
SRCS+=	amdgpu_spl_fixpt31_32.c
SRCS+=	amdgpu_vg_clk_mgr.c
SRCS+=	amdgpu_virtual_link_hwss.c
# --- end display sync ---

.include <bsd.kmodule.mk>

# Belt-and-suspenders: drop any leftover .SUNW_ctf (bootloader stall).
realall: strip-amdgpu-ctf
strip-amdgpu-ctf: .PHONY ${PROG}
	@if ${OBJDUMP} -h ${PROG} 2>/dev/null | ${GREP} -q SUNW_ctf; then \
		echo "Stripping .SUNW_ctf from ${PROG}"; \
		${OBJCOPY} -R .SUNW_ctf ${PROG} ${PROG}.noctf && \
		mv ${PROG}.noctf ${PROG}; \
	fi

# XXX
CFLAGS+=	${CWARNFLAGS.${.IMPSRC:T}}
CWARNFLAGS.amdgpu_dce_clock_source.c+=	-Wno-unused-variable
CWARNFLAGS.amdgpu_dce_calcs.c+=	-Wno-cast-qual
CWARNFLAGS.amdgpu_dce_aux.c+=	-Wno-cast-qual -Wno-discarded-qualifiers
