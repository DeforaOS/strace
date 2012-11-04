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



#include <sys/types.h>
#include <sys/ptrace.h>
#include <sys/wait.h>
#include <unistd.h>
#include <stdio.h>
#include <signal.h>
#include "freebsd.h"
#include "linux.h"
#include "netbsd.h"


/* strace */
/* private */
/* prototypes */
static int _strace(char * argv[]);

static int _strace_error(char const * message, int ret);
static int _strace_parent(pid_t pid);

static int _strace_handle(pid_t pid, int res);

#ifdef DEBUG
static void _strace_regs_print(struct reg * regs);
#endif


/* functions */
/* strace */
static int _strace(char * argv[])
{
	pid_t pid;

#ifdef DEBUG
	fprintf(stderr, "DEBUG: %s(\"%s\")\n", __func__, argv[0]);
#endif
	if((pid = fork()) == -1)
		return _strace_error("fork", 1);
	if(pid == 0)
	{
		/* child */
		ptrace(PT_TRACE_ME, -1, NULL, (ptrace_data_t)0);
		execvp(argv[0], argv);
		return _strace_error(argv[0], 1);
	}
	return _strace_parent(pid);
}


/* strace_error */
static int _strace_error(char const * message, int ret)
{
	fputs("strace: ", stderr);
	perror(message);
	return ret;
}


/* strace_parent */
static int _strace_parent(pid_t pid)
{
	int status;

	for(;;)
	{
		if(waitpid(pid, &status, 0) == -1)
			return -_strace_error("waitpid", 1);
		if(_strace_handle(pid, status) != 0)
			return 0;
	}
}


/* strace_handle */
static void _handle_trap_before(pid_t pid);
static void _handle_trap_after(pid_t pid);

static int _strace_handle(pid_t pid, int status)
{
	if(!WIFSTOPPED(status))
		return -1;
	switch(WSTOPSIG(status))
	{
		case SIGTRAP:
			/* examine the system call */
			_handle_trap_before(pid);
			/* execute the system call */
			ptrace(PT_SYSCALL, pid, (caddr_t)1, (ptrace_data_t)0);
			/* examine the return value and data */
			_handle_trap_after(pid);
			/* wait until the next syscall */
			ptrace(PT_SYSCALL, pid, (caddr_t)1, (ptrace_data_t)0);
			break;
		default:
			ptrace(PT_CONTINUE, pid, (caddr_t)1, WSTOPSIG(status));
			break;
	}
	return 0;
}

static void _handle_trap_before(pid_t pid)
{
	struct user context;
	int size = sizeof(stracecall) / sizeof(*stracecall);

	ptrace(PT_GETREGS, pid, &context, 0);
#ifdef DEBUG
	_strace_regs_print(&context.regs);
#endif
#if defined(__amd64__)
	if(context.regs.orig_rax >= 0 && context.regs.orig_rax < size)
		fprintf(stderr, "%s();", stracecall[context.regs.orig_rax]);
	else
		fprintf(stderr, "%ld", context.regs.orig_rax);
#elif defined(__i386__)
	if(context.regs.orig_eax >= 0 && context.regs.orig_eax < size)
		fprintf(stderr, "%s();", stracecall[context.regs.orig_eax]);
	else
		fprintf(stderr, "%ld", context.regs.orig_eax);
#endif
}

static void _handle_trap_after(pid_t pid)
{
	struct user context;

	ptrace(PT_GETREGS, pid, &context, 0);
#ifdef DEBUG
	_strace_regs_print(&context.regs);
#endif
#if defined(__amd64__)
	fprintf(stderr, " => %ld\n", context.regs.orig_rax);
#elif defined(__i386__)
	fprintf(stderr, " => %ld\n", context.regs.orig_eax);
#endif
}


#ifdef DEBUG
/* strace_regs_print */
static void _strace_regs_print(struct reg * regs)
{
#if defined(__amd64__)
	fprintf(stderr, "rax: 0x%016lx\n", regs->orig_rax);
#endif
}
#endif


/* usage */
static int _usage(void)
{
	fputs("Usage: strace program [argument...]\n", stderr);
	return 1;
}


/* main */
int main(int argc, char * argv[])
{
	int o;

	while((o = getopt(argc, argv, "")) != -1)
		switch(o)
		{
			default:
				return _usage();
		}
	if(argc - optind < 1)
		return _usage();
	return (_strace(&argv[optind]) == 0) ? 0 : 2;
}
