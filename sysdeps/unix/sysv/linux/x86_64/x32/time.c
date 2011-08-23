/* Copyright (C) 2011 Free Software Foundation, Inc.
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

#include <errno.h>
#include <sysdep.h>
#include <time.h>

#ifdef SHARED
# include <dl-vdso.h>

void *time_ifunc (void) __asm__ ("__GI_time");

static time_t
time_syscall (time_t *t)
{
  return INLINE_SYSCALL (time, 1, t);
}

void *
time_ifunc (void)
{
  PREPARE_VERSION (linux26, "LINUX_2.6", 61765110);

  return _dl_vdso_vsym ("time", &linux26) ?: (void *) time_syscall;
}
__asm (".type __GI_time, %gnu_indirect_function");
#else
time_t
time (time_t *t)
{
  return INLINE_SYSCALL (time, 1, t);
}
#endif

libc_hidden_def (time)
