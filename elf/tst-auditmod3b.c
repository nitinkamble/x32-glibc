/* Verify that changing xmm registers in audit library won't affect
   function parameter passing/return.  */

#include <dlfcn.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <bits/wordsize.h>
#include <gnu/lib-names.h>
#include <emmintrin.h>

unsigned int
la_version (unsigned int v)
{
  setlinebuf (stdout);

  printf ("version: %u\n", v);

  char buf[20];
  sprintf (buf, "%u", v);

  return v;
}

void
la_activity (uintptr_t *cookie, unsigned int flag)
{
  if (flag == LA_ACT_CONSISTENT)
    printf ("activity: consistent\n");
  else if (flag == LA_ACT_ADD)
    printf ("activity: add\n");
  else if (flag == LA_ACT_DELETE)
    printf ("activity: delete\n");
  else
    printf ("activity: unknown activity %u\n", flag);
}

char *
la_objsearch (const char *name, uintptr_t *cookie, unsigned int flag)
{
  char buf[100];
  const char *flagstr;
  if (flag == LA_SER_ORIG)
    flagstr = "LA_SET_ORIG";
  else if (flag == LA_SER_LIBPATH)
    flagstr = "LA_SER_LIBPATH";
  else if (flag == LA_SER_RUNPATH)
    flagstr = "LA_SER_RUNPATH";
  else if (flag == LA_SER_CONFIG)
    flagstr = "LA_SER_CONFIG";
  else if (flag == LA_SER_DEFAULT)
    flagstr = "LA_SER_DEFAULT";
  else if (flag == LA_SER_SECURE)
    flagstr = "LA_SER_SECURE";
  else
    {
       sprintf (buf, "unknown flag %d", flag);
       flagstr = buf;
    }
  printf ("objsearch: %s, %s\n", name, flagstr);

  return (char *) name;
}

unsigned int
la_objopen (struct link_map *l, Lmid_t lmid, uintptr_t *cookie)
{
  printf ("objopen: %ld, %s\n", lmid, l->l_name);

  return 3;
}

void
la_preinit (uintptr_t *cookie)
{
  printf ("preinit\n");
}

unsigned int
la_objclose  (uintptr_t *cookie)
{
  printf ("objclose\n");
  return 0;
}

uintptr_t
la_symbind32 (Elf32_Sym *sym, unsigned int ndx, uintptr_t *refcook,
	      uintptr_t *defcook, unsigned int *flags, const char *symname)
{
  printf ("symbind32: symname=%s, st_value=%#lx, ndx=%u, flags=%u\n",
	  symname, (long int) sym->st_value, ndx, *flags);

  return sym->st_value;
}

uintptr_t
la_symbind64 (Elf64_Sym *sym, unsigned int ndx, uintptr_t *refcook,
	      uintptr_t *defcook, unsigned int *flags, const char *symname)
{
  printf ("symbind64: symname=%s, st_value=%#lx, ndx=%u, flags=%u\n",
	  symname, (long int) sym->st_value, ndx, *flags);

  return sym->st_value;
}

#ifdef __LP64__
# define pltenter la_x86_64_gnu_pltenter
# define pltexit la_x86_64_gnu_pltexit
#else
# define pltenter la_x32_gnu_pltenter
# define pltexit la_x32_gnu_pltexit
#endif
#define La_regs La_x86_64_regs
#define La_retval La_x86_64_retval
#define int_retval lrv_rax

#include <tst-audit.h>

ElfW(Addr)
pltenter (ElfW(Sym) *sym, unsigned int ndx, uintptr_t *refcook,
	  uintptr_t *defcook, La_regs *regs, unsigned int *flags,
	  const char *symname, long int *framesizep)
{
  printf ("pltenter: symname=%s, st_value=%#lx, ndx=%u, flags=%u\n",
	  symname, (long int) sym->st_value, ndx, *flags);

  __m128i xmm = _mm_set1_epi32 (-1);
  asm volatile ("movdqa %0, %%xmm0" : : "x" (xmm) : "xmm0" );
  asm volatile ("movdqa %0, %%xmm1" : : "x" (xmm) : "xmm1" );
  asm volatile ("movdqa %0, %%xmm2" : : "x" (xmm) : "xmm2" );
  asm volatile ("movdqa %0, %%xmm3" : : "x" (xmm) : "xmm3" );
  asm volatile ("movdqa %0, %%xmm4" : : "x" (xmm) : "xmm4" );
  asm volatile ("movdqa %0, %%xmm5" : : "x" (xmm) : "xmm5" );
  asm volatile ("movdqa %0, %%xmm6" : : "x" (xmm) : "xmm6" );
  asm volatile ("movdqa %0, %%xmm7" : : "x" (xmm) : "xmm7" );

  return sym->st_value;
}

unsigned int
pltexit (ElfW(Sym) *sym, unsigned int ndx, uintptr_t *refcook,
	 uintptr_t *defcook, const La_regs *inregs, La_retval *outregs,
	 const char *symname)
{
  printf ("pltexit: symname=%s, st_value=%#lx, ndx=%u, retval=%tu\n",
	  symname, (long int) sym->st_value, ndx,
	  (ptrdiff_t) outregs->int_retval);

  __m128i xmm = _mm_set1_epi32 (-1);
  asm volatile ("movdqa %0, %%xmm0" : : "x" (xmm) : "xmm0" );
  asm volatile ("movdqa %0, %%xmm1" : : "x" (xmm) : "xmm1" );
  asm volatile ("movdqa %0, %%xmm2" : : "x" (xmm) : "xmm2" );
  asm volatile ("movdqa %0, %%xmm3" : : "x" (xmm) : "xmm3" );
  asm volatile ("movdqa %0, %%xmm4" : : "x" (xmm) : "xmm4" );
  asm volatile ("movdqa %0, %%xmm5" : : "x" (xmm) : "xmm5" );
  asm volatile ("movdqa %0, %%xmm6" : : "x" (xmm) : "xmm6" );
  asm volatile ("movdqa %0, %%xmm7" : : "x" (xmm) : "xmm7" );

  return 0;
}
