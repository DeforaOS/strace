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



#include <stddef.h>
#include "netbsd.h"


#ifdef __NetBSD__
/* variables */
char const * stracecall[] =
{
	"syscall",
	"exit",
	"fork",
	"read",
	"write",
	"open",
	"close",
	"wait4",
	"oldcreat",
	"link",
	"unlink",
	"oldexecv",
	"chdir",
	"fchdir",
	"mknod",
	"chmod",
	"chown",
	"break",
	NULL,
	"oldlseek",
	"getpid"
};
#endif /* __NetBSD__ */
