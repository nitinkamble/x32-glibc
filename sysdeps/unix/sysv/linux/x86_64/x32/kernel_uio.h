/* This is the kernel structure for scatter/gather I/O.  */

struct kernel_iovec
{
  uint64_t iov_base;
  uint64_t iov_len;
};
