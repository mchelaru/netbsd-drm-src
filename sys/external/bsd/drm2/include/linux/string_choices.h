/*	$NetBSD$	*/

/*-
 * Copyright (c) 2026 The NetBSD Foundation, Inc.
 * All rights reserved.
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

#ifndef _LINUX_STRING_CHOICES_H_
#define _LINUX_STRING_CHOICES_H_

#include <sys/types.h>
#include <sys/stdbool.h>

static inline const char *
str_yes_no(bool v)
{
	return v ? "yes" : "no";
}

#define str_no_yes(v)		str_yes_no(!(v))

static inline const char *
str_on_off(bool v)
{
	return v ? "on" : "off";
}

#define str_off_on(v)		str_on_off(!(v))

static inline const char *
str_enable_disable(bool v)
{
	return v ? "enable" : "disable";
}

#define str_disable_enable(v)	str_enable_disable(!(v))

static inline const char *
str_enabled_disabled(bool v)
{
	return v ? "enabled" : "disabled";
}

#define str_disabled_enabled(v)	str_enabled_disabled(!(v))

static inline const char *
str_read_write(bool v)
{
	return v ? "read" : "write";
}

#define str_write_read(v)	str_read_write(!(v))

static inline const char *
str_true_false(bool v)
{
	return v ? "true" : "false";
}

#define str_false_true(v)	str_true_false(!(v))

static inline const char *
str_up_down(bool v)
{
	return v ? "up" : "down";
}

#define str_down_up(v)		str_up_down(!(v))

static inline const char *
str_high_low(bool v)
{
	return v ? "high" : "low";
}

#define str_low_high(v)		str_high_low(!(v))

#endif /* _LINUX_STRING_CHOICES_H_ */
