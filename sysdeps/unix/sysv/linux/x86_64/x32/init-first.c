/* FIXME: Remove this file after x32 vDSO support is added to kernel.  */

#ifdef SHARED
# include <dl-vdso.h>
# include <bits/libc-vdso.h>

long int (*__vdso_gettimeofday) (struct timeval *, void *) attribute_hidden;

long int (*__vdso_clock_gettime) (clockid_t, struct timespec *)
  __attribute__ ((nocommon));
strong_alias (__vdso_clock_gettime, __GI___vdso_clock_gettime attribute_hidden)
#endif

#include "../../init-first.c"
