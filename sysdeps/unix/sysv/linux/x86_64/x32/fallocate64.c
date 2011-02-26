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
#include <fcntl.h>
#include <sysdep.h>

extern int __fallocate64 (int, int, __off64_t, __off64_t);
libc_hidden_proto(__fallocate64)

/* Reserve storage for the data of the file associated with FD.  */
int
__fallocate64 (int fd, int mode, __off64_t offset, __off64_t len)
{
  return INLINE_SYSCALL (fallocate, 4, fd, mode, offset, len);
}

libc_hidden_def (__fallocate64)
weak_alias (__fallocate64, fallocate64)
