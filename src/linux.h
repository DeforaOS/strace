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



#ifndef STRACE_LINUX_H
# define STRACE_LINUX_H

# ifdef __linux__
#  include <sys/user.h>
#  include <asm/unistd.h>


/* types */
typedef void * ptrace_data_t;


/* variables */
extern char const * stracecall[__NR_getpid];
# endif /* __linux__ */

#endif /* !STRACE_LINUX_H */
