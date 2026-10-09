/*	$NetBSD$	*/

/* XXX amdgpu: minimal Linux irq_chip / irq_data stubs for NetBSD */

#ifndef _LINUX_IRQ_H_
#define _LINUX_IRQ_H_

#include <sys/types.h>

#include <linux/interrupt.h>

typedef unsigned long irq_hw_number_t;

struct irq_data {
	unsigned int irq;
	irq_hw_number_t hwirq;
	void *chip_data;
};

struct irq_chip {
	const char *name;
	void (*irq_mask)(struct irq_data *);
	void (*irq_unmask)(struct irq_data *);
};

#define	handle_simple_irq	((void *)0)

static inline void
irq_set_chip_and_handler(unsigned int irq, struct irq_chip *chip, void *handle)
{
	(void)irq;
	(void)chip;
	(void)handle;
}

#endif /* _LINUX_IRQ_H_ */
