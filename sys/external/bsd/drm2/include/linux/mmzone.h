/*	$NetBSD$	*/

#ifndef _LINUX_MMZONE_H_
#define _LINUX_MMZONE_H_

#include <linux/mm_types.h>

#ifndef MAX_PAGE_ORDER
#define	MAX_PAGE_ORDER		10
#endif

#define	MAX_ORDER_NR_PAGES	(1 << MAX_PAGE_ORDER)
#define	NR_PAGE_ORDERS		(MAX_PAGE_ORDER + 1)

#endif /* _LINUX_MMZONE_H_ */
