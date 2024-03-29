/* Round double value to long long int.
   Copyright (C) 1997, 2004, 2009, 2011 Free Software Foundation, Inc.
   This file is part of the GNU C Library.
   Contributed by Ulrich Drepper <drepper@cygnus.com>, 1997.

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

#define lround __hidden_lround
#define __lround __hidden___lround

#include <math.h>

#include "math_private.h"


long long int
__llround (double x)
{
  int32_t j0;
  int64_t i0;
  long long int result;
  int sign;

  EXTRACT_WORDS64 (i0, x);
  j0 = ((i0 >> 52) & 0x7ff) - 0x3ff;
  sign = i0 < 0 ? -1 : 1;
  i0 &= UINT64_C(0xfffffffffffff);
  i0 |= UINT64_C(0x10000000000000);

  if (j0 < (int32_t) (8 * sizeof (long long int)) - 1)
    {
      if (j0 < 0)
	return j0 < -1 ? 0 : sign;
      else if (j0 >= 52)
	result = i0 << (j0 - 52);
      else
	{
	  i0 += UINT64_C(0x8000000000000) >> j0;

	  result = i0 >> (52 - j0);
	}
    }
  else
    {
      /* The number is too large.  It is left implementation defined
	 what happens.  */
      return (long long int) x;
    }

  return sign * result;
}

weak_alias (__llround, llround)
#ifdef NO_LONG_DOUBLE
strong_alias (__llround, __lroundl)
weak_alias (__llround, lroundl)
#endif

/* long has the same width as long long on 64-bit machines.  */
#undef lround
#undef __lround
strong_alias (__llround, __lround)
weak_alias (__llround, lround)
#ifdef NO_LONG_DOUBLE
strong_alias (__llround, __llroundl)
weak_alias (__llround, llroundl)
#endif
