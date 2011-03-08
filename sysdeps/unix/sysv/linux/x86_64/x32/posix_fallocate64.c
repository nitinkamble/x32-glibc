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

#include <fcntl.h>
#include <sysdep.h>

#undef strong_alias
#define strong_alias(symbol, alias)
#define __posix_fallocate64_l64 static internal_fallocate
#include <sysdeps/posix/posix_fallocate64.c>
#undef __posix_fallocate64_l64
 
#undef LOAD_ARGS_2
#define LOAD_ARGS_2(a1, a2)				\
  long long int __arg2 = (long long int) (a2);		\
  LOAD_ARGS_1 (a1)
#undef LOAD_REGS_2
#define LOAD_REGS_2					\
  register long long int _a2 asm ("rsi") = __arg2;	\
  LOAD_REGS_1

#undef LOAD_ARGS_3
#define LOAD_ARGS_3(a1, a2, a3)				\
  long long int __arg3 = (long long int) (a3);		\
  LOAD_ARGS_2 (a1, a2)
#undef LOAD_REGS_3
#define LOAD_REGS_3					\
  register long long int _a3 asm ("rdx") = __arg3;	\
  LOAD_REGS_2

extern int __posix_fallocate64 (int, __off64_t, __off64_t);
libc_hidden_proto(__posix_fallocate64)

/* Reserve storage for the data of the file associated with FD.  */
int
__posix_fallocate64 (int fd, __off64_t offset, __off64_t len)
{
  INTERNAL_SYSCALL_DECL (err);
  int res = INTERNAL_SYSCALL (fallocate, err, 4, fd, 0, offset, len);

  if (! INTERNAL_SYSCALL_ERROR_P (res, err))
    return 0;

  if (INTERNAL_SYSCALL_ERRNO (res, err) != EOPNOTSUPP)
    return INTERNAL_SYSCALL_ERRNO (res, err);

  return internal_fallocate (fd, offset, len);
}

libc_hidden_def (__posix_fallocate64)
weak_alias (__posix_fallocate64, posix_fallocate64)
