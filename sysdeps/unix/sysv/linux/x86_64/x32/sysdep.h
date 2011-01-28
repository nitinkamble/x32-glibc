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

#ifndef _LINUX_X32_SYSDEP_H
#define _LINUX_X32_SYSDEP_H 1

/* There is some commonality.  */
#include <sysdeps/unix/sysv/linux/x86_64/sysdep.h>

#ifdef __ASSEMBLER__

# ifndef PIC
/* Nothing here.  */
# elif RTLD_PRIVATE_ERRNO
#  undef SYSCALL_ERROR_HANDLER
#  define SYSCALL_ERROR_HANDLER			\
0:						\
  leaq rtld_errno(%rip), %ecx;			\
  xorl %edx, %edx;				\
  subl %eax, %edx;				\
  movl %edx, (%rcx);				\
  orl $-1, %eax;				\
  jmp L(pseudo_end);
# elif USE___THREAD
#  undef SYSCALL_ERROR_HANDLER
#  define SYSCALL_ERROR_HANDLER			\
0:						\
  movq SYSCALL_ERROR_ERRNO@GOTTPOFF(%rip), %rcx;\
  xorl %edx, %edx;				\
  subl %eax, %edx;				\
  movl %edx, %fs:(%rcx);			\
  orl $-1, %eax;				\
  jmp L(pseudo_end);
# elif defined _LIBC_REENTRANT
/* Store (- %eax) into errno through the GOT.
   Note that errno occupies only 4 bytes.  */
#  undef SYSCALL_ERROR_HANDLER
#  define SYSCALL_ERROR_HANDLER			\
0:						\
  xorl %edx, %edx;				\
  subl %eax, %edx;				\
  pushq %rdx;					\
  cfi_adjust_cfa_offset(8);			\
  PUSH_ERRNO_LOCATION_RETURN;			\
  call BP_SYM (__errno_location)@PLT;		\
  POP_ERRNO_LOCATION_RETURN;			\
  popq %rdx;					\
  cfi_adjust_cfa_offset(-8);			\
  movl %edx, (%rax);				\
  orl $-1, %eax;				\
  jmp L(pseudo_end);

/* A quick note: it is assumed that the call to `__errno_location' does
   not modify the stack!  */
# else /* Not _LIBC_REENTRANT.  */
#  undef SYSCALL_ERROR_HANDLER
#  define SYSCALL_ERROR_HANDLER			\
0:movq errno@GOTPCREL(%RIP), %rcx;		\
  xorl %edx, %edx;				\
  subl %eax, %edx;				\
  movl %edx, (%rcx);				\
  orl $-1, %eax;				\
  jmp L(pseudo_end);
# endif	/* PIC */

/* For now, we use Linux/i386 int $0x80 for system calls.  */
#undef ENTER_KERNEL
#define ENTER_KERNEL int $0x80

/* Linux takes system call arguments in registers:

	syscall number	%eax	     call-clobbered
	arg 1		%ebx	     call-saved
	arg 2		%ecx	     call-clobbered
	arg 3		%edx	     call-clobbered
	arg 4		%esi	     call-clobbered
	arg 5		%edi	     call-clobbered
	arg 6		%ebp	     call-saved

   The register contents and stack layout upon entering the function 
   are:

	    %edi	Arg# 1
	    %esi	Arg# 2
	    %edx	Arg# 3
	    %ecx	Arg# 4
	    %r8d	Arg# 5
	    %r9d	Arg# 6
	  (%rsp)	Return address

   (Of course a function with say 3 arguments does not have entries for
   arguments 4, 5, and 6.)  */

#undef	DO_CALL
#define DO_CALL(syscall_name, args)			      		      \
    PUSHARGS_##args							      \
    DOARGS_##args							      \
    movl $SYS_ify (syscall_name), %eax;					      \
    ENTER_KERNEL							      \
    POPARGS_##args

#undef DOARGS_0
#undef DOARGS_1
#undef DOARGS_2
#undef DOARGS_3
#undef DOARGS_4
#undef DOARGS_5
#undef DOARGS_6

#define PUSHARGS_0	/* No arguments to push.  */
#define	DOARGS_0	/* No arguments to frob.  */
#define	POPARGS_0	/* No arguments to pop.  */
#define	_PUSHARGS_0	/* No arguments to push.  */
#define _DOARGS_0(n)	/* No arguments to frob.  */
#define	_POPARGS_0	/* No arguments to pop.  */

#define PUSHARGS_1	movq %rbx, %r9; L(SAVEBX1): PUSHARGS_0
#define	DOARGS_1	_DOARGS_1 (4)
#define	POPARGS_1	POPARGS_0; movq %r9, %rbx; L(RESTBX1):
#define	_PUSHARGS_1	pushq %rbx; cfi_adjust_cfa_offset (8); \
			cfi_rel_offset (rbx, 0); L(PUSHBX1): _PUSHARGS_0
#define _DOARGS_1(n)	movl %edi, %ebx; _DOARGS_0(n-4)
#define	_POPARGS_1	_POPARGS_0; popq %rbx; cfi_adjust_cfa_offset (-8); \
			cfi_restore (rbx); L(POPBX1):

#define PUSHARGS_2	PUSHARGS_1
#define	DOARGS_2	_DOARGS_2 (8)
#define	POPARGS_2	POPARGS_1
#define _PUSHARGS_2	_PUSHARGS_1
#define	_DOARGS_2(n)	xchgl %esi, %ecx; _DOARGS_1 (n-4)
#define	_POPARGS_2	_POPARGS_1

#define PUSHARGS_3	_PUSHARGS_2
#define DOARGS_3	_DOARGS_3 (16)
#define POPARGS_3	_POPARGS_3
#define _PUSHARGS_3	_PUSHARGS_2
#define _DOARGS_3(n)	_DOARGS_2 (n-4)
#define _POPARGS_3	_POPARGS_2

#define PUSHARGS_4	_PUSHARGS_4
#define DOARGS_4	_DOARGS_4 (24)
#define POPARGS_4	_POPARGS_4
#define _PUSHARGS_4	_PUSHARGS_3
#define _DOARGS_4(n)	_DOARGS_3 (n-4)
#define _POPARGS_4	_POPARGS_3

#define PUSHARGS_5	_PUSHARGS_5
#define DOARGS_5	_DOARGS_5 (32)
#define POPARGS_5	_POPARGS_5
#define _PUSHARGS_5	_PUSHARGS_4
#define _DOARGS_5(n)	movl %r8d, %edi; _DOARGS_4 (n-4)
#define _POPARGS_5	_POPARGS_4;

#define PUSHARGS_6	_PUSHARGS_6
#define DOARGS_6	_DOARGS_6 (40)
#define POPARGS_6	_POPARGS_6
#define _PUSHARGS_6	pushq %rbp; cfi_adjust_cfa_offset (8); \
			cfi_rel_offset (rbp, 0); L(PUSHBP1): _PUSHARGS_5
#define _DOARGS_6(n)	movl %r9d, %ebp; _DOARGS_5 (n-4)
#define _POPARGS_6	_POPARGS_5; popq %rbp; cfi_adjust_cfa_offset (-8); \
			cfi_restore (rbp); L(POPBP1):

#else	/* !__ASSEMBLER__ */

# undef INTERNAL_SYSCALL_NCS
# define INTERNAL_SYSCALL_NCS(name, err, nr, args...) \
  ({									      \
    register unsigned int resultvar;					      \
    asm volatile (							      \
    LOADARGS_##nr							      \
    "int $0x80\n\t"							      \
    LOADARGS_##nr							      \
    : "=a" (resultvar)							      \
    : "0" (name) ASMFMT_##nr(args) : "memory", "cc");			      \
    (int) resultvar; })

# undef INTERNAL_SYSCALL
# define INTERNAL_SYSCALL(name, err, nr, args...) \
  INTERNAL_SYSCALL_NCS (__NR_##name, err, nr, ##args)

# undef INLINE_VSYSCALL
# define INLINE_VSYSCALL(name, nr, args...) \
  INLINE_SYSCALL (name, nr, ##args)
# undef INTERNAL_VSYSCALL
# define INTERNAL_VSYSCALL(name, err, nr, args...) \
  INTERNAL_SYSCALL (name, err, nr, ##args)

# define LOADARGS_0
# ifdef __PIC__
#  define LOADARGS_1	"xchgq %q1, %%rbx\n"
#  define LOADARGS_2	LOADARGS_1
#  define LOADARGS_3	LOADARGS_1
#  define LOADARGS_4	LOADARGS_1
#  define LOADARGS_5	LOADARGS_1
# else
#  define LOADARGS_1
#  define LOADARGS_2
#  define LOADARGS_3
#  define LOADARGS_4
#  define LOADARGS_5
# endif

# define ASMFMT_0()
# ifdef __PIC__
#  define ASMFMT_1(arg1) \
	, "cd" (arg1)
#  define ASMFMT_2(arg1, arg2) \
	, "d" (arg1), "c" (arg2)
#  define ASMFMT_3(arg1, arg2, arg3) \
	, "D" (arg1), "c" (arg2), "d" (arg3)
#  define ASMFMT_4(arg1, arg2, arg3, arg4) \
	, "D" (arg1), "c" (arg2), "d" (arg3), "S" (arg4)
#  define ASMFMT_5(arg1, arg2, arg3, arg4, arg5) \
	, "r9" (arg1), "c" (arg2), "d" (arg3), "S" (arg4), "D" (arg5)
# else
#  define ASMFMT_1(arg1) \
	, "b" (arg1)
#  define ASMFMT_2(arg1, arg2) \
	, "b" (arg1), "c" (arg2)
#  define ASMFMT_3(arg1, arg2, arg3) \
	, "b" (arg1), "c" (arg2), "d" (arg3)
#  define ASMFMT_4(arg1, arg2, arg3, arg4) \
	, "b" (arg1), "c" (arg2), "d" (arg3), "S" (arg4)
#  define ASMFMT_5(arg1, arg2, arg3, arg4, arg5) \
	, "b" (arg1), "c" (arg2), "d" (arg3), "S" (arg4), "D" (arg5)
# endif

#endif	/* __ASSEMBLER__ */

/* Pointer mangling support.  */
#undef PTR_MANGLE
#undef PTR_DEMANGLE

#if defined NOT_IN_libc && defined IS_IN_rtld
/* We cannot use the thread descriptor because in ld.so we use setjmp
   earlier than the descriptor is initialized.  */
# ifdef __ASSEMBLER__
#  define PTR_MANGLE(reg)	xorl __pointer_chk_guard_local(%rip), reg;    \
				roll $17, reg
#  define PTR_DEMANGLE(reg)	rorl $17, reg;				      \
				xorl __pointer_chk_guard_local(%rip), reg
# else
#  define PTR_MANGLE(reg)	asm ("xorl __pointer_chk_guard_local(%%rip), %0\n" \
				     "roll $17, %0"			      \
				     : "=r" (reg) : "0" (reg))
#  define PTR_DEMANGLE(reg)	asm ("rorl $17, %0\n"			      \
				     "xorl __pointer_chk_guard_local(%%rip), %0" \
				     : "=r" (reg) : "0" (reg))
# endif
#else
# ifdef __ASSEMBLER__
#  define PTR_MANGLE(reg)	xorl %fs:POINTER_GUARD, reg;		      \
				roll $17, reg
#  define PTR_DEMANGLE(reg)	rorl $17, reg;				      \
				xorl %fs:POINTER_GUARD, reg
# else
#  define PTR_MANGLE(var)	asm ("xorl %%fs:%c2, %0\n"		      \
				     "roll $17, %0"			      \
				     : "=r" (var)			      \
				     : "0" (var),			      \
				       "i" (offsetof (tcbhead_t,	      \
						      pointer_guard)))
#  define PTR_DEMANGLE(var)	asm ("rorl $17, %0\n"			      \
				     "xorl %%fs:%c2, %0"		      \
				     : "=r" (var)			      \
				     : "0" (var),			      \
				       "i" (offsetof (tcbhead_t,	      \
						      pointer_guard)))
# endif
#endif

#endif /* linux/x86_64/x32/sysdep.h */
