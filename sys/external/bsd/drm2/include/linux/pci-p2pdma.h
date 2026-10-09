/*	$NetBSD$	*/

/* SPDX-License-Identifier: GPL-2.0 */

/*
 * NetBSD: CONFIG_PCI_P2PDMA is not enabled; provide the CONFIG-off stubs
 * from Linux so amdgpu can call pci_p2pdma_distance().
 */

#ifndef _LINUX_PCI_P2PDMA_H
#define _LINUX_PCI_P2PDMA_H

#include <sys/errno.h>

#include <linux/types.h>
#include <linux/pci.h>

struct block_device;
struct scatterlist;

#ifndef pci_bus_addr_t
typedef u64 pci_bus_addr_t;
#endif

static inline int
pci_p2pdma_add_resource(struct pci_dev *pdev, int bar, size_t size, u64 offset)
{
	(void)pdev;
	(void)bar;
	(void)size;
	(void)offset;
	return -EOPNOTSUPP;
}

static inline int
pci_p2pdma_distance_many(struct pci_dev *provider, struct device **clients,
    int num_clients, bool verbose)
{
	(void)provider;
	(void)clients;
	(void)num_clients;
	(void)verbose;
	return -1;
}

static inline bool
pci_has_p2pmem(struct pci_dev *pdev)
{
	(void)pdev;
	return false;
}

static inline struct pci_dev *
pci_p2pmem_find_many(struct device **clients, int num_clients)
{
	(void)clients;
	(void)num_clients;
	return NULL;
}

static inline void *
pci_alloc_p2pmem(struct pci_dev *pdev, size_t size)
{
	(void)pdev;
	(void)size;
	return NULL;
}

static inline void
pci_free_p2pmem(struct pci_dev *pdev, void *addr, size_t size)
{
	(void)pdev;
	(void)addr;
	(void)size;
}

static inline pci_bus_addr_t
pci_p2pmem_virt_to_bus(struct pci_dev *pdev, void *addr)
{
	(void)pdev;
	(void)addr;
	return 0;
}

static inline struct scatterlist *
pci_p2pmem_alloc_sgl(struct pci_dev *pdev, unsigned int *nents, u32 length)
{
	(void)pdev;
	(void)nents;
	(void)length;
	return NULL;
}

static inline void
pci_p2pmem_free_sgl(struct pci_dev *pdev, struct scatterlist *sgl)
{
	(void)pdev;
	(void)sgl;
}

static inline void
pci_p2pmem_publish(struct pci_dev *pdev, bool publish)
{
	(void)pdev;
	(void)publish;
}

static inline int
pci_p2pdma_enable_store(const char *page, struct pci_dev **p2p_dev,
    bool *use_p2pdma)
{
	(void)page;
	(void)p2p_dev;
	*use_p2pdma = false;
	return 0;
}

static inline ssize_t
pci_p2pdma_enable_show(char *page, struct pci_dev *p2p_dev, bool use_p2pdma)
{
	(void)p2p_dev;
	(void)use_p2pdma;
	if (page) {
		page[0] = 'n';
		page[1] = 'o';
		page[2] = 'n';
		page[3] = 'e';
		page[4] = '\n';
		page[5] = '\0';
	}
	return 5;
}

static inline int
pci_p2pdma_distance(struct pci_dev *provider, struct device *client,
    bool verbose)
{
	return pci_p2pdma_distance_many(provider, &client, 1, verbose);
}

static inline struct pci_dev *
pci_p2pmem_find(struct device *client)
{
	return pci_p2pmem_find_many(&client, 1);
}

#endif /* _LINUX_PCI_P2PDMA_H */
