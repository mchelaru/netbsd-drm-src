#ifndef _ASM_PGTABLE_H_
#define _ASM_PGTABLE_H_

#include <sys/types.h>

#include <uvm/uvm_extern.h>
#include <uvm/uvm_pmap.h>

#include <linux/mm_types.h>

/*
 * NetBSD pgprot_t carries UVM_PROT_* plus optional PMAP_* cache flags
 * for pmap_kenter_pa (see linux vmap()).
 */
#define	pgprot_writecombine(prot)	((pgprot_t)((prot) | PMAP_WRITE_COMBINE))
#define	pgprot_noncached(prot)		((pgprot_t)((prot) | PMAP_NOCACHE))
#define	pgprot_noncached_wc(prot)	pgprot_writecombine(prot)

#endif /* _ASM_PGTABLE_H_ */
