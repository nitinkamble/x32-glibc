/* __NR_fcntl64 is the same as __NR_fcntl under x32 kernel.  */
#define __NR_fcntl64 __NR_fcntl

#undef __ASSUME_FCNTL64
#define __ASSUME_FCNTL64 1

#include <sysdeps/unix/sysv/linux/i386/lockf64.c>
