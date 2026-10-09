/*	$NetBSD$	*/

#ifndef _LINUX_MMU_NOTIFIER_H_
#define _LINUX_MMU_NOTIFIER_H_

#include <sys/types.h>

/*
 * Stub MMU notifier types for amdgpu/KFD builds without HSA SVM.
 */
struct mmu_notifier;
struct mmu_interval_notifier {
	unsigned long		interval_seq;
	void			*mm;
};

struct mmu_notifier_range {
	unsigned long		start;
	unsigned long		end;
};

static inline void
mmu_interval_read_begin(struct mmu_interval_notifier *mni)
{
	(void)mni;
}

static inline bool
mmu_interval_read_retry(struct mmu_interval_notifier *mni, unsigned long seq)
{
	(void)mni;
	(void)seq;
	return false;
}

#endif /* _LINUX_MMU_NOTIFIER_H_ */
