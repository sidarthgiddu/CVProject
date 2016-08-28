/* Licensed to the Apache Software Foundation (ASF) under one or more
 * contributor license agreements.  See the NOTICE file distributed with
 * this work for additional information regarding copyright ownership.
 * The ASF licenses this file to You under the Apache License, Version 2.0
 * (the "License"); you may not use this file except in compliance with
 * the License.  You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */


#ifndef APR_H
#define APR_H

/* GENERATED FILE WARNING!  DO NOT EDIT apr.h
 *
 * You must modify apr.h.in instead.
 *
 * And please, make an effort to stub apr.hw and apr.hnw in the process.
 */

/* So that we can use inline on some critical functions, and use
 * GNUC attributes (such as to get -Wall warnings for printf-like
 * functions).  Only do this in gcc 2.7 or later ... it may work
 * on earlier stuff, but why chance it.
 *
 * We've since discovered that the gcc shipped with NeXT systems
 * as "cc" is completely broken.  It claims to be __GNUC__ and so
 * on, but it doesn't implement half of the things that __GNUC__
 * means.  In particular it's missing inline and the __attribute__
 * stuff.  So we hack around it.  PR#1613. -djg
 */
#if !defined(__GNUC__) || __GNUC__ < 2 || \
    (__GNUC__ == 2 && __GNUC_MINOR__ < 7) ||\
    defined(NEXT)
#ifndef __attribute__
#define __attribute__(__x)
#endif
#define APR_INLINE
#define APR_HAS_INLINE          0
#else
#define APR_INLINE __inline__
#define APR_HAS_INLINE          1
#endif

#define APR_HAVE_ARPA_INET_H     1
#define APR_HAVE_CONIO_H         0
#define APR_HAVE_CRYPT_H         1
#define APR_HAVE_CTYPE_H         1
#define APR_HAVE_DIRENT_H        1
#define APR_HAVE_ERRNO_H         1
#define APR_HAVE_FCNTL_H         1
#define APR_HAVE_IO_H            0
#define APR_HAVE_LIMITS_H        1
#define APR_HAVE_NETDB_H         1
#define APR_HAVE_NETINET_IN_H    1
#define APR_HAVE_NETINET_SCTP_H  0
#define APR_HAVE_NETINET_SCTP_UIO_H 0
#define APR_HAVE_NETINET_TCP_H   1
#define APR_HAVE_PTHREAD_H       1
#define APR_HAVE_SEMAPHORE_H     1
#define APR_HAVE_SIGNAL_H        1
#define APR_HAVE_STDARG_H        1
#define APR_HAVE_STDINT_H        1
#define APR_HAVE_STDIO_H         1
#define APR_HAVE_STDLIB_H        1
#define APR_HAVE_STRING_H        1
#define APR_HAVE_STRINGS_H       1
#define APR_HAVE_SYS_IOCTL_H     1
#define APR_HAVE_SYS_SENDFILE_H  1
#define APR_HAVE_SYS_SIGNAL_H    1
#define APR_HAVE_SYS_SOCKET_H    1
#define APR_HAVE_SYS_SOCKIO_H    0
#define APR_HAVE_SYS_SYSLIMITS_H 0
#define APR_HAVE_SYS_TIME_H      1
#define APR_HAVE_SYS_TYPES_H     1
#define APR_HAVE_SYS_UIO_H       1
#define APR_HAVE_SYS_UN_H        1
#define APR_HAVE_SYS_WAIT_H      1
#define APR_HAVE_TIME_H          1
#define APR_HAVE_UNISTD_H        1

/* We don't include our conditional headers within the doxyblocks 
 * or the extern "C" namespace 
 */

#if APR_HAVE_SYS_TYPES_H
#include <sys/types.h>
#endif

#if APR_HAVE_SYS_SOCKET_H
#include <sys/socket.h>
#endif

#if defined(__cplusplus) && !defined(__STDC_CONSTANT_MACROS)
/* C99 7.18.4 requires that stdint.h only exposes INT64_C 
 * and UINT64_C for C++ implementations if this is defined: */
#define __STDC_CONSTANT_MACROS
#endif

#if APR_HAVE_STDINT_H
#include <stdint.h>
#endif

#if APR_HAVE_SYS_WAIT_H
#include <sys/wait.h>
#endif

#ifdef OS2
#define INCL_DOS
#define INCL_DOSERRORS
#include <os2.h>
#endif

/* header files for PATH_MAX, _POSIX_PATH_MAX */
#if APR_HAVE_LIMITS_H
#include <limits.h>
#else
#if APR_HAVE_SYS_SYSLIMITS_H
#include <sys/syslimits.h>
#endif
#endif


#ifdef __cplusplus
extern "C" {
#endif

#define APR_HAVE_SHMEM_MMAP_TMP     1
#define APR_HAVE_SHMEM_MMAP_SHM     1
#define APR_HAVE_SHMEM_MMAP_ZERO    1
#define APR_HAVE_SHMEM_SHMGET_ANON  1
#define APR_HAVE_SHMEM_SHMGET       1
#define APR_HAVE_SHMEM_MMAP_ANON    1
#define APR_HAVE_SHMEM_BEOS         0

#define APR_USE_SHMEM_MMAP_TMP     0
#define APR_USE_SHMEM_MMAP_SHM     0
#define APR_USE_SHMEM_MMAP_ZERO    0
#define APR_USE_SHMEM_SHMGET_ANON  0
#define APR_USE_SHMEM_SHMGET       1
#define APR_USE_SHMEM_MMAP_ANON    1
#define APR_USE_SHMEM_BEOS         0

#define APR_USE_FLOCK_SERIALIZE           0 
#define APR_USE_SYSVSEM_SERIALIZE         1
#define APR_USE_POSIXSEM_SERIALIZE        0
#define APR_USE_FCNTL_SERIALIZE           0
#define APR_USE_PROC_PTHREAD_SERIALIZE    0 
#define APR_USE_PTHREAD_SERIALIZE         1 

#define APR_HAS_FLOCK_SERIALIZE           1
#define APR_HAS_SYSVSEM_SERIALIZE         1
#define APR_HAS_POSIXSEM_SERIALIZE        1
#define APR_HAS_FCNTL_SERIALIZE           1
#define APR_HAS_PROC_PTHREAD_SERIALIZE    1
#define APR_HAS_RWLOCK_SERIALIZE          0

#define APR_PROCESS_LOCK_IS_GLOBAL        0

#define APR_HAVE_CORKABLE_TCP   1 
#define APR_HAVE_GETRLIMIT      1
#define APR_HAVE_IN_ADDR        1
#define APR_HAVE_INET_ADDR      1
#define APR_HAVE_INET_NETWORK   1
#define APR_HAVE_IPV6           1
#define APR_HAVE_MEMMOVE        1
#define APR_HAVE_SETRLIMIT      1
#define APR_HAVE_SIGACTION      1
#define APR_HAVE_SIGSUSPEND     1
#define APR_HAVE_SIGWAIT        1
#define APR_HAVE_STRCASECMP     1
#define APR_HAVE_STRDUP         1
#define APR_HAVE_STRICMP        0
#define APR_HAVE_STRNCASECMP    1
#define APR_HAVE_STRNICMP       0
#define APR_HAVE_STRSTR         1
#define APR_HAVE_MEMCHR         1
#define APR_HAVE_STRUCT_RLIMIT  1
#define APR_HAVE_UNION_SEMUN    0
#define APR_HAVE_SCTP           0

/*  APR Feature Macros */
#define APR_HAS_SHARED_MEMORY     1
#define APR_HAS_THREADS           1
#define APR_HAS_SENDFILE          1
#define APR_HAS_MMAP              1
#define APR_HAS_FORK              1
#define APR_HAS_RANDOM            1
#define APR_HAS_OTHER_CHILD       1
#define APR_HAS_DSO               1
#define APR_HAS_SO_ACCEPTFILTER   0
#define APR_HAS_UNICODE_FS        0
#define APR_HAS_PROC_INVOKED      0
#define APR_HAS_USER              1
#define APR_HAS_LARGE_FILES       0
#define APR_HAS_XTHREAD_FILES     0
#define APR_HAS_OS_UUID           0

/* APR sets APR_FILES_AS_SOCKETS to 1 on systems where it is possible
 * to poll on files/pipes.  On such a system, the application can
 * call apr_socket_from_file() to get an APR socket representation and 
 * then pass the socket representation to apr_poll_socket_add().
 */
#define APR_FILES_AS_SOCKETS      1

/* Not all platforms have a real INADDR_NONE.  This macro replaces INADDR_NONE
 * on all platforms.
 */
#define APR_INADDR_NONE           INADDR_NONE

/* This macro indicates whether or not EBCDIC is the native character set.
 */
#define APR_CHARSET_EBCDIC        0

/* If we have a TCP implementation that can be "corked", what flag
 * do we use?
 */
#define APR_TCP_NOPUSH_FLAG       TCP_CORK

/* Is the TCP_NODELAY socket option inherited from listening sockets?
*/
#define APR_TCP_NODELAY_INHERITED 1

/* Is the O_NONBLOCK flag inherited from listening sockets?
*/
#define APR_O_NONBLOCK_INHERITED 0

/* Typedefs that APR needs. */

typedef  unsigned char           apr_byte_t;

typedef  short           apr_int16_t;
typedef  unsigned short  apr_uint16_t;
                                               
typedef  int             apr_int32_t;
typedef  unsigned int    apr_uint32_t;
                                               
typedef  long long            apr_int64_t;
typedef  unsigned long long   apr_uint64_t;

typedef  size_t          apr_size_t;
typedef  ssize_t         apr_ssize_t;
typedef  long           apr_off_t;
typedef  socklen_t       apr_socklen_t;

typedef unsigned long            apr_ino_t;

#define APR_SIZEOF_VOIDP 4

/* Mechanisms to properly type numeric literals */
#define APR_INT64_C(val) INT64_C(val)

/* Definitions that APR programs need to work properly. */

#define APR_THREAD_FUNC

#define APR_DECLARE(type)            type 

#define APR_DECLARE_NONSTD(type)     type

#define APR_DECLARE_DATA

/* Define APR_SSIZE_T_FMT.  
 * If ssize_t is an integer we define it to be "d",
 * if ssize_t is a long int we define it to be "ld",
 * if ssize_t is neither we declare an error here.
 * I looked for a better way to define this here, but couldn't find one, so
 * to find the logic for this definition search for "ssize_t_fmt" in
 * configure.in.
 */
#define APR_SSIZE_T_FMT "d"

/* And APR_SIZE_T_FMT */
#define APR_SIZE_T_FMT "d"

/* And APR_OFF_T_FMT */
#define APR_OFF_T_FMT "ld"

/* And APR_PID_T_FMT */
#define APR_PID_T_FMT "d"

/* And APR_INT64_T_FMT */
#define APR_INT64_T_FMT "lld"
#define APR_INT64_T_FMT_LEN 3

/* And APR_UINT64_T_FMT */
#define APR_UINT64_T_FMT "llu"
#define APR_UINT64_T_FMT_LEN 3

/* And APR_UINT64_T_HEX_FMT */
#define APR_UINT64_T_HEX_FMT "llx"
#define APR_UINT64_T_HEX_FMT_LEN  (sizeof(APR_UINT64_T_HEX_FMT) - 1)

/* Deal with atoi64 variables ... these should move to apr_private.h */
#define APR_HAVE_INT64_STRFN  1
#define APR_INT64_STRFN       strtoll

/* are we going to force the generic atomic operations */
#define APR_FORCE_ATOMIC_GENERIC 1

/* Does the proc mutex lock threads too */
#define APR_PROC_MUTEX_IS_GLOBAL      0

/* Local machine definition for console and log output. */
#define APR_EOL_STR              "\n"


#if APR_HAVE_SYS_WAIT_H
#ifdef WEXITSTATUS
#define apr_wait_t       int
#else
#define apr_wait_t       union wait
#define WEXITSTATUS(status)    (int)((status).w_retcode)
#define WTERMSIG(status)       (int)((status).w_termsig)
#endif /* !WEXITSTATUS */
#endif /* HAVE_SYS_WAIT_H */

#if defined(PATH_MAX)
#define APR_PATH_MAX       PATH_MAX
#elif defined(_POSIX_PATH_MAX)
#define APR_PATH_MAX       _POSIX_PATH_MAX
#else
#error no decision has been made on APR_PATH_MAX for your platform
#endif

#ifdef __cplusplus
}
#endif

#endif /* APR_H */