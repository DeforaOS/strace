/* $Id$ */
/* Copyright (c) 2012-2014 Pierre Pronchery <khorben@defora.org> */
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

# if defined(__FreeBSD__)
#  include <sys/ptrace.h>


/* types */
typedef int ptrace_data_t;
struct user
{
	struct reg regs;
};


/* constants */
#  if defined(__amd64__)
#   define rax		r_rax
#   define rcx		r_rcx
#   define rbx		r_rbx
#   define rdx		r_rdx
#   define rsi		r_rsi
#   define rdi		r_rdi
#   define r10		r_r10
#  elif defined(__i386__)
#   define eax		r_eax
#   define ecx		r_ecx
#   define ebx		r_ebx
#   define edx		r_edx
#   define esi		r_esi
#   define edi		r_edi
#  endif

# endif /* __FreeBSD__ */
#endif /* !STRACE_FREEBSD_H */
