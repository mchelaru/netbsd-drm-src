/*	$NetBSD$	*/

/* SPDX-License-Identifier: GPL-2.0 */

/*
 * NetBSD: CONFIG_XEN / CONFIG_XEN_DOM0 are not enabled for the amdgpu
 * module build.  Provide the CONFIG-off stubs from Linux so
 * xen_initial_domain() (and friends) compile without pulling in
 * NetBSD's arch/xen headers via the module obj/xen symlink.
 */

#ifndef _XEN_XEN_H
#define _XEN_XEN_H

#include <linux/types.h>

enum xen_domain_type {
	XEN_NATIVE,		/* running on bare hardware    */
	XEN_PV_DOMAIN,		/* running in a PV domain      */
	XEN_HVM_DOMAIN,		/* running in a Xen hvm domain */
};

#define xen_domain_type		XEN_NATIVE
#define xen_pvh			0
#define xen_pv_pci_possible	0

#define xen_domain()		(xen_domain_type != XEN_NATIVE)
#define xen_pv_domain()		(xen_domain_type == XEN_PV_DOMAIN)
#define xen_hvm_domain()	(xen_domain_type == XEN_HVM_DOMAIN)
#define xen_pvh_domain()	(xen_pvh)

#define xen_initial_domain()	(0)

#endif /* _XEN_XEN_H */
