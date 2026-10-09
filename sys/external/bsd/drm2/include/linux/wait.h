/*	$NetBSD: wait.h,v 1.2 2014/03/18 18:20:43 riastradh Exp $	*/

/*-
 * Copyright (c) 2013 The NetBSD Foundation, Inc.
 * All rights reserved.
 *
 * This code is derived from software contributed to The NetBSD Foundation
 * by Taylor R. Campbell.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE NETBSD FOUNDATION, INC. AND CONTRIBUTORS
 * ``AS IS'' AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED
 * TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
 * PURPOSE ARE DISCLAIMED.  IN NO EVENT SHALL THE FOUNDATION OR CONTRIBUTORS
 * BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 */

#ifndef _LINUX_WAIT_H_
#define _LINUX_WAIT_H_

#include <sys/types.h>
#include <sys/condvar.h>
#include <sys/systm.h>
#include <sys/errno.h>

#include <linux/spinlock.h>

/*
 * Minimal wait-queue stub for embedding in Linux DRM structures.
 * Full wait_event semantics are not ported; callers that sleep will
 * need further work.  @lock matches Linux spinlock_t so code that does
 * spin_lock(&wq.lock) type-checks.
 */
typedef struct wait_queue_head {
	spinlock_t	lock;
	kcondvar_t	cv;
} wait_queue_head_t;

static inline void
init_waitqueue_head(wait_queue_head_t *wq)
{
	spin_lock_init(&wq->lock);
	cv_init(&wq->cv, "lnxwq");
}

static inline void
wake_up(wait_queue_head_t *wq)
{
	spin_lock(&wq->lock);
	cv_broadcast(&wq->cv);
	spin_unlock(&wq->lock);
}

static inline void
wake_up_all(wait_queue_head_t *wq)
{
	wake_up(wq);
}

static inline void
wake_up_interruptible(wait_queue_head_t *wq)
{
	wake_up(wq);
}

/* Caller already holds wq->lock. */
static inline void
wake_up_all_locked(wait_queue_head_t *wq)
{
	cv_broadcast(&wq->cv);
}

/* XXX amdgpu: no real sleep; succeed only if CONDITION already true. */
#define	wait_event(wq, CONDITION)					\
	do {								\
		if (!(CONDITION)) {					\
			/* XXX spin/sleep not implemented */		\
		}							\
	} while (/*CONSTCOND*/0)

#define	wait_event_interruptible(wq, CONDITION)				\
	({								\
		int __ret = 0;						\
		if (!(CONDITION))					\
			__ret = -ERESTARTSYS; /* XXX no sleep */	\
		__ret;							\
	})

/* Like wait_event_interruptible, but caller already holds wq.lock. */
#define	wait_event_interruptible_locked(wq, CONDITION)			\
	wait_event_interruptible(wq, CONDITION)

/* XXX amdgpu: no real timed sleep; succeed only if CONDITION already true. */
#define	wait_event_interruptible_timeout(wq, CONDITION, TIMEOUT)	\
	({								\
		long __ret = (TIMEOUT);					\
		(void)(wq);						\
		if (!(CONDITION))					\
			__ret = -ERESTARTSYS; /* XXX no sleep */	\
		__ret;							\
	})

#endif  /* _LINUX_WAIT_H_ */
