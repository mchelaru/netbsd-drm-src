/*	$NetBSD$	*/

#ifndef _LINUX_PGTABLE_H_
#define _LINUX_PGTABLE_H_

#include <linux/mm_types.h>
#include <asm/pgtable.h>

#ifndef pgprot_decrypted
#define	pgprot_decrypted(prot)	(prot)
#endif

#endif /* _LINUX_PGTABLE_H_ */
