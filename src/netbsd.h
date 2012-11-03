/* $Id$ */
/* Copyright (c) 2009-2012 Pierre Pronchery <khorben@defora.org> */
/* This file is part of DeforaOS Devel strace */
/* This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, version 3 of the License.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>. */



#ifndef STRACE_NETBSD_H
# define STRACE_NETBSD_H

# ifdef __NetBSD__
#  include <sys/types.h>
#  include <sys/ptrace.h>
#  include <sys/syscall.h>
#  include <machine/reg.h>


/* types */
typedef long ptrace_data_t; /* XXX really is int */
struct user
{
	struct reg regs;
};


/* constants */
#  if defined(__amd64__)
#   define _REG32_EAX		11 /* XXX or #define _KERNEL */
#   define orig_eax		regs[_REG32_EAX]
#  elif defined(__i386__)
#   define orig_eax		r_eax
#  endif


/* variables */
extern char const * stracecall[SYS_getpid + 1];
# endif /* __NetBSD__ */

#endif /* !STRACE_NETBSD_H */
