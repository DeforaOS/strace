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



#include <stdio.h>
#include "../platform.h"
#include "linux.h"


#ifdef __linux__

/* variables */
char const * stracecall[] =
{
	"exit",
	"fork",
	"read",
	"write",
	"open",
	"close",
	"waitpid",
	"creat",
	"link",
	"unlink",
	"execve",
	"chdir",
	"time",
	"mknod",
	"chmod",
	"lchown",
	"break",
	"oldstat",
	"lseek",
	"getpid"
};


/* functions */
/* accessors */
/* platform_get_registers */
void platform_get_registers(pid_t pid, struct user * context)
{
	ptrace(PT_GETREGS, pid, NULL, &context->regs);
}


/* platform_get_syscall */
char const * platform_get_syscall(pid_t pid, struct user * context)
{
	const size_t size = sizeof(stracecall) / sizeof(*stracecall);
	struct user c;
	unsigned long res;
	static char buf[32];

	if(context == NULL)
	{
		platform_get_registers(pid, &c);
		context = &c;
	}
#if defined(__amd64__)
	res = context->regs.orig_rax;
	res &= 0xffffffff;
#elif defined(__i386__)
	res = context->regs.orig_eax;
#else
	res = -1;
#endif
	if(res < size && stracecall[res] != NULL)
		return stracecall[res];
	snprintf(buf, sizeof(buf), "%s%lx", "syscall_", res);
	return buf;
}


/* useful */
/* platform_print_registers */
void platform_print_registers(pid_t pid, struct user * context)
{
	struct user c;

	if(context == NULL)
	{
		platform_get_registers(pid, &c);
		context = &c;
	}
# if defined(__amd64__)
	fprintf(stderr, "rax: 0x%016lx\n", regs->orig_rax);
	fprintf(stderr, "rcx: 0x%016lx\n", regs->orig_rcx);
	fprintf(stderr, "rdx: 0x%016lx\n", regs->orig_rdx);
	fprintf(stderr, "rbx: 0x%016lx\n", regs->orig_rbx);
	fprintf(stderr, "rsi: 0x%016lx\n", regs->orig_rsi);
	fprintf(stderr, "rdi: 0x%016lx\n", regs->orig_rdi);
# elif defined(__i386__)
	fprintf(stderr, "eax: 0x%08lx\n", regs->orig_eax);
	fprintf(stderr, "ecx: 0x%08lx\n", regs->orig_ecx);
	fprintf(stderr, "edx: 0x%08lx\n", regs->orig_edx);
	fprintf(stderr, "ebx: 0x%08lx\n", regs->orig_ebx);
	fprintf(stderr, "esi: 0x%08lx\n", regs->orig_esi);
	fprintf(stderr, "edi: 0x%08lx\n", regs->orig_edi);
# endif
}

#endif /* __linux__ */
