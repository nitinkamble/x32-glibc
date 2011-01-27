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

#include <sysdeps/x86_64/pthreaddef.h>

#undef CURRENT_STACK_FRAME
/* Location of current stack frame.  The frame pointer is not usable.  */
#define CURRENT_STACK_FRAME \
  ({ char *frame; asm ("movl %%esp, %0" : "=r" (frame)); frame; })

/* XXX Until we have a better place keep the definitions here.  */

/* While there is no such syscall.  */
#undef __exit_thread_inline
#ifdef PIC
# define __exit_thread_inline(val) 					\
  asm volatile ("xchgl %1, %%ebx\n\t"					\
		"int $0x80\n\t"						\
		"xchgl %%ebx, %1" :: "a" (__NR_exit), "r" (val))
#else
# define __exit_thread_inline(val)					\
  asm volatile ("int $0x80" :: "a" (__NR_exit), "b" (val))
#endif
