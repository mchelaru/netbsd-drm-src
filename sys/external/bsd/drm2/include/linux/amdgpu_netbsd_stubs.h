/* XXX amdgpu: NetBSD stubs for Linux-only PCI/PM/IOMMU helpers used by amdgpu */

#ifndef _LINUX_AMDGPU_NETBSD_STUBS_H_
#define _LINUX_AMDGPU_NETBSD_STUBS_H_

#include <sys/types.h>
#include <sys/errno.h>

#include <linux/types.h>
#include <linux/list.h>

struct pci_dev;
struct device;
struct iommu_domain;
struct pci_saved_state;
struct kobject;

#ifndef CONFIG_HOTPLUG_PCI_PCIE
#define CONFIG_HOTPLUG_PCI_PCIE	0
#endif

#ifndef PCI_COMMAND
#define PCI_COMMAND		0x04	/* Linux PCI config command register */
#endif

#ifndef PCI_POSSIBLE_ERROR
#define PCI_POSSIBLE_ERROR(val)	(((val) & 0xffffffffU) == 0xffffffffU)
#endif

#ifndef TAINT_SOFTLOCKUP
#define TAINT_SOFTLOCKUP	0
#endif

#ifndef PM_HIBERNATION_PREPARE
#define PM_HIBERNATION_PREPARE	0x0001
#endif
#ifndef PM_SUSPEND_PREPARE
#define PM_SUSPEND_PREPARE	0x0002
#endif

#ifndef IOMMU_DOMAIN_IDENTITY
#define IOMMU_DOMAIN_IDENTITY	0
#endif
#ifndef IOMMU_DOMAIN_DMA
#define IOMMU_DOMAIN_DMA	1
#endif
#ifndef IOMMU_DOMAIN_DMA_FQ
#define IOMMU_DOMAIN_DMA_FQ	2
#endif

struct iommu_domain {
	unsigned int type;
};

static inline bool
dev_is_removable(struct device *dev)
{
	(void)dev;
	return false;
}

static inline struct pci_dev *
pcie_find_root_port(struct pci_dev *dev)
{
	(void)dev;
	return NULL;
}

static inline bool
pci_pr3_present(struct pci_dev *pdev)
{
	(void)pdev;
	return false;
}

static inline bool
pcie_aspm_enabled(struct pci_dev *pdev)
{
	(void)pdev;
	return false;
}

static inline struct pci_dev *
pci_upstream_bridge(struct pci_dev *dev)
{
	(void)dev;
	return NULL;
}

static inline int
pci_find_ext_capability(struct pci_dev *dev, int cap)
{
	(void)dev;
	(void)cap;
	return 0;
}

static inline int
pcie_get_width_cap(struct pci_dev *dev)
{
	(void)dev;
	return 0;
}

static inline struct iommu_domain *
iommu_get_domain_for_dev(struct device *dev)
{
	(void)dev;
	return NULL;
}

/* Provided by linux/reboot.h when that header is included first. */
#ifndef _LINUX_REBOOT_H_
static inline void
emergency_restart(void)
{
}
#endif

/*
 * Linux: rotate so @list becomes the new front of @head.
 * list_move_tail(head, list) makes head the new tail after list.
 */
static inline void
list_rotate_to_front(struct list_head *list, struct list_head *head)
{
	list_move_tail(head, list);
}

static inline struct pci_saved_state *
pci_store_saved_state(struct pci_dev *dev)
{
	(void)dev;
	return NULL;
}

static inline int
pci_load_saved_state(struct pci_dev *dev, struct pci_saved_state *state)
{
	(void)dev;
	(void)state;
	return -ENOSYS;
}

/* Prefer existing NetBSD helper; declare if header order lacks it. */
void linux_pci_disable_device(struct pci_dev *);

static inline void
pci_disable_device(struct pci_dev *dev)
{
	linux_pci_disable_device(dev);
}

static inline int
pci_wait_for_pending_transaction(struct pci_dev *dev)
{
	(void)dev;
	return 0;
}

static inline void
unmap_mapping_range(void *mapping, loff_t hole_start, loff_t hole_end,
    int even_cows)
{
	(void)mapping;
	(void)hole_start;
	(void)hole_end;
	(void)even_cows;
}

static inline struct device *
kobj_to_dev(struct kobject *kobj)
{
	(void)kobj;
	return NULL;
}

static inline bool
pm_resume_via_firmware(void)
{
	return false;
}

#ifndef min_not_zero
#define min_not_zero(x, y) ({			\
	typeof(x) __x = (x);			\
	typeof(y) __y = (y);			\
	__x == 0 ? __y : ((__y == 0) ? __x : min(__x, __y)); \
})
#endif

#endif /* _LINUX_AMDGPU_NETBSD_STUBS_H_ */
