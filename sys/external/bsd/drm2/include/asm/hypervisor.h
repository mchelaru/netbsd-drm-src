/*	$NetBSD$	*/

/* XXX amdgpu: minimal Linux asm/hypervisor.h stub for NetBSD */

#ifndef _ASM_HYPERVISOR_H_
#define _ASM_HYPERVISOR_H_

#include <sys/types.h>
#include <linux/types.h>

enum x86_hypervisor_type {
	X86_HYPER_NATIVE = 0,
	X86_HYPER_VMWARE,
	X86_HYPER_MS_HYPERV,
	X86_HYPER_XEN_PV,
	X86_HYPER_XEN_HVM,
	X86_HYPER_KVM,
	X86_HYPER_JAILHOUSE,
	X86_HYPER_ACRN,
};

static inline bool
hypervisor_is_type(enum x86_hypervisor_type type)
{
	(void)type;
	return false;
}

#endif /* _ASM_HYPERVISOR_H_ */
