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

#ifndef _X32_LOWLEVELLOCK_H
#define _X32_LOWLEVELLOCK_H	1

#include <sysdeps/unix/sysv/linux/x86_64/lowlevellock.h>

#ifndef __ASSEMBLER__

#ifdef PIC
# define LLL_EBX_LOAD	"xchgl %2, %%ebx\n"
# define LLL_EBX_REG	"D"
#else
# define LLL_EBX_LOAD
# define LLL_EBX_REG	"b"
#endif

#undef lll_futex_timed_wait
#define lll_futex_timed_wait(futex, val, timeout, private) \
  ({									      \
    int __status;							      \
    register __typeof (val) _val asm ("edx") = (val);			      \
    __asm __volatile (LLL_EBX_LOAD					      \
		      "int $0x80\n\t"					      \
		      LLL_EBX_LOAD					      \
		      : "=a" (__status)					      \
		      : "0" (SYS_futex), LLL_EBX_REG (futex), "S" (timeout),  \
			"c" (__lll_private_flag (FUTEX_WAIT, private)),	      \
			"d" (_val), "i" (offsetof (tcbhead_t, sysinfo))	      \
		      : "memory");					      \
    __status;								      \
  })

#undef lll_futex_wake
#define lll_futex_wake(futex, nr, private) \
  do {									      \
    int __ignore;							      \
    register __typeof (nr) _nr asm ("edx") = (nr);			      \
    __asm __volatile (LLL_EBX_LOAD					      \
		      "int $0x80\n\t"					      \
		      LLL_EBX_LOAD					      \
		      : "=a" (__ignore)					      \
		      : "0" (SYS_futex), LLL_EBX_REG (futex),		      \
			"c" (__lll_private_flag (FUTEX_WAKE, private)),	      \
			"d" (_nr),					      \
			"i" (0) /* phony, to align next arg's number */,      \
			"i" (offsetof (tcbhead_t, sysinfo)));		      \
  } while (0)

#undef lll_lock
#define lll_lock(futex, private) \
  (void)								      \
    ({ int ignore1, ignore2, ignore3;					      \
       if (__builtin_constant_p (private) && (private) == LLL_PRIVATE)	      \
	 __asm __volatile (__lll_lock_asm_start				      \
			   ".subsection 1\n\t"				      \
			   ".type _L_lock_%=, @function\n"		      \
			   "_L_lock_%=:\n"				      \
			   "1:\tleal %2, %%edi\n"			      \
			   "2:\tsubl $128, %%esp\n"			      \
			   "3:\tcallq __lll_lock_wait_private\n"	      \
			   "4:\taddl $128, %%esp\n"			      \
			   "5:\tjmp 24f\n"				      \
			   "6:\t.size _L_lock_%=, 6b-1b\n\t"		      \
			   ".previous\n"				      \
			   LLL_STUB_UNWIND_INFO_5			      \
			   "24:"					      \
			   : "=S" (ignore1), "=&D" (ignore2), "=m" (futex),   \
			     "=a" (ignore3)				      \
			   : "0" (1), "m" (futex), "3" (0)		      \
			   : "cx", "r11", "cc", "memory");		      \
       else								      \
	 __asm __volatile (__lll_lock_asm_start				      \
			   ".subsection 1\n\t"				      \
			   ".type _L_lock_%=, @function\n"		      \
			   "_L_lock_%=:\n"				      \
			   "1:\tleal %2, %%edi\n"			      \
			   "2:\tsubl $128, %%esp\n"			      \
			   "3:\tcallq __lll_lock_wait\n"		      \
			   "4:\taddl $128, %%esp\n"			      \
			   "5:\tjmp 24f\n"				      \
			   "6:\t.size _L_lock_%=, 6b-1b\n\t"		      \
			   ".previous\n"				      \
			   LLL_STUB_UNWIND_INFO_5			      \
			   "24:"					      \
			   : "=S" (ignore1), "=D" (ignore2), "=m" (futex),    \
			     "=a" (ignore3)				      \
			   : "1" (1), "m" (futex), "3" (0), "0" (private)     \
			   : "cx", "r11", "cc", "memory");		      \
    })									      \

#undef lll_robust_lock
#define lll_robust_lock(futex, id, private) \
  ({ int result, ignore1, ignore2;					      \
    __asm __volatile (LOCK_INSTR "cmpxchgl %4, %2\n\t"			      \
		      "jnz 1f\n\t"					      \
		      ".subsection 1\n\t"				      \
		      ".type _L_robust_lock_%=, @function\n"		      \
		      "_L_robust_lock_%=:\n"				      \
		      "1:\tleal %2, %%edi\n"				      \
		      "2:\tsubl $128, %%esp\n"				      \
		      "3:\tcallq __lll_robust_lock_wait\n"		      \
		      "4:\taddl $128, %%esp\n"				      \
		      "5:\tjmp 24f\n"					      \
		      "6:\t.size _L_robust_lock_%=, 6b-1b\n\t"		      \
		      ".previous\n"					      \
		      LLL_STUB_UNWIND_INFO_5				      \
		      "24:"						      \
		      : "=S" (ignore1), "=D" (ignore2), "=m" (futex),	      \
			"=a" (result)					      \
		      : "1" (id), "m" (futex), "3" (0), "0" (private)	      \
		      : "cx", "r11", "cc", "memory");			      \
    result; })

#undef lll_cond_lock
#define lll_cond_lock(futex, private) \
  (void)								      \
    ({ int ignore1, ignore2, ignore3;					      \
       __asm __volatile (LOCK_INSTR "cmpxchgl %4, %2\n\t"		      \
			 "jnz 1f\n\t"					      \
			 ".subsection 1\n\t"				      \
			 ".type _L_cond_lock_%=, @function\n"		      \
			 "_L_cond_lock_%=:\n"				      \
			 "1:\tleal %2, %%edi\n"				      \
			 "2:\tsubl $128, %%esp\n"			      \
			 "3:\tcallq __lll_lock_wait\n"			      \
			 "4:\taddl $128, %%esp\n"			      \
			 "5:\tjmp 24f\n"				      \
			 "6:\t.size _L_cond_lock_%=, 6b-1b\n\t"		      \
			 ".previous\n"					      \
			 LLL_STUB_UNWIND_INFO_5				      \
			 "24:"						      \
			 : "=S" (ignore1), "=D" (ignore2), "=m" (futex),      \
			   "=a" (ignore3)				      \
			 : "1" (2), "m" (futex), "3" (0), "0" (private)	      \
			 : "cx", "r11", "cc", "memory");		      \
    })

#undef lll_robust_cond_lock
#define lll_robust_cond_lock(futex, id, private) \
  ({ int result, ignore1, ignore2;					      \
    __asm __volatile (LOCK_INSTR "cmpxchgl %4, %2\n\t"			      \
		      "jnz 1f\n\t"					      \
		      ".subsection 1\n\t"				      \
		      ".type _L_robust_cond_lock_%=, @function\n"	      \
		      "_L_robust_cond_lock_%=:\n"			      \
		      "1:\tleal %2, %%edi\n"				      \
		      "2:\tsubl $128, %%esp\n"				      \
		      "3:\tcallq __lll_robust_lock_wait\n"		      \
		      "4:\taddl $128, %%esp\n"				      \
		      "5:\tjmp 24f\n"					      \
		      "6:\t.size _L_robust_cond_lock_%=, 6b-1b\n\t"	      \
		      ".previous\n"					      \
		      LLL_STUB_UNWIND_INFO_5				      \
		      "24:"						      \
		      : "=S" (ignore1), "=D" (ignore2), "=m" (futex),	      \
			"=a" (result)					      \
		      : "1" (id | FUTEX_WAITERS), "m" (futex), "3" (0),	      \
			"0" (private)					      \
		      : "cx", "r11", "cc", "memory");			      \
    result; })

#undef lll_timedlock
#define lll_timedlock(futex, timeout, private) \
  ({ int result, ignore1, ignore2, ignore3;				      \
     __asm __volatile (LOCK_INSTR "cmpxchgl %1, %4\n\t"			      \
		       "jnz 1f\n\t"					      \
		       ".subsection 1\n\t"				      \
		       ".type _L_timedlock_%=, @function\n"		      \
		       "_L_timedlock_%=:\n"				      \
		       "1:\tleal %4, %%edi\n"				      \
		       "0:\tmovl %8, %%edx\n"				      \
		       "2:\tsubl $128, %%esp\n"				      \
		       "3:\tcallq __lll_timedlock_wait\n"		      \
		       "4:\taddl $128, %%esp\n"				      \
		       "5:\tjmp 24f\n"					      \
		       "6:\t.size _L_timedlock_%=, 6b-1b\n\t"		      \
		       ".previous\n"					      \
		       LLL_STUB_UNWIND_INFO_6				      \
		       "24:"						      \
		       : "=a" (result), "=D" (ignore1), "=S" (ignore2),	      \
			 "=&d" (ignore3), "=m" (futex)			      \
		       : "0" (0), "1" (1), "m" (futex), "m" (timeout),	      \
			 "2" (private)					      \
		       : "memory", "cx", "cc", "r10", "r11");		      \
     result; })

#undef lll_robust_timedlock
#define lll_robust_timedlock(futex, timeout, id, private) \
  ({ int result, ignore1, ignore2, ignore3;				      \
     __asm __volatile (LOCK_INSTR "cmpxchgl %1, %4\n\t"			      \
		       "jnz 1f\n\t"					      \
		       ".subsection 1\n\t"				      \
		       ".type _L_robust_timedlock_%=, @function\n"	      \
		       "_L_robust_timedlock_%=:\n"			      \
		       "1:\tleal %4, %%edi\n"				      \
		       "0:\tmovl %8, %%edx\n"				      \
		       "2:\tsubl $128, %%esp\n"				      \
		       "3:\tcallq __lll_robust_timedlock_wait\n"	      \
		       "4:\taddl $128, %%esp\n"				      \
		       "5:\tjmp 24f\n"					      \
		       "6:\t.size _L_robust_timedlock_%=, 6b-1b\n\t"	      \
		       ".previous\n"					      \
		       LLL_STUB_UNWIND_INFO_6				      \
		       "24:"						      \
		       : "=a" (result), "=D" (ignore1), "=S" (ignore2),       \
			 "=&d" (ignore3), "=m" (futex)			      \
		       : "0" (0), "1" (id), "m" (futex), "m" (timeout),	      \
			 "2" (private)					      \
		       : "memory", "cx", "cc", "r10", "r11");		      \
     result; })

#undef lll_unlock
#define lll_unlock(futex, private) \
  (void)								      \
    ({ int ignore;							      \
       if (__builtin_constant_p (private) && (private) == LLL_PRIVATE)	      \
	 __asm __volatile (__lll_unlock_asm_start			      \
			   ".subsection 1\n\t"				      \
			   ".type _L_unlock_%=, @function\n"		      \
			   "_L_unlock_%=:\n"				      \
			   "1:\tleal %0, %%edi\n"			      \
			   "2:\tsubl $128, %%esp\n"			      \
			   "3:\tcallq __lll_unlock_wake_private\n"	      \
			   "4:\taddl $128, %%esp\n"			      \
			   "5:\tjmp 24f\n"				      \
			   "6:\t.size _L_unlock_%=, 6b-1b\n\t"		      \
			   ".previous\n"				      \
			   LLL_STUB_UNWIND_INFO_5			      \
			   "24:"					      \
			   : "=m" (futex), "=&D" (ignore)		      \
			   : "m" (futex)				      \
			   : "ax", "cx", "r11", "cc", "memory");	      \
       else								      \
	 __asm __volatile (__lll_unlock_asm_start			      \
			   ".subsection 1\n\t"				      \
			   ".type _L_unlock_%=, @function\n"		      \
			   "_L_unlock_%=:\n"				      \
			   "1:\tleal %0, %%edi\n"			      \
			   "2:\tsubl $128, %%esp\n"			      \
			   "3:\tcallq __lll_unlock_wake\n"		      \
			   "4:\taddl $128, %%esp\n"			      \
			   "5:\tjmp 24f\n"				      \
			   "6:\t.size _L_unlock_%=, 6b-1b\n\t"		      \
			   ".previous\n"				      \
			   LLL_STUB_UNWIND_INFO_5			      \
			   "24:"					      \
			   : "=m" (futex), "=&D" (ignore)		      \
			   : "m" (futex), "S" (private)			      \
			   : "ax", "cx", "r11", "cc", "memory");	      \
    })

#undef lll_robust_unlock
#define lll_robust_unlock(futex, private) \
  do									      \
    {									      \
      int ignore;							      \
      __asm __volatile (LOCK_INSTR "andl %2, %0\n\t"			      \
			"jne 1f\n\t"					      \
			".subsection 1\n\t"				      \
			".type _L_robust_unlock_%=, @function\n"	      \
			"_L_robust_unlock_%=:\n"			      \
			"1:\tleal %0, %%edi\n"				      \
			"2:\tsubl $128, %%esp\n"			      \
			"3:\tcallq __lll_unlock_wake\n"			      \
			"4:\taddl $128, %%esp\n"			      \
			"5:\tjmp 24f\n"					      \
			"6:\t.size _L_robust_unlock_%=, 6b-1b\n\t"	      \
			".previous\n"					      \
			LLL_STUB_UNWIND_INFO_5				      \
			"24:"						      \
			: "=m" (futex), "=&D" (ignore)			      \
			: "i" (FUTEX_WAITERS), "m" (futex),		      \
			  "S" (private)					      \
			: "ax", "cx", "r11", "cc", "memory");		      \
    }									      \
  while (0)

#undef lll_robust_dead
#define lll_robust_dead(futex, private) \
  (void)								      \
    ({ int __ignore;							      \
       register int _nr asm ("edx") = 1;				      \
       __asm __volatile (LOCK_INSTR "orl %5, (%2)\n\t"			      \
			 LLL_EBX_LOAD					      \
			 "int $0x80\n\t"				      \
			 LLL_EBX_LOAD					      \
			 : "=a" (__ignore)				      \
			 : "0" (SYS_futex), LLL_EBX_REG (&(futex)),	      \
			   "c" (__lll_private_flag (FUTEX_WAKE, private)),    \
			   "d" (_nr), "i" (FUTEX_OWNER_DIED),		      \
			   "i" (offsetof (tcbhead_t, sysinfo)));	      \
    })

/* Returns non-zero if error happened, zero if success.  */
#undef lll_futex_requeue

/* The kernel notifies a process with uses CLONE_CLEARTID via futex
   wakeup when the clone terminates.  The memory location contains the
   thread ID while the clone is running and is reset to zero
   afterwards.

   The macro parameter must not have any side effect.  */
#undef lll_wait_tid
#define lll_wait_tid(tid) \
  do {									      \
    int __ignore;							      \
    register __typeof (tid) _tid asm ("edx") = (tid);			      \
    if (_tid != 0)							      \
      __asm __volatile (LLL_EBX_LOAD					      \
			"1:\tmovl %1, %%eax\n\t"			      \
			"int $0x80\n\t"					      \
			"cmpl $0, (%%ebx)\n\t"				      \
			"jne 1b\n\t"					      \
			LLL_EBX_LOAD					      \
			: "=&a" (__ignore)				      \
			: "i" (SYS_futex), LLL_EBX_REG (&tid), "S" (0),	      \
			  "c" (FUTEX_WAIT), "d" (_tid),			      \
			  "i" (offsetof (tcbhead_t, sysinfo))		      \
			: "memory");					      \
  } while (0)


#endif  /* !__ASSEMBLER__ */

#endif	/* x32/lowlevellock.h */
