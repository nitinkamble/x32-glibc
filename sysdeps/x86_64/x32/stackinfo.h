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

/* This file contains a bit of information about the stack allocation
   of the processor.  */

#ifndef _X32_STACKINFO_H
#define _X32_STACKINFO_H	1

#include <sysdeps/x86_64/stackinfo.h>

#undef stackinfo_get_sp
#define stackinfo_get_sp() \
  ({ void *p__; asm volatile ("mov %%esp, %0" : "=r" (p__)); p__; })

#undef stackinfo_sub_sp
#define stackinfo_sub_sp(ptr) \
  ({ ptrdiff_t d__;						\
     asm volatile ("sub %%esp, %0" : "=r" (d__) : "0" (ptr));	\
     d__; })

#endif	/* stackinfo.h */
