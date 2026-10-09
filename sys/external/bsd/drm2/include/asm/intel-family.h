/*
 * NetBSD stub for <asm/intel-family.h>.
 *
 * amdgpu_device.c includes this header but currently only checks
 * X86_VENDOR_INTEL (from elsewhere).  Provide the Intel family/model
 * constants from Linux so any future VFM matches compile.
 *
 * VFM_MAKE / X86_*_ANY live in Linux's cpu_device_id.h; define the
 * minimal pieces locally so this header stands alone.
 */

#ifndef _ASM_X86_INTEL_FAMILY_H
#define _ASM_X86_INTEL_FAMILY_H

#ifndef X86_VENDOR_INTEL
#define X86_VENDOR_INTEL	0
#endif
#ifndef X86_FAMILY_ANY
#define X86_FAMILY_ANY		0
#endif
#ifndef X86_MODEL_ANY
#define X86_MODEL_ANY		0
#endif
#ifndef VFM_MAKE
#define VFM_MAKE(_vendor, _family, _model)	\
	(((_vendor) << 24) | ((_family) << 16) | ((_model) << 8))
#endif

#define IFM(_fam, _model)	VFM_MAKE(X86_VENDOR_INTEL, _fam, _model)

#define INTEL_ANY			IFM(X86_FAMILY_ANY, X86_MODEL_ANY)

#define INTEL_PENTIUM_PRO		IFM(6, 0x01)

#define INTEL_CORE_YONAH		IFM(6, 0x0E)

#define INTEL_CORE2_MEROM		IFM(6, 0x0F)
#define INTEL_CORE2_MEROM_L		IFM(6, 0x16)
#define INTEL_CORE2_PENRYN		IFM(6, 0x17)
#define INTEL_CORE2_DUNNINGTON		IFM(6, 0x1D)

#define INTEL_NEHALEM			IFM(6, 0x1E)
#define INTEL_NEHALEM_G			IFM(6, 0x1F)
#define INTEL_NEHALEM_EP		IFM(6, 0x1A)
#define INTEL_NEHALEM_EX		IFM(6, 0x2E)

#define INTEL_WESTMERE			IFM(6, 0x25)
#define INTEL_WESTMERE_EP		IFM(6, 0x2C)
#define INTEL_WESTMERE_EX		IFM(6, 0x2F)

#define INTEL_SANDYBRIDGE		IFM(6, 0x2A)
#define INTEL_SANDYBRIDGE_X		IFM(6, 0x2D)
#define INTEL_IVYBRIDGE			IFM(6, 0x3A)
#define INTEL_IVYBRIDGE_X		IFM(6, 0x3E)

#define INTEL_HASWELL			IFM(6, 0x3C)
#define INTEL_HASWELL_X			IFM(6, 0x3F)
#define INTEL_HASWELL_L			IFM(6, 0x45)
#define INTEL_HASWELL_G			IFM(6, 0x46)

#define INTEL_BROADWELL			IFM(6, 0x3D)
#define INTEL_BROADWELL_G		IFM(6, 0x47)
#define INTEL_BROADWELL_X		IFM(6, 0x4F)
#define INTEL_BROADWELL_D		IFM(6, 0x56)

#define INTEL_SKYLAKE_L			IFM(6, 0x4E)
#define INTEL_SKYLAKE			IFM(6, 0x5E)
#define INTEL_SKYLAKE_X			IFM(6, 0x55)

#define INTEL_KABYLAKE_L		IFM(6, 0x8E)
#define INTEL_KABYLAKE			IFM(6, 0x9E)

#define INTEL_COMETLAKE			IFM(6, 0xA5)
#define INTEL_COMETLAKE_L		IFM(6, 0xA6)

#define INTEL_CANNONLAKE_L		IFM(6, 0x66)

#define INTEL_ICELAKE_X			IFM(6, 0x6A)
#define INTEL_ICELAKE_D			IFM(6, 0x6C)
#define INTEL_ICELAKE			IFM(6, 0x7D)
#define INTEL_ICELAKE_L			IFM(6, 0x7E)
#define INTEL_ICELAKE_NNPI		IFM(6, 0x9D)

#define INTEL_ROCKETLAKE		IFM(6, 0xA7)

#define INTEL_TIGERLAKE_L		IFM(6, 0x8C)
#define INTEL_TIGERLAKE			IFM(6, 0x8D)

#define INTEL_SAPPHIRERAPIDS_X		IFM(6, 0x8F)

#define INTEL_EMERALDRAPIDS_X		IFM(6, 0xCF)

#define INTEL_GRANITERAPIDS_X		IFM(6, 0xAD)
#define INTEL_GRANITERAPIDS_D		IFM(6, 0xAE)

#define INTEL_LAKEFIELD			IFM(6, 0x8A)

#define INTEL_ALDERLAKE			IFM(6, 0x97)
#define INTEL_ALDERLAKE_L		IFM(6, 0x9A)

#define INTEL_RAPTORLAKE		IFM(6, 0xB7)
#define INTEL_RAPTORLAKE_P		IFM(6, 0xBA)
#define INTEL_RAPTORLAKE_S		IFM(6, 0xBF)

#define INTEL_METEORLAKE		IFM(6, 0xAC)
#define INTEL_METEORLAKE_L		IFM(6, 0xAA)

#define INTEL_ARROWLAKE_H		IFM(6, 0xC5)
#define INTEL_ARROWLAKE			IFM(6, 0xC6)
#define INTEL_ARROWLAKE_U		IFM(6, 0xB5)

#define INTEL_LUNARLAKE_M		IFM(6, 0xBD)

#define INTEL_PANTHERLAKE_L		IFM(6, 0xCC)

#define INTEL_ATOM_BONNELL		IFM(6, 0x1C)
#define INTEL_ATOM_BONNELL_MID		IFM(6, 0x26)

#define INTEL_ATOM_SALTWELL		IFM(6, 0x36)
#define INTEL_ATOM_SALTWELL_MID		IFM(6, 0x27)
#define INTEL_ATOM_SALTWELL_TABLET	IFM(6, 0x35)

#define INTEL_ATOM_SILVERMONT		IFM(6, 0x37)
#define INTEL_ATOM_SILVERMONT_D		IFM(6, 0x4D)
#define INTEL_ATOM_SILVERMONT_MID	IFM(6, 0x4A)

#define INTEL_ATOM_AIRMONT		IFM(6, 0x4C)
#define INTEL_ATOM_AIRMONT_MID		IFM(6, 0x5A)
#define INTEL_ATOM_AIRMONT_NP		IFM(6, 0x75)

#define INTEL_ATOM_GOLDMONT		IFM(6, 0x5C)
#define INTEL_ATOM_GOLDMONT_D		IFM(6, 0x5F)
#define INTEL_ATOM_GOLDMONT_PLUS	IFM(6, 0x7A)

#define INTEL_ATOM_TREMONT_D		IFM(6, 0x86)
#define INTEL_ATOM_TREMONT		IFM(6, 0x96)
#define INTEL_ATOM_TREMONT_L		IFM(6, 0x9C)

#define INTEL_ATOM_GRACEMONT		IFM(6, 0xBE)

#define INTEL_ATOM_CRESTMONT_X		IFM(6, 0xAF)
#define INTEL_ATOM_CRESTMONT		IFM(6, 0xB6)

#define INTEL_ATOM_DARKMONT_X		IFM(6, 0xDD)

#define INTEL_XEON_PHI_KNL		IFM(6, 0x57)
#define INTEL_XEON_PHI_KNM		IFM(6, 0x85)

#define INTEL_QUARK_X1000		IFM(5, 0x09)

#define INTEL_PANTHERCOVE_X		IFM(19, 0x01)

enum intel_cpu_type {
	INTEL_CPU_TYPE_ATOM = 0x20,
	INTEL_CPU_TYPE_CORE = 0x40,
};

#endif /* _ASM_X86_INTEL_FAMILY_H */
