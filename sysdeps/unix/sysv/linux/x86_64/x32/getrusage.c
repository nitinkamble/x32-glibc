/* Copyright (C) 2011
   Free Software Foundation, Inc.
   This file is part of the GNU C Library.

   The GNU C Library is free software; you can redistribute it and/or
   modify it under the terms of the GNU Lesser General Public
   License as published by the Free Software Foundation; either
   version 2.1 of the License, or (at your option) any later version.

   The GNU C Library is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
   Lesser General Public License for more details.

   You should have received a copy of the GNU Lesser General Public
   License along with the GNU C Library; if not, write to the Free
   Software Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA
   02111-1307 USA.  */

#include <sys/resource.h>
#include <errno.h>
#include "kernel_resource.h"

/* Return resource usage information on process indicated by WHO
   and put it in *USAGE.  Returns 0 for success, -1 for failure.  */
int
__getrusage (__rusage_who_t who, struct rusage *usage)
{
  struct kernel_rusage k;
  pid_t ret = INLINE_SYSCALL (getrusage, 2, who, &k);
  if (ret != -1)
    copy_from_kernel_rusage (usage, &k);
  return ret;
}
weak_alias (__getrusage, getrusage)
