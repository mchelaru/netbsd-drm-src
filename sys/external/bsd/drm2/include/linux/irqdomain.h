/*	$NetBSD$	*/

/* XXX amdgpu: minimal Linux irq_domain stubs for NetBSD */

#ifndef _LINUX_IRQDOMAIN_H_
#define _LINUX_IRQDOMAIN_H_

#include <sys/types.h>
#include <sys/errno.h>

#include <linux/irq.h>

struct irq_domain {
	void *host_data;
};

struct irq_domain_ops {
	int (*map)(struct irq_domain *d, unsigned int virq,
	    irq_hw_number_t hwirq);
};

static inline struct irq_domain *
irq_domain_add_linear(void *fwnode, unsigned int size,
    const struct irq_domain_ops *ops, void *host_data)
{
	(void)fwnode;
	(void)size;
	(void)ops;
	(void)host_data;
	return NULL;
}

static inline void
irq_domain_remove(struct irq_domain *domain)
{
	(void)domain;
}

static inline unsigned int
irq_create_mapping(struct irq_domain *domain, irq_hw_number_t hwirq)
{
	(void)domain;
	(void)hwirq;
	return 0;
}

static inline void
generic_handle_domain_irq(struct irq_domain *domain, unsigned int hwirq)
{
	(void)domain;
	(void)hwirq;
}

#endif /* _LINUX_IRQDOMAIN_H_ */
