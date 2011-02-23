/* Copyright (C) 2001,2004,2005,2006,2009 Free Software Foundation, Inc.
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
#include <jmpbuf-offsets.h>
#include <asm-syntax.h>

/* Jump to the position specified by ENV, causing the
   setjmp call there to return VAL, or 1 if VAL is 0.
   void __longjmp (__jmp_buf env, int val).  */
	.text
ENTRY(__longjmp)
	/* Restore registers.  */
	mov (JB_RSP*8)(%rdi),%RAX
	mov (JB_RBP*8)(%rdi),%RCX
	mov (JB_PC*8)(%rdi),%RDX
#ifdef PTR_DEMANGLE
	PTR_DEMANGLE (%RAX)
	PTR_DEMANGLE (%RCX)
	PTR_DEMANGLE (%RDX)
#endif
	/* We add unwind information for the target here.  */
	cfi_def_cfa(%rdi, 0)
	cfi_register(%rsp,%rax)
	cfi_register(%rbp,%rcx)
	cfi_register(%rip,%rdx)
	cfi_offset(%rbx,JB_RBX*8)
	cfi_offset(%r12,JB_R12*8)
	cfi_offset(%r13,JB_R13*8)
	cfi_offset(%r14,JB_R14*8)
	cfi_offset(%r15,JB_R15*8)
	movq (JB_RBX*8)(%rdi),%rbx
	movq (JB_R12*8)(%rdi),%r12
	movq (JB_R13*8)(%rdi),%r13
	movq (JB_R14*8)(%rdi),%r14
	movq (JB_R15*8)(%rdi),%r15
	mov %RAX,%RSP
	mov %RCX,%RBP
	/* Set return value for setjmp.  */
	mov %esi, %eax
	jmpq *%rdx
END (__longjmp)
