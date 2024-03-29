/* Copyright (C) 1995, 1996, 1997, 2000, 2002, 2004, 2009, 2011
   Free Software Foundation, Inc.
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

#ifndef _SYS_SHM_H
# error "Never include <bits/shm.h> directly; use <sys/shm.h> instead."
#endif

#include <bits/types.h>

/* Permission flag for shmget.  */
#define SHM_R		0400		/* or S_IRUGO from <linux/stat.h> */
#define SHM_W		0200		/* or S_IWUGO from <linux/stat.h> */

/* Flags for `shmat'.  */
#define SHM_RDONLY	010000		/* attach read-only else read-write */
#define SHM_RND		020000		/* round attach address to SHMLBA */
#define SHM_REMAP	040000		/* take-over region on attach */
#define SHM_EXEC	0100000		/* execution access */

/* Commands for `shmctl'.  */
#define SHM_LOCK	11		/* lock segment (root only) */
#define SHM_UNLOCK	12		/* unlock segment (root only) */

__BEGIN_DECLS

/* Segment low boundary address multiple.  */
#define SHMLBA		(__getpagesize ())
extern int __getpagesize (void) __THROW __attribute__ ((__const__));


/* Type to count number of attaches.  */
typedef __UNATIVE_LONG_TYPE shmatt_t;

/* Data structure describing a shared memory segment.  */
struct shmid_ds
  {
    struct ipc_perm shm_perm;		/* operation permission struct */
    size_t shm_segsz;			/* size of segment in bytes */
    __time_t shm_atime;			/* time of last shmat() */
#ifndef __x86_64__
    unsigned long int __unused1;
#endif
    __time_t shm_dtime;			/* time of last shmdt() */
#ifndef __x86_64__
    unsigned long int __unused2;
#endif
    __time_t shm_ctime;			/* time of last change by shmctl() */
#ifndef __x86_64__
    unsigned long int __unused3;
#endif
    __pid_t shm_cpid;			/* pid of creator */
    __pid_t shm_lpid;			/* pid of last shmop */
    shmatt_t shm_nattch;		/* number of current attaches */
    __UNATIVE_LONG_TYPE __unused4;
    __UNATIVE_LONG_TYPE __unused5;
  };

#ifdef __USE_MISC

/* ipcs ctl commands */
# define SHM_STAT 	13
# define SHM_INFO 	14

/* shm_mode upper byte flags */
# define SHM_DEST	01000	/* segment will be destroyed on last detach */
# define SHM_LOCKED	02000   /* segment will not be swapped */
# define SHM_HUGETLB	04000	/* segment is mapped via hugetlb */
# define SHM_NORESERVE	010000	/* don't check for reservations */

struct	shminfo
  {
    __UNATIVE_LONG_TYPE shmmax;
    __UNATIVE_LONG_TYPE shmmin;
    __UNATIVE_LONG_TYPE shmmni;
    __UNATIVE_LONG_TYPE shmseg;
    __UNATIVE_LONG_TYPE shmall;
    __UNATIVE_LONG_TYPE __unused1;
    __UNATIVE_LONG_TYPE __unused2;
    __UNATIVE_LONG_TYPE __unused3;
    __UNATIVE_LONG_TYPE __unused4;
  };

struct shm_info
  {
    int used_ids;
    __UNATIVE_LONG_TYPE shm_tot;	/* total allocated shm */
    __UNATIVE_LONG_TYPE shm_rss;	/* total resident shm */
    __UNATIVE_LONG_TYPE shm_swp;	/* total swapped shm */
    __UNATIVE_LONG_TYPE swap_attempts;
    __UNATIVE_LONG_TYPE swap_successes;
  };

#endif /* __USE_MISC */

__END_DECLS
