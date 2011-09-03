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

#include <sys/types.h>
#include <sys/wait.h>
#include <errno.h>
#include "kernel_resource.h"

pid_t
__wait4 (__pid_t pid, __WAIT_STATUS_DEFN stat_loc, int options, 
	 struct rusage *usage)
{
  struct kernel_rusage *kusage;
  pid_t ret;

  if (usage)
    {
      struct kernel_rusage k;
      kusage = &k;
    }
  else
    kusage = NULL;

  ret = INLINE_SYSCALL (wait4, 4, pid, stat_loc, options, kusage);

  if (ret != -1 && kusage)
    copy_from_kernel_rusage (usage, kusage);

  return ret;
}

weak_alias (__wait4, wait4)
