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



#include <stdio.h>
#include "freebsd.h"


#ifdef __FreeBSD__
/* variables */
char const * stracecall[SYS_getpid + 1] =
{
	NULL,
	"exit",
	"fork",
	"read",
	"write",
	"open",
	"close",
	"wait4",
	NULL,
	"link",
	"unlink",
	NULL,
	"chdir",
	"fchdir",
	"mknod",
	"chmod",
	"chown",
	"break",
	NULL,
	NULL,
	"getpid"
};


/* functions */
/* accessors */
/* platform_get_registers */
void platform_get_registers(pid_t pid, struct user * context)
{
	ptrace(PT_GETREGS, pid, &context->regs, 0);
}


/* platform_get_result */
long platform_get_result(pid_t pid, struct user * context)
{
	struct user c;

	if(context == NULL)
	{
		platform_get_registers(pid, &c);
		context = &c;
	}
#if defined(__amd64__)
	return context->regs.orig_rax;
#elif defined(__i386__)
	return context->regs.orig_eax;
#else
	return -1;
#endif
}


/* platform_get_syscall */
char const * platform_get_syscall(pid_t pid, struct user * context)
{
	const size_t size = sizeof(stracecall) / sizeof(*stracecall);
	struct user c;
	long res;
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
	if(res >= 0 && (size_t)res < size && stracecall[res] != NULL)
		return stracecall[res];
	if(res >= 0)
	{
		snprintf(buf, sizeof(buf), "%s%ld", "syscall_", res);
		return buf;
	}
	return NULL;
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
	fprintf(stderr, "rax: 0x%016lx\n", context->regs.orig_rax);
	fprintf(stderr, "rcx: 0x%016lx\n", context->regs.orig_rcx);
	fprintf(stderr, "rdx: 0x%016lx\n", context->regs.orig_rdx);
	fprintf(stderr, "rbx: 0x%016lx\n", context->regs.orig_rbx);
	fprintf(stderr, "rsi: 0x%016lx\n", context->regs.orig_rsi);
	fprintf(stderr, "rdi: 0x%016lx\n", context->regs.orig_rdi);
# elif defined(__i386__)
	fprintf(stderr, "eax: 0x%08lx\n", context->regs.orig_eax);
	fprintf(stderr, "ecx: 0x%08lx\n", context->regs.orig_ecx);
	fprintf(stderr, "edx: 0x%08lx\n", context->regs.orig_edx);
	fprintf(stderr, "ebx: 0x%08lx\n", context->regs.orig_ebx);
	fprintf(stderr, "esi: 0x%08lx\n", context->regs.orig_esi);
	fprintf(stderr, "edi: 0x%08lx\n", context->regs.orig_edi);
# endif
}
#endif /* __FreeBSD__ */