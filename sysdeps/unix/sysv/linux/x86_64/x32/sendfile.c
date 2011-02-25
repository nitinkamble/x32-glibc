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

#include <sys/types.h>
#include <sys/sendfile.h>

extern ssize_t __libc_sendfile64 (int, int, off64_t *, size_t);

/* Sign extend offset to 64bit.  */

ssize_t sendfile (int out_fd, int in_fd, off_t *offset, size_t count)
{
  if (offset)
    {
      off64_t offset64 = *offset;
      ssize_t ret = __libc_sendfile64 (out_fd, in_fd, &offset64, count);
      *offset = offset64;
      return ret;
    }
 
  return __libc_sendfile64 (out_fd, in_fd, (off64_t *) offset, count);
}
