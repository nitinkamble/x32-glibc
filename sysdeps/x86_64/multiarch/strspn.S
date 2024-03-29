/* Multiple versions of strspn
   Copyright (C) 2009 Free Software Foundation, Inc.
   Contributed by Intel Corporation.
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

#include <config.h>

#ifdef HAVE_SSE4_SUPPORT

#include <sysdep.h>
#include <init-arch.h>

/* Define multiple versions only for the definition in libc.  */
#ifndef NOT_IN_libc
	.text
ENTRY(strspn)
	.type	strspn, @gnu_indirect_function
	cmpl	$0, __cpu_features+KIND_OFFSET(%rip)
	jne	1f
	call	__init_cpu_features
1:	lea	__strspn_sse2(%rip), %RAX_LP
	testl	$bit_SSE4_2, __cpu_features+CPUID_OFFSET+index_SSE4_2(%rip)
	jz	2f
	lea	__strspn_sse42(%rip), %RAX_LP
2:	ret
END(strspn)

# undef ENTRY
# define ENTRY(name) \
	.type __strspn_sse2, @function; \
	.globl __strspn_sse2; \
	.align 16; \
	__strspn_sse2: cfi_startproc; \
	CALL_MCOUNT
# undef END
# define END(name) \
	cfi_endproc; .size __strspn_sse2, .-__strspn_sse2
# undef libc_hidden_builtin_def
/* It doesn't make sense to send libc-internal strspn calls through a PLT.
   The speedup we get from using SSE4.2 instruction is likely eaten away
   by the indirect call in the PLT.  */
# define libc_hidden_builtin_def(name) \
	.globl __GI_strspn; __GI_strspn = __strspn_sse2
#endif

#endif /* HAVE_SSE4_SUPPORT */

#include "../strspn.S"
