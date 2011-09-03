/* Definition of `struct rusage' used in the kernel.
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

/* Structure which says how much of each resource has been used.  */
struct kernel_rusage
  {
    /* Total amount of user time used.  */
    struct timeval ru_utime;
    /* Total amount of system time used.  */
    struct timeval ru_stime;
    /* Maximum resident set size (in kilobytes).  */
    long long int ru_maxrss;
    /* Amount of sharing of text segment memory
       with other processes (kilobyte-seconds).  */
    long long int ru_ixrss;
    /* Amount of data segment memory used (kilobyte-seconds).  */
    long long int ru_idrss;
    /* Amount of stack memory used (kilobyte-seconds).  */
    long long int ru_isrss;
    /* Number of soft page faults (i.e. those serviced by reclaiming
       a page from the list of pages awaiting reallocation.  */
    long long int ru_minflt;
    /* Number of hard page faults (i.e. those that required I/O).  */
    long long int ru_majflt;
    /* Number of times a process was swapped out of physical memory.  */
    long long int ru_nswap;
    /* Number of input operations via the file system.  Note: This
       and `ru_oublock' do not include operations with the cache.  */
    long long int ru_inblock;
    /* Number of output operations via the file system.  */
    long long int ru_oublock;
    /* Number of IPC messages sent.  */
    long long int ru_msgsnd;
    /* Number of IPC messages received.  */
    long long int ru_msgrcv;
    /* Number of signals delivered.  */
    long long int ru_nsignals;
    /* Number of voluntary context switches, i.e. because the process
       gave up the process before it had to (usually to wait for some
       resource to be available).  */
    long long int ru_nvcsw;
    /* Number of involuntary context switches, i.e. a higher priority process
       became runnable or the current process used up its time slice.  */
    long long int ru_nivcsw;
  };

static inline void
copy_to_kernel_rusage (struct kernel_rusage *to,
		       const struct rusage *from)
{
  to->ru_utime = from->ru_utime;
  to->ru_stime = from->ru_stime;
  to->ru_maxrss = from->ru_maxrss;
  to->ru_ixrss = from->ru_ixrss;
  to->ru_idrss = from->ru_idrss;
  to->ru_isrss = from->ru_isrss;
  to->ru_minflt = from->ru_minflt;
  to->ru_majflt = from->ru_majflt;
  to->ru_nswap = from->ru_nswap;
  to->ru_inblock = from->ru_inblock;
  to->ru_oublock = from->ru_oublock;
  to->ru_msgsnd = from->ru_msgsnd;
  to->ru_msgrcv = from->ru_msgrcv;
  to->ru_nsignals = from->ru_nsignals;
  to->ru_nvcsw = from->ru_nvcsw;
  to->ru_nivcsw = from->ru_nivcsw;
}

static inline void
copy_from_kernel_rusage (struct rusage *to,
			 const struct kernel_rusage *from)
{
  to->ru_utime = from->ru_utime;
  to->ru_stime = from->ru_stime;
  to->ru_maxrss = from->ru_maxrss;
  to->ru_ixrss = from->ru_ixrss;
  to->ru_idrss = from->ru_idrss;
  to->ru_isrss = from->ru_isrss;
  to->ru_minflt = from->ru_minflt;
  to->ru_majflt = from->ru_majflt;
  to->ru_nswap = from->ru_nswap;
  to->ru_inblock = from->ru_inblock;
  to->ru_oublock = from->ru_oublock;
  to->ru_msgsnd = from->ru_msgsnd;
  to->ru_msgrcv = from->ru_msgrcv;
  to->ru_nsignals = from->ru_nsignals;
  to->ru_nvcsw = from->ru_nvcsw;
  to->ru_nivcsw = from->ru_nivcsw;
}
