/*	$NetBSD$	*/

#ifndef _LINUX_TOPOLOGY_H_
#define _LINUX_TOPOLOGY_H_

#include <sys/types.h>

static inline int
num_online_nodes(void)
{
	return 1;
}

static inline int
num_possible_nodes(void)
{
	return 1;
}

static inline int
numa_node_id(void)
{
	return 0;
}

/* Linux topology helper used by APU SMU; approximate with online CPUs. */
static inline int
topology_num_cores_per_package(void)
{
	extern int ncpu;

	return ncpu > 0 ? ncpu : 1;
}

#endif /* _LINUX_TOPOLOGY_H_ */
