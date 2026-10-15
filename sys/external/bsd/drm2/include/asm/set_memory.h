#ifndef _ASM_SET_MEMORY_H_
#define _ASM_SET_MEMORY_H_

#include <sys/types.h>

#include <linux/mm_types.h>

/*
 * Linux PAT helpers.  On NetBSD the direct map is large-page and cannot
 * be CPA-split; see linux_set_memory.c.  These record WC/UC intent and
 * drive kmap to use PMAP_WRITE_COMBINE / PMAP_NOCACHE instead of the WB
 * direct map.
 */

int	set_pages_wb(struct page *page, int numpages);
int	set_pages_array_wc(struct page **pages, int addrinarray);
int	set_pages_array_uc(struct page **pages, int addrinarray);
int	set_pages_array_wb(struct page **pages, int addrinarray);

int	linux_set_memory_init(void);
void	linux_set_memory_fini(void);

u_int	linux_page_pmap_cache_flags(paddr_t paddr);
bool	linux_page_skip_direct_map(paddr_t paddr);

#endif /* _ASM_SET_MEMORY_H_ */
