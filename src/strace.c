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



#include <sys/types.h>
#include <sys/ptrace.h>
#include <sys/wait.h>
#include <unistd.h>
#include <stdio.h>
#include <signal.h>
#include "freebsd.h"
#include "linux.h"
#include "netbsd.h"
#include "strace.h"
#include "../config.h"

#ifndef PROGNAME
# define PROGNAME PACKAGE
#endif


/* strace */
/* private */
/* prototypes */
static int _strace_error(char const * message, int ret);
static int _strace_parent(pid_t pid);

static int _strace_handle(pid_t pid, int res);

#ifdef DEBUG
static void _strace_regs_print(struct reg * regs);
#endif


/* public */
/* functions */
/* strace */
int strace(char * argv[])
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


/* private */
/* functions */
/* strace_error */
static int _strace_error(char const * message, int ret)
{
	fputs(PROGNAME ": ", stderr);
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

#ifdef __linux__
	ptrace(PT_GETREGS, pid, NULL, &context.regs);
#else
	ptrace(PT_GETREGS, pid, &context.regs, 0);
#endif
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

#ifdef __linux__
	ptrace(PT_GETREGS, pid, NULL, &context.regs);
#else
	ptrace(PT_GETREGS, pid, &context.regs, 0);
#endif
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
# if defined(__amd64__)
	fprintf(stderr, "rax: 0x%016lx\n", regs->orig_rax);
	fprintf(stderr, "rcx: 0x%016lx\n", regs->orig_rcx);
	fprintf(stderr, "rdx: 0x%016lx\n", regs->orig_rdx);
	fprintf(stderr, "rbx: 0x%016lx\n", regs->orig_rbx);
# endif
}
#endif
