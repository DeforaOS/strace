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
static int _strace_error(char const * message, int ret);
static int _strace_parent(pid_t pid);

static int _strace(char * argv[])
{
	pid_t pid;

	if((pid = fork()) == -1)
		return _strace_error("fork", 1);
	if(pid == 0)
	{
		ptrace(PT_TRACE_ME, -1, NULL, (ptrace_data_t)NULL);
		execvp(argv[0], argv);
		return _strace_error(argv[0], 1);
	}
	return _strace_parent(pid);
}

static int _strace_error(char const * message, int ret)
{
	fputs("strace: ", stderr);
	perror(message);
	return ret;
}

static int _handle(pid_t pid, int res);
static int _strace_parent(pid_t pid)
{
	int status;

	for(;;)
	{
		waitpid(pid, &status, 0);
		if(_handle(pid, status) != 0)
			return 0;
	}
}

static int _handle(pid_t pid, int status)
{
	struct user context;
	int size = sizeof(stracecall) / sizeof(*stracecall);

	if(!WIFSTOPPED(status))
		return -1;
	switch(WSTOPSIG(status))
	{
		case SIGTRAP:
			ptrace(PT_GETREGS, pid, NULL,
					(ptrace_data_t)&context);
			if(size >= context.regs.orig_eax)
				fprintf(stderr, "%s();\n", stracecall[
						context.regs.orig_eax - 1]);
			else
				fprintf(stderr, "%ld\n", context.regs.orig_eax);
			ptrace(PT_SYSCALL, pid, NULL, (ptrace_data_t)NULL);
			wait(0);
			ptrace(PT_SYSCALL, pid, NULL, (ptrace_data_t)NULL);
			break;
		default:
			ptrace(PT_CONTINUE, pid, NULL, WSTOPSIG(status));
			break;
	}
	return 0;
}


/* usage */
static int _usage(void)
{
	fputs("Usage: strace program [argument...]\n", stderr);
	return 1;
}


/* main */
int main(int argc, char * argv[])
{
	if(argc <= 1)
		return _usage();
	return (_strace(&argv[1]) == 0) ? 0 : 2;
}
