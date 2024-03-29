/* Multiple versions of __memset_chk
   Copyright (C) 2010 Free Software Foundation, Inc.
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
#include <init-arch.h>

/* Define multiple versions only for the definition in lib.  */
#ifndef NOT_IN_libc
# ifdef SHARED
ENTRY(__memset_chk)
	.type	__memset_chk, @gnu_indirect_function
	cmpl	$0, __cpu_features+KIND_OFFSET(%rip)
	jne	1f
	call	__init_cpu_features
1:	lea	__memset_chk_x86_64(%rip), %RAX_LP
	testl	$bit_Prefer_SSE_for_memop, __cpu_features+FEATURE_OFFSET+index_Prefer_SSE_for_memop(%rip)
	jz	2f
	lea	__memset_chk_sse2(%rip), %RAX_LP
2:	ret
END(__memset_chk)

strong_alias (__memset_chk, __memset_zero_constant_len_parameter)
	.section .gnu.warning.__memset_zero_constant_len_parameter
	.string "memset used with constant zero length parameter; this could be due to transposed parameters"
# else
#  include "../memset_chk.S"
# endif
#endif
