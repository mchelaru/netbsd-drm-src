/*	$NetBSD$	*/

#ifndef _LINUX_CGROUP_DMEM_H_
#define _LINUX_CGROUP_DMEM_H_

#include <sys/types.h>
#include <sys/stdbool.h>

#include <linux/types.h>

struct dmem_cgroup_pool_state;
struct dmem_cgroup_region;

static inline int
dmem_cgroup_try_charge(struct dmem_cgroup_region *cg, u64 size,
    struct dmem_cgroup_pool_state **pool,
    struct dmem_cgroup_pool_state **ret_limit_pool)
{
	(void)cg;
	(void)size;
	if (pool)
		*pool = NULL;
	if (ret_limit_pool)
		*ret_limit_pool = NULL;
	return 0;
}

static inline void
dmem_cgroup_uncharge(struct dmem_cgroup_pool_state *pool, u64 size)
{
	(void)pool;
	(void)size;
}

static inline void
dmem_cgroup_pool_state_put(struct dmem_cgroup_pool_state *pool)
{
	(void)pool;
}

static inline bool
dmem_cgroup_state_evict_valuable(struct dmem_cgroup_pool_state *limit,
    struct dmem_cgroup_pool_state *css, bool ignore_low, bool *ret_hit_low)
{
	(void)limit;
	(void)css;
	(void)ignore_low;
	if (ret_hit_low)
		*ret_hit_low = false;
	return true;
}

#endif /* _LINUX_CGROUP_DMEM_H_ */
