/* FIXME: Remove this file after x32 vDSO support is added to kernel.  */

#define SYSCALL_NAME gettimeofday
#define SYSCALL_NARGS 2
#define SYSCALL_SYMBOL __gettimeofday
#include <syscall-template.S>
weak_alias (__gettimeofday, gettimeofday)
libc_hidden_weak (gettimeofday)
weak_alias (__gettimeofday, __gettimeofday_internal)
libc_hidden_weak (__gettimeofday_internal)
