/* Definition for thread-local data handling.  nptl/x32 version.
   Copyright (C) 2011 Free Software Foundation, Inc.
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

#ifndef _X32_TLS_H
#define _X32_TLS_H	1

#include <sysdeps/x86_64/tls.h>

#ifndef __ASSEMBLER__

# ifdef PIC
#  define TLS_EBX_LOAD	"xchgl %1, %%ebx\n"
#  define TLS_EBX_REG	"D"
# else
#  define TLS_EBX_LOAD
#  define TLS_EBX_REG	"b"
# endif

/* Code to initially initialize the thread pointer.  This might need
   special attention since 'errno' is not yet available and if the
   operation can cause a failure 'errno' must not be touched.

   We have to make the syscall for both uses of the macro since the
   address might be (and probably is) different.  */
# undef TLS_INIT_TP
# define TLS_INIT_TP(thrdescr, secondcall) \
  ({ void *_thrdescr = (thrdescr);					      \
     tcbhead_t *_head = _thrdescr;					      \
     int _result;							      \
									      \
     _head->tcb = _thrdescr;						      \
     /* For now the thread descriptor is at the same address.  */	      \
     _head->self = _thrdescr;						      \
									      \
     /* It is a simple syscall to set the %fs value for the thread.  */	      \
     asm volatile (TLS_EBX_LOAD						      \
		   "int $0x80\n\t"					      \
		   TLS_EBX_LOAD						      \
		   : "=a" (_result)					      \
		   : "0" ((unsigned long int) __NR_arch_prctl),		      \
		     TLS_EBX_REG ((unsigned long int) ARCH_SET_FS),	      \
		     "c" (_thrdescr)					      \
		   : "memory", "cc");					      \
									      \
    _result ? "cannot set %fs base address for thread-local storage" : 0;     \
  })


/* Return the thread descriptor for the current thread.

   The contained asm must *not* be marked volatile since otherwise
   assignments like
	pthread_descr self = thread_self();
   do not get optimized away.  */
# undef THREAD_SELF
# define THREAD_SELF \
  ({ struct pthread *__self;						      \
     asm ("movl %%fs:%c1,%0" : "=r" (__self)				      \
	  : "i" (offsetof (struct pthread, header.self)));	 	      \
     __self;})

# undef CALL_THREAD_FCT
# define CALL_THREAD_FCT(descr) \
  ({ void *__res;							      \
     asm volatile ("movl %%fs:%P2, %%edi\n\t"				      \
		   "callq *%%fs:%P1"					      \
		   : "=a" (__res)					      \
		   : "i" (offsetof (struct pthread, start_routine)),	      \
		     "i" (offsetof (struct pthread, arg))		      \
		   : "di", "si", "cx", "dx", "r8", "r9", "r10", "r11",	      \
		     "memory", "cc");					      \
     __res; })

#endif /* __ASSEMBLER__ */

#endif	/* x32/tls.h */
