/* $Id$ */
/* Copyright (c) 2012 Pierre Pronchery <khorben@defora.org> */
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



#ifndef STRACE_FREEBSD_H
# define STRACE_FREEBSD_H

# ifdef __FreeBSD__
#  include <sys/ptrace.h>


/* types */
typedef int ptrace_data_t;
struct user
{
	struct reg regs;
};


/* constants */
#  if defined(__amd64__)
#   define orig_eax		r_rax
#   define orig_ebx		r_rbx
#   define orig_ecx		r_rcx
#   define orig_edx		r_rdx
#  elif defined(__i386__)
#   define orig_eax		r_eax
#  endif


/* variables */
extern char const * stracecall[SYS_getpid + 1];
# endif /* __FreeBSD__ */

#endif /* !STRACE_FREEBSD_H */
