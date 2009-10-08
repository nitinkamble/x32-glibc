/* Copyright (C) 2009 Free Software Foundation, Inc.
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

#include <sysdep.h>

/* Please consult the file sysdeps/unix/sysv/linux/x86-64/sysdep.h for
   more information about the value -4095 used below.  */

/* Usage: long syscall (syscall_number, arg1, arg2, arg3, arg4, arg5, arg6)
   We need to do some arg shifting, the syscall_number will be in
   rax.  */


	.text
ENTRY (syscall)
	pushq %rbp
	pushq %rbx
	mov %edi, %eax		/* Syscall number -> rax.  */
	mov %esi, %ebx		/* shift arg1 - arg6.  */
	xchg %edx, %ecx
	mov %r8d, %edi
	mov %r9d, %ebp
	int $0x80
	cmpq $-4095, %rax	/* Check %rax for error.  */
	popq %rbx
	popq %rbp
	jae SYSCALL_ERROR_LABEL	/* Jump to error handler if error.  */
L(pseudo_end):
	ret			/* Return to caller.  */

PSEUDO_END (syscall)
