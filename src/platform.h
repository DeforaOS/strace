/* $Id$ */
/* Copyright (c) 2014-2020 Pierre Pronchery <khorben@defora.org> */
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



#ifndef STRACE_PLATFORM_H
# define STRACE_PLATFORM_H

# include <sys/types.h>
# include "platform/freebsd.h"
# include "platform/linux.h"
# include "platform/netbsd.h"


/* platform */
/* public */
/* prototypes */
/* accessors */
void platform_get_registers(pid_t pid, struct user * context);
long platform_get_result(pid_t pid, struct user * context);
char const * platform_get_syscall(pid_t pid, struct user * context);

/* useful */
void platform_print_registers(pid_t pid, struct user * context);

#endif /* !STRACE_PLATFORM_H */
