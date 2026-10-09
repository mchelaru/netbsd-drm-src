#ifndef _ASM_SET_MEMORY_H_
#define _ASM_SET_MEMORY_H_

#include <linux/mm_types.h>

static inline int
set_pages_wb(struct page *page, int numpages)
{
	(void)page;
	(void)numpages;
	return 0;
}

static inline int
set_pages_array_wc(struct page **pages, int addrinarray)
{
	(void)pages;
	(void)addrinarray;
	return 0;
}

static inline int
set_pages_array_uc(struct page **pages, int addrinarray)
{
	(void)pages;
	(void)addrinarray;
	return 0;
}

static inline int
set_pages_array_wb(struct page **pages, int addrinarray)
{
	(void)pages;
	(void)addrinarray;
	return 0;
}

#endif /* _ASM_SET_MEMORY_H_ */
