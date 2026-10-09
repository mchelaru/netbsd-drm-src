/*	$NetBSD: file.h,v 1.7 2021/12/19 10:39:06 riastradh Exp $	*/

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

#ifndef _LINUX_FILE_H_
#define _LINUX_FILE_H_

#include <sys/param.h>
#include <sys/file.h>
#include <sys/filedesc.h>
#include <sys/proc.h>

struct file;

struct fd {
	struct file	*file;
	int		fd_number;
};

static inline struct fd
fdget(int number)
{
	struct fd fd;

	fd.file = fd_getfile(number);
	fd.fd_number = number;

	return fd;
}

static inline void
fdput(struct fd fd)
{

	fd_putfile(fd.fd_number);
}

static inline bool
fd_empty(struct fd f)
{

	return f.file == NULL;
}

static inline struct file *
fd_file(struct fd f)
{

	return f.file;
}

/*
 * Approximate Linux CLASS(fd, var)(fdnum) without requiring cleanup.h.
 * Expands to: struct fd var = fdget(fdnum);
 * Callers that return early must still fdput(var) on NetBSD (no __cleanup).
 */
typedef struct fd class_fd_t;
static inline struct fd
class_fd_constructor(int fdnum)
{

	return fdget(fdnum);
}
static inline void
class_fd_destructor(struct fd *p)
{

	fdput(*p);
}

#ifndef CLASS
#define	CLASS(_name, var)						\
	class_##_name##_t var __attribute__((__cleanup__(class_##_name##_destructor))) = \
	    class_##_name##_constructor
#endif

/* fget translates; fput(fp) doesn't because we have fd_putfile(fd).  */
static inline struct file *
fget(int fd)
{
	return fd_getfile(fd);
}

static inline void
fd_install(int fd, struct file *fp)
{
	fd_affix(curproc, fp, fd);
}

static inline void
fput(struct file *fp)
{
	(void)fp;
	/* XXX: NetBSD callers use fd_putfile(fd); no filp refcount API. */
}

#endif  /* _LINUX_FILE_H_ */
