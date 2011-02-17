/* Assembler macros for x32.
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

#undef ASM_ADDR

#ifdef	__ASSEMBLER__

# define ASM_ADDR .long

# undef RAX
# undef RBP
# undef RCX
# undef RDI
# undef RDX
# undef RSP
# undef R8
# undef R9
# undef R10
# undef R11

# define RAX	eax
# define RBP	ebp
# define RCX	ecx
# define RDI	edi
# define RDX	edx
# define RSP	esp
# define R8	r8d
# define R9	r9d
# define R10	r10d
# define R11	r11d

#else	/* __ASSEMBLER__ */

# define ASM_ADDR ".long"

#endif	/* __ASSEMBLER__ */
