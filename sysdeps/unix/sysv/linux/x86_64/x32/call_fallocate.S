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

#include <sysdep.h>

/*
   int __call_fallocate (int fd, int mode, __off64_t offset,
			 __off64_t len);
 */

	.text
ENTRY (__call_fallocate)
	movq	%rbp
	cfi_adjust_cfa_offset (8)
	pushl	%rbx
	cfi_adjust_cfa_offset (8)

	mov	%edi, %ebx	/* fd			*/
	mov	%rcx, %rbp	/* len			*/
	mov	%esi, %ecx	/* mode 		*/
	mov	%rdx, %rsi	/* offset		*/
	mov	%ebp, %edi	/* len (0-31b)		*/
	sarq	$32, %rsi	/* offset (32-63b)	*/
	sarq	$32, %rbp	/* len (32-63b)		*/
	movl	$SYS_ify(fallocate), %eax
	ENTER_KERNEL

	popl	%rbx
	cfi_adjust_cfa_offset (-8)
	popl	%rbp
	cfi_adjust_cfa_offset (-8)
	cfi_restore (rbp)

	cmpl	$-4095, %eax
	jae	SYSCALL_ERROR_LABEL
L(pseudo_end):
	ret
PSEUDO_END (__call_fallocate)
