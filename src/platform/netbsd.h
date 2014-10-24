/* $Id$ */
/* Copyright (c) 2009-2014 Pierre Pronchery <khorben@defora.org> */
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

# if defined(__NetBSD__)
#  include <sys/types.h>
#  include <sys/ptrace.h>
#  include <sys/syscall.h>
#  ifndef __DeforaOS__
#   include <machine/reg.h>
#  endif


/* types */
typedef int ptrace_data_t;
struct user
{
	struct reg regs;
};


/* constants */
#  if defined(__amd64__)
#   define rax		regs[_REG_RAX]
#   define rcx		regs[_REG_RCX]
#   define rbx		regs[_REG_RBX]
#   define rdx		regs[_REG_RDX]
#   define rsi		regs[_REG_RSI]
#   define rdi		regs[_REG_RDI]
#   define r10		regs[_REG_R10]
#  elif defined(__i386__)
#   define eax		r_eax
#   define ecx		r_ecx
#   define ebx		r_ebx
#   define edx		r_edx
#   define esi		r_esi
#   define edi		r_edi
#  endif

# endif /* __NetBSD__ */
#endif /* !STRACE_NETBSD_H */
