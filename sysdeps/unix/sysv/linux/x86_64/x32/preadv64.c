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
#include <stddef.h>
#include <sys/param.h>
#include <sys/uio.h>
#include <sysdep-cancel.h>
#include <sys/syscall.h>

#include <kernel_uio.h>

extern ssize_t __preadv64 (int, const struct iovec *, int, off64_t);
libc_hidden_proto(__preadv64)

ssize_t
__preadv64 (int fd, const struct iovec *vector, int count, off64_t offset)
{
  ssize_t result;
  struct kernel_iovec kvector;

  kvector.iov_base = (uint64_t) (uintptr_t) vector->iov_base;
  kvector.iov_len= vector->iov_len;

  if (SINGLE_THREAD_P)
    result = INLINE_SYSCALL (preadv, 4, fd, &kvector, count, offset);
  else
    {
      int oldtype = LIBC_CANCEL_ASYNC ();

      result = INLINE_SYSCALL (preadv, 4, fd, &kvector, count, offset);

      LIBC_CANCEL_RESET (oldtype);
    }
  return result;
}

libc_hidden_def (__preadv64)
weak_alias (__preadv64, preadv64)
