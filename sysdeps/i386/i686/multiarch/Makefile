ifeq ($(subdir),csu)
aux += init-arch
gen-as-const-headers += ifunc-defines.sym
endif

ifeq ($(subdir),string)
sysdep_routines += bzero-sse2 memset-sse2 memcpy-ssse3 mempcpy-ssse3 \
		   memmove-ssse3 memcpy-ssse3-rep mempcpy-ssse3-rep \
		   memmove-ssse3-rep bcopy-ssse3 bcopy-ssse3-rep \
		   memset-sse2-rep bzero-sse2-rep strcmp-ssse3 \
		   strcmp-sse4 strncmp-c strncmp-ssse3 strncmp-sse4 \
		   memcmp-ssse3 memcmp-sse4 strcasestr-nonascii varshift \
		   strlen-sse2 strlen-sse2-bsf strncpy-c strcpy-ssse3 \
		   strncpy-ssse3 stpcpy-ssse3 stpncpy-ssse3 strcpy-sse2 \
		   strncpy-sse2 stpcpy-sse2 stpncpy-sse2 strcat-ssse3 \
		   strcat-sse2 strncat-ssse3 strncat-sse2 strncat-c \
		   strchr-sse2 strrchr-sse2 strchr-sse2-bsf strrchr-sse2-bsf \
		   wcscmp-sse2 wcscmp-c
ifeq (yes,$(config-cflags-sse4))
sysdep_routines += strcspn-c strpbrk-c strspn-c strstr-c strcasestr-c
CFLAGS-varshift.c += -msse4
CFLAGS-strcspn-c.c += -msse4
CFLAGS-strpbrk-c.c += -msse4
CFLAGS-strspn-c.c += -msse4
CFLAGS-strstr.c += -msse4
CFLAGS-strcasestr.c += -msse4
CFLAGS-strcasestr-nonascii.c += -msse4
endif
endif

ifeq (mathyes,$(subdir)$(config-cflags-avx))
libm-sysdep_routines += s_fma-fma s_fmaf-fma
CFLAGS-s_fma-fma.c += -mavx -mfpmath=sse
CFLAGS-s_fmaf-fma.c += -mavx -mfpmath=sse
endif
