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
#include "platform.h"
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
	char const * syscall;

	platform_get_registers(pid, &context);
#ifdef DEBUG
	platform_print_registers(pid, &context);
#endif
	if((syscall = platform_get_syscall(pid, &context)) == NULL)
		syscall = "syscall";
	fprintf(stderr, "%s()", syscall);
}

static void _handle_trap_after(pid_t pid)
{
	struct user context;
	long res;

	platform_get_registers(pid, &context);
#ifdef DEBUG
	platform_print_registers(pid, &context);
#endif
	res = platform_get_result(pid, &context);
	fprintf(stderr, " => %ld\n", res);
}
