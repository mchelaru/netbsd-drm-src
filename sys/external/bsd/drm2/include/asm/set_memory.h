#ifndef _ASM_SET_MEMORY_H_
#define _ASM_SET_MEMORY_H_

#include <sys/types.h>

#include <linux/mm_types.h>

/*
 * Linux PAT helpers.  Change the amd64 direct-map PTE cache mode via
 * pmap_page_set_memattr() (see linux_set_memory.c).  Transient CPA
 * failures are retried; fatal errors are returned to the caller.
 */

int	set_pages_wb(struct page *page, int numpages);
int	set_pages_array_wc(struct page **pages, int addrinarray);
int	set_pages_array_uc(struct page **pages, int addrinarray);
int	set_pages_array_wb(struct page **pages, int addrinarray);

int	linux_set_memory_init(void);
void	linux_set_memory_fini(void);

#endif /* _ASM_SET_MEMORY_H_ */
