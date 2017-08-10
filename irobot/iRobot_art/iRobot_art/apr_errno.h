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

#ifndef APR_ERRNO_H
#define APR_ERRNO_H

#include "apr.h"

#if APR_HAVE_ERRNO_H
#include <errno.h>
#endif

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

	typedef int apr_status_t;

	APR_DECLARE(char *) apr_strerror(apr_status_t statcode, char *buf,
		apr_size_t bufsize);

#if defined(DOXYGEN)

#define APR_FROM_OS_ERROR(e) (e == 0 ? APR_SUCCESS : e + APR_OS_START_SYSERR)

#define APR_TO_OS_ERROR(e) (e == 0 ? APR_SUCCESS : e - APR_OS_START_SYSERR)

#endif /* defined(DOXYGEN) */

#define APR_OS_START_ERROR     20000

#define APR_OS_ERRSPACE_SIZE 50000

#define APR_OS_START_STATUS    (APR_OS_START_ERROR + APR_OS_ERRSPACE_SIZE)

#define APR_OS_START_USERERR    (APR_OS_START_STATUS + APR_OS_ERRSPACE_SIZE)

#define APR_OS_START_USEERR     APR_OS_START_USERERR

#define APR_OS_START_CANONERR  (APR_OS_START_USERERR \
                                 + (APR_OS_ERRSPACE_SIZE * 10))

#define APR_OS_START_EAIERR    (APR_OS_START_CANONERR + APR_OS_ERRSPACE_SIZE)

#define APR_OS_START_SYSERR    (APR_OS_START_EAIERR + APR_OS_ERRSPACE_SIZE)

#define APR_SUCCESS 0

#define APR_ENOSTAT        (APR_OS_START_ERROR + 1)

#define APR_ENOPOOL        (APR_OS_START_ERROR + 2)
	/* empty slot: +3 */
#define APR_EBADDATE       (APR_OS_START_ERROR + 4)

#define APR_EINVALSOCK     (APR_OS_START_ERROR + 5)

#define APR_ENOPROC        (APR_OS_START_ERROR + 6)

#define APR_ENOTIME        (APR_OS_START_ERROR + 7)

#define APR_ENODIR         (APR_OS_START_ERROR + 8)

#define APR_ENOLOCK        (APR_OS_START_ERROR + 9)

#define APR_ENOPOLL        (APR_OS_START_ERROR + 10)

#define APR_ENOSOCKET      (APR_OS_START_ERROR + 11)

#define APR_ENOTHREAD      (APR_OS_START_ERROR + 12)

#define APR_ENOTHDKEY      (APR_OS_START_ERROR + 13)

#define APR_EGENERAL       (APR_OS_START_ERROR + 14)

#define APR_ENOSHMAVAIL    (APR_OS_START_ERROR + 15)

#define APR_EBADIP         (APR_OS_START_ERROR + 16)

#define APR_EBADMASK       (APR_OS_START_ERROR + 17)
	/* empty slot: +18 */
#define APR_EDSOOPEN       (APR_OS_START_ERROR + 19)

#define APR_EABSOLUTE      (APR_OS_START_ERROR + 20)

#define APR_ERELATIVE      (APR_OS_START_ERROR + 21)

#define APR_EINCOMPLETE    (APR_OS_START_ERROR + 22)

#define APR_EABOVEROOT     (APR_OS_START_ERROR + 23)

#define APR_EBADPATH       (APR_OS_START_ERROR + 24)

#define APR_EPATHWILD      (APR_OS_START_ERROR + 25)

#define APR_ESYMNOTFOUND   (APR_OS_START_ERROR + 26)

#define APR_EPROC_UNKNOWN  (APR_OS_START_ERROR + 27)

#define APR_STATUS_IS_ENOSTAT(s)        ((s) == APR_ENOSTAT)

#define APR_STATUS_IS_ENOPOOL(s)        ((s) == APR_ENOPOOL)

#define APR_STATUS_IS_EBADDATE(s)       ((s) == APR_EBADDATE)

#define APR_STATUS_IS_EINVALSOCK(s)     ((s) == APR_EINVALSOCK)

#define APR_STATUS_IS_ENOPROC(s)        ((s) == APR_ENOPROC)

#define APR_STATUS_IS_ENOTIME(s)        ((s) == APR_ENOTIME)

#define APR_STATUS_IS_ENODIR(s)         ((s) == APR_ENODIR)

#define APR_STATUS_IS_ENOLOCK(s)        ((s) == APR_ENOLOCK)

#define APR_STATUS_IS_ENOPOLL(s)        ((s) == APR_ENOPOLL)

#define APR_STATUS_IS_ENOSOCKET(s)      ((s) == APR_ENOSOCKET)

#define APR_STATUS_IS_ENOTHREAD(s)      ((s) == APR_ENOTHREAD)

#define APR_STATUS_IS_ENOTHDKEY(s)      ((s) == APR_ENOTHDKEY)

#define APR_STATUS_IS_EGENERAL(s)       ((s) == APR_EGENERAL)

#define APR_STATUS_IS_ENOSHMAVAIL(s)    ((s) == APR_ENOSHMAVAIL)

#define APR_STATUS_IS_EBADIP(s)         ((s) == APR_EBADIP)

#define APR_STATUS_IS_EBADMASK(s)       ((s) == APR_EBADMASK)
	/* empty slot: +18 */
#if defined(WIN32)
#define APR_STATUS_IS_EDSOOPEN(s)       ((s) == APR_EDSOOPEN \
                       || APR_TO_OS_ERROR(s) == ERROR_MOD_NOT_FOUND)
#else
#define APR_STATUS_IS_EDSOOPEN(s)       ((s) == APR_EDSOOPEN)
#endif

#define APR_STATUS_IS_EABSOLUTE(s)      ((s) == APR_EABSOLUTE)

#define APR_STATUS_IS_ERELATIVE(s)      ((s) == APR_ERELATIVE)

#define APR_STATUS_IS_EINCOMPLETE(s)    ((s) == APR_EINCOMPLETE)

#define APR_STATUS_IS_EABOVEROOT(s)     ((s) == APR_EABOVEROOT)

#define APR_STATUS_IS_EBADPATH(s)       ((s) == APR_EBADPATH)

#define APR_STATUS_IS_EPATHWILD(s)      ((s) == APR_EPATHWILD)

#if defined(WIN32)
#define APR_STATUS_IS_ESYMNOTFOUND(s)   ((s) == APR_ESYMNOTFOUND \
                       || APR_TO_OS_ERROR(s) == ERROR_PROC_NOT_FOUND)
#else
#define APR_STATUS_IS_ESYMNOTFOUND(s)   ((s) == APR_ESYMNOTFOUND)
#endif

#define APR_STATUS_IS_EPROC_UNKNOWN(s)  ((s) == APR_EPROC_UNKNOWN)

#define APR_INCHILD        (APR_OS_START_STATUS + 1)

#define APR_INPARENT       (APR_OS_START_STATUS + 2)

#define APR_DETACH         (APR_OS_START_STATUS + 3)

#define APR_NOTDETACH      (APR_OS_START_STATUS + 4)

#define APR_CHILD_DONE     (APR_OS_START_STATUS + 5)

#define APR_CHILD_NOTDONE  (APR_OS_START_STATUS + 6)

#define APR_TIMEUP         (APR_OS_START_STATUS + 7)

#define APR_INCOMPLETE     (APR_OS_START_STATUS + 8)
	/* empty slot: +9 */
	/* empty slot: +10 */
	/* empty slot: +11 */
#define APR_BADCH          (APR_OS_START_STATUS + 12)

#define APR_BADARG         (APR_OS_START_STATUS + 13)

#define APR_EOF            (APR_OS_START_STATUS + 14)

#define APR_NOTFOUND       (APR_OS_START_STATUS + 15)
	/* empty slot: +16 */
	/* empty slot: +17 */
	/* empty slot: +18 */
#define APR_ANONYMOUS      (APR_OS_START_STATUS + 19)

#define APR_FILEBASED      (APR_OS_START_STATUS + 20)

#define APR_KEYBASED       (APR_OS_START_STATUS + 21)

#define APR_EINIT          (APR_OS_START_STATUS + 22)  

#define APR_ENOTIMPL       (APR_OS_START_STATUS + 23)

#define APR_EMISMATCH      (APR_OS_START_STATUS + 24)

#define APR_EBUSY          (APR_OS_START_STATUS + 25)

#define APR_STATUS_IS_INCHILD(s)        ((s) == APR_INCHILD)

#define APR_STATUS_IS_INPARENT(s)       ((s) == APR_INPARENT)

#define APR_STATUS_IS_DETACH(s)         ((s) == APR_DETACH)

#define APR_STATUS_IS_NOTDETACH(s)      ((s) == APR_NOTDETACH)

#define APR_STATUS_IS_CHILD_DONE(s)     ((s) == APR_CHILD_DONE)

#define APR_STATUS_IS_CHILD_NOTDONE(s)  ((s) == APR_CHILD_NOTDONE)

#define APR_STATUS_IS_TIMEUP(s)         ((s) == APR_TIMEUP)

#define APR_STATUS_IS_INCOMPLETE(s)     ((s) == APR_INCOMPLETE)
	/* empty slot: +9 */
	/* empty slot: +10 */
	/* empty slot: +11 */
#define APR_STATUS_IS_BADCH(s)          ((s) == APR_BADCH)

#define APR_STATUS_IS_BADARG(s)         ((s) == APR_BADARG)

#define APR_STATUS_IS_EOF(s)            ((s) == APR_EOF)

#define APR_STATUS_IS_NOTFOUND(s)       ((s) == APR_NOTFOUND)
	/* empty slot: +16 */
	/* empty slot: +17 */
	/* empty slot: +18 */
#define APR_STATUS_IS_ANONYMOUS(s)      ((s) == APR_ANONYMOUS)

#define APR_STATUS_IS_FILEBASED(s)      ((s) == APR_FILEBASED)

#define APR_STATUS_IS_KEYBASED(s)       ((s) == APR_KEYBASED)

#define APR_STATUS_IS_EINIT(s)          ((s) == APR_EINIT)

#define APR_STATUS_IS_ENOTIMPL(s)       ((s) == APR_ENOTIMPL)

#define APR_STATUS_IS_EMISMATCH(s)      ((s) == APR_EMISMATCH)

#define APR_STATUS_IS_EBUSY(s)          ((s) == APR_EBUSY)

	/* APR CANONICAL ERROR VALUES */
#ifdef EACCES
#define APR_EACCES EACCES
#else
#define APR_EACCES         (APR_OS_START_CANONERR + 1)
#endif

#ifdef EEXIST
#define APR_EEXIST EEXIST
#else
#define APR_EEXIST         (APR_OS_START_CANONERR + 2)
#endif

#ifdef ENAMETOOLONG
#define APR_ENAMETOOLONG ENAMETOOLONG
#else
#define APR_ENAMETOOLONG   (APR_OS_START_CANONERR + 3)
#endif

#ifdef ENOENT
#define APR_ENOENT ENOENT
#else
#define APR_ENOENT         (APR_OS_START_CANONERR + 4)
#endif

#ifdef ENOTDIR
#define APR_ENOTDIR ENOTDIR
#else
#define APR_ENOTDIR        (APR_OS_START_CANONERR + 5)
#endif

#ifdef ENOSPC
#define APR_ENOSPC ENOSPC
#else
#define APR_ENOSPC         (APR_OS_START_CANONERR + 6)
#endif

#ifdef ENOMEM
#define APR_ENOMEM ENOMEM
#else
#define APR_ENOMEM         (APR_OS_START_CANONERR + 7)
#endif

#ifdef EMFILE
#define APR_EMFILE EMFILE
#else
#define APR_EMFILE         (APR_OS_START_CANONERR + 8)
#endif

#ifdef ENFILE
#define APR_ENFILE ENFILE
#else
#define APR_ENFILE         (APR_OS_START_CANONERR + 9)
#endif

#ifdef EBADF
#define APR_EBADF EBADF
#else
#define APR_EBADF          (APR_OS_START_CANONERR + 10)
#endif

#ifdef EINVAL
#define APR_EINVAL EINVAL
#else
#define APR_EINVAL         (APR_OS_START_CANONERR + 11)
#endif

#ifdef ESPIPE
#define APR_ESPIPE ESPIPE
#else
#define APR_ESPIPE         (APR_OS_START_CANONERR + 12)
#endif

#ifdef EAGAIN
#define APR_EAGAIN EAGAIN
#elif defined(EWOULDBLOCK)
#define APR_EAGAIN EWOULDBLOCK
#else
#define APR_EAGAIN         (APR_OS_START_CANONERR + 13)
#endif

#ifdef EINTR
#define APR_EINTR EINTR
#else
#define APR_EINTR          (APR_OS_START_CANONERR + 14)
#endif

#ifdef ENOTSOCK
#define APR_ENOTSOCK ENOTSOCK
#else
#define APR_ENOTSOCK       (APR_OS_START_CANONERR + 15)
#endif

#ifdef ECONNREFUSED
#define APR_ECONNREFUSED ECONNREFUSED
#else
#define APR_ECONNREFUSED   (APR_OS_START_CANONERR + 16)
#endif

#ifdef EINPROGRESS
#define APR_EINPROGRESS EINPROGRESS
#else
#define APR_EINPROGRESS    (APR_OS_START_CANONERR + 17)
#endif

#ifdef ECONNABORTED
#define APR_ECONNABORTED ECONNABORTED
#else
#define APR_ECONNABORTED   (APR_OS_START_CANONERR + 18)
#endif

#ifdef ECONNRESET
#define APR_ECONNRESET ECONNRESET
#else
#define APR_ECONNRESET     (APR_OS_START_CANONERR + 19)
#endif

#ifdef ETIMEDOUT
#define APR_ETIMEDOUT ETIMEDOUT
#else
#define APR_ETIMEDOUT      (APR_OS_START_CANONERR + 20)
#endif

#ifdef EHOSTUNREACH
#define APR_EHOSTUNREACH EHOSTUNREACH
#else
#define APR_EHOSTUNREACH   (APR_OS_START_CANONERR + 21)
#endif

#ifdef ENETUNREACH
#define APR_ENETUNREACH ENETUNREACH
#else
#define APR_ENETUNREACH    (APR_OS_START_CANONERR + 22)
#endif

#ifdef EFTYPE
#define APR_EFTYPE EFTYPE
#else
#define APR_EFTYPE        (APR_OS_START_CANONERR + 23)
#endif

#ifdef EPIPE
#define APR_EPIPE EPIPE
#else
#define APR_EPIPE         (APR_OS_START_CANONERR + 24)
#endif

#ifdef EXDEV
#define APR_EXDEV EXDEV
#else
#define APR_EXDEV         (APR_OS_START_CANONERR + 25)
#endif

#ifdef ENOTEMPTY
#define APR_ENOTEMPTY ENOTEMPTY
#else
#define APR_ENOTEMPTY     (APR_OS_START_CANONERR + 26)
#endif

#if defined(OS2) && !defined(DOXYGEN)

#define APR_FROM_OS_ERROR(e) (e == 0 ? APR_SUCCESS : e + APR_OS_START_SYSERR)
#define APR_TO_OS_ERROR(e)   (e == 0 ? APR_SUCCESS : e - APR_OS_START_SYSERR)

#define INCL_DOSERRORS
#define INCL_DOS

	/* Leave these undefined.
	* OS2 doesn't rely on the errno concept.
	* The API calls always return a result codes which
	* should be filtered through APR_FROM_OS_ERROR().
	*
	* #define apr_get_os_error()   (APR_FROM_OS_ERROR(GetLastError()))
	* #define apr_set_os_error(e)  (SetLastError(APR_TO_OS_ERROR(e)))
	*/

	/* A special case, only socket calls require this;
	*/
#define apr_get_netos_error()   (APR_FROM_OS_ERROR(errno))
#define apr_set_netos_error(e)  (errno = APR_TO_OS_ERROR(e))

	/* And this needs to be greped away for good:
	*/
#define APR_OS2_STATUS(e) (APR_FROM_OS_ERROR(e))

#define APR_STATUS_IS_SUCCESS(s)           ((s) == APR_SUCCESS \
                || (s) == APR_OS_START_SYSERR + NO_ERROR)

	/* These can't sit in a private header, so in spite of the extra size,
	* they need to be made available here.
	*/
#define SOCBASEERR              10000
#define SOCEPERM                (SOCBASEERR+1)             /* Not owner */
#define SOCESRCH                (SOCBASEERR+3)             /* No such process */
#define SOCEINTR                (SOCBASEERR+4)             /* Interrupted system call */
#define SOCENXIO                (SOCBASEERR+6)             /* No such device or address */
#define SOCEBADF                (SOCBASEERR+9)             /* Bad file number */
#define SOCEACCES               (SOCBASEERR+13)            /* Permission denied */
#define SOCEFAULT               (SOCBASEERR+14)            /* Bad address */
#define SOCEINVAL               (SOCBASEERR+22)            /* Invalid argument */
#define SOCEMFILE               (SOCBASEERR+24)            /* Too many open files */
#define SOCEPIPE                (SOCBASEERR+32)            /* Broken pipe */
#define SOCEOS2ERR              (SOCBASEERR+100)           /* OS/2 Error */
#define SOCEWOULDBLOCK          (SOCBASEERR+35)            /* Operation would block */
#define SOCEINPROGRESS          (SOCBASEERR+36)            /* Operation now in progress */
#define SOCEALREADY             (SOCBASEERR+37)            /* Operation already in progress */
#define SOCENOTSOCK             (SOCBASEERR+38)            /* Socket operation on non-socket */
#define SOCEDESTADDRREQ         (SOCBASEERR+39)            /* Destination address required */
#define SOCEMSGSIZE             (SOCBASEERR+40)            /* Message too long */
#define SOCEPROTOTYPE           (SOCBASEERR+41)            /* Protocol wrong type for socket */
#define SOCENOPROTOOPT          (SOCBASEERR+42)            /* Protocol not available */
#define SOCEPROTONOSUPPORT      (SOCBASEERR+43)            /* Protocol not supported */
#define SOCESOCKTNOSUPPORT      (SOCBASEERR+44)            /* Socket type not supported */
#define SOCEOPNOTSUPP           (SOCBASEERR+45)            /* Operation not supported on socket */
#define SOCEPFNOSUPPORT         (SOCBASEERR+46)            /* Protocol family not supported */
#define SOCEAFNOSUPPORT         (SOCBASEERR+47)            /* Address family not supported by protocol family */
#define SOCEADDRINUSE           (SOCBASEERR+48)            /* Address already in use */
#define SOCEADDRNOTAVAIL        (SOCBASEERR+49)            /* Can't assign requested address */
#define SOCENETDOWN             (SOCBASEERR+50)            /* Network is down */
#define SOCENETUNREACH          (SOCBASEERR+51)            /* Network is unreachable */
#define SOCENETRESET            (SOCBASEERR+52)            /* Network dropped connection on reset */
#define SOCECONNABORTED         (SOCBASEERR+53)            /* Software caused connection abort */
#define SOCECONNRESET           (SOCBASEERR+54)            /* Connection reset by peer */
#define SOCENOBUFS              (SOCBASEERR+55)            /* No buffer space available */
#define SOCEISCONN              (SOCBASEERR+56)            /* Socket is already connected */
#define SOCENOTCONN             (SOCBASEERR+57)            /* Socket is not connected */
#define SOCESHUTDOWN            (SOCBASEERR+58)            /* Can't send after socket shutdown */
#define SOCETOOMANYREFS         (SOCBASEERR+59)            /* Too many references: can't splice */
#define SOCETIMEDOUT            (SOCBASEERR+60)            /* Connection timed out */
#define SOCECONNREFUSED         (SOCBASEERR+61)            /* Connection refused */
#define SOCELOOP                (SOCBASEERR+62)            /* Too many levels of symbolic links */
#define SOCENAMETOOLONG         (SOCBASEERR+63)            /* File name too long */
#define SOCEHOSTDOWN            (SOCBASEERR+64)            /* Host is down */
#define SOCEHOSTUNREACH         (SOCBASEERR+65)            /* No route to host */
#define SOCENOTEMPTY            (SOCBASEERR+66)            /* Directory not empty */

	/* APR CANONICAL ERROR TESTS */
#define APR_STATUS_IS_EACCES(s)         ((s) == APR_EACCES \
                || (s) == APR_OS_START_SYSERR + ERROR_ACCESS_DENIED \
                || (s) == APR_OS_START_SYSERR + ERROR_SHARING_VIOLATION)
#define APR_STATUS_IS_EEXIST(s)         ((s) == APR_EEXIST \
                || (s) == APR_OS_START_SYSERR + ERROR_OPEN_FAILED \
                || (s) == APR_OS_START_SYSERR + ERROR_FILE_EXISTS \
                || (s) == APR_OS_START_SYSERR + ERROR_ALREADY_EXISTS \
                || (s) == APR_OS_START_SYSERR + ERROR_ACCESS_DENIED)
#define APR_STATUS_IS_ENAMETOOLONG(s)   ((s) == APR_ENAMETOOLONG \
                || (s) == APR_OS_START_SYSERR + ERROR_FILENAME_EXCED_RANGE \
                || (s) == APR_OS_START_SYSERR + SOCENAMETOOLONG)
#define APR_STATUS_IS_ENOENT(s)         ((s) == APR_ENOENT \
                || (s) == APR_OS_START_SYSERR + ERROR_FILE_NOT_FOUND \
                || (s) == APR_OS_START_SYSERR + ERROR_PATH_NOT_FOUND \
                || (s) == APR_OS_START_SYSERR + ERROR_NO_MORE_FILES \
                || (s) == APR_OS_START_SYSERR + ERROR_OPEN_FAILED)
#define APR_STATUS_IS_ENOTDIR(s)        ((s) == APR_ENOTDIR)
#define APR_STATUS_IS_ENOSPC(s)         ((s) == APR_ENOSPC \
                || (s) == APR_OS_START_SYSERR + ERROR_DISK_FULL)
#define APR_STATUS_IS_ENOMEM(s)         ((s) == APR_ENOMEM)
#define APR_STATUS_IS_EMFILE(s)         ((s) == APR_EMFILE \
                || (s) == APR_OS_START_SYSERR + ERROR_TOO_MANY_OPEN_FILES)
#define APR_STATUS_IS_ENFILE(s)         ((s) == APR_ENFILE)
#define APR_STATUS_IS_EBADF(s)          ((s) == APR_EBADF \
                || (s) == APR_OS_START_SYSERR + ERROR_INVALID_HANDLE)
#define APR_STATUS_IS_EINVAL(s)         ((s) == APR_EINVAL \
                || (s) == APR_OS_START_SYSERR + ERROR_INVALID_PARAMETER \
                || (s) == APR_OS_START_SYSERR + ERROR_INVALID_FUNCTION)
#define APR_STATUS_IS_ESPIPE(s)         ((s) == APR_ESPIPE \
                || (s) == APR_OS_START_SYSERR + ERROR_NEGATIVE_SEEK)
#define APR_STATUS_IS_EAGAIN(s)         ((s) == APR_EAGAIN \
                || (s) == APR_OS_START_SYSERR + ERROR_NO_DATA \
                || (s) == APR_OS_START_SYSERR + SOCEWOULDBLOCK \
                || (s) == APR_OS_START_SYSERR + ERROR_LOCK_VIOLATION)
#define APR_STATUS_IS_EINTR(s)          ((s) == APR_EINTR \
                || (s) == APR_OS_START_SYSERR + SOCEINTR)
#define APR_STATUS_IS_ENOTSOCK(s)       ((s) == APR_ENOTSOCK \
                || (s) == APR_OS_START_SYSERR + SOCENOTSOCK)
#define APR_STATUS_IS_ECONNREFUSED(s)   ((s) == APR_ECONNREFUSED \
                || (s) == APR_OS_START_SYSERR + SOCECONNREFUSED)
#define APR_STATUS_IS_EINPROGRESS(s)    ((s) == APR_EINPROGRESS \
                || (s) == APR_OS_START_SYSERR + SOCEINPROGRESS)
#define APR_STATUS_IS_ECONNABORTED(s)   ((s) == APR_ECONNABORTED \
                || (s) == APR_OS_START_SYSERR + SOCECONNABORTED)
#define APR_STATUS_IS_ECONNRESET(s)     ((s) == APR_ECONNRESET \
                || (s) == APR_OS_START_SYSERR + SOCECONNRESET)
#define APR_STATUS_IS_ETIMEDOUT(s)      ((s) == APR_ETIMEDOUT \
                || (s) == APR_OS_START_SYSERR + SOCETIMEDOUT)    
#define APR_STATUS_IS_EHOSTUNREACH(s)   ((s) == APR_EHOSTUNREACH \
                || (s) == APR_OS_START_SYSERR + SOCEHOSTUNREACH)
#define APR_STATUS_IS_ENETUNREACH(s)    ((s) == APR_ENETUNREACH \
                || (s) == APR_OS_START_SYSERR + SOCENETUNREACH)
#define APR_STATUS_IS_EFTYPE(s)         ((s) == APR_EFTYPE)
#define APR_STATUS_IS_EPIPE(s)          ((s) == APR_EPIPE \
                || (s) == APR_OS_START_SYSERR + ERROR_BROKEN_PIPE \
                || (s) == APR_OS_START_SYSERR + SOCEPIPE)
#define APR_STATUS_IS_EXDEV(s)          ((s) == APR_EXDEV \
                || (s) == APR_OS_START_SYSERR + ERROR_NOT_SAME_DEVICE)
#define APR_STATUS_IS_ENOTEMPTY(s)      ((s) == APR_ENOTEMPTY \
                || (s) == APR_OS_START_SYSERR + ERROR_DIR_NOT_EMPTY \
                || (s) == APR_OS_START_SYSERR + ERROR_ACCESS_DENIED)

	/*
	Sorry, too tired to wrap this up for OS2... feel free to
	fit the following into their best matches.

	{ ERROR_NO_SIGNAL_SENT,     ESRCH           },
	{ SOCEALREADY,              EALREADY        },
	{ SOCEDESTADDRREQ,          EDESTADDRREQ    },
	{ SOCEMSGSIZE,              EMSGSIZE        },
	{ SOCEPROTOTYPE,            EPROTOTYPE      },
	{ SOCENOPROTOOPT,           ENOPROTOOPT     },
	{ SOCEPROTONOSUPPORT,       EPROTONOSUPPORT },
	{ SOCESOCKTNOSUPPORT,       ESOCKTNOSUPPORT },
	{ SOCEOPNOTSUPP,            EOPNOTSUPP      },
	{ SOCEPFNOSUPPORT,          EPFNOSUPPORT    },
	{ SOCEAFNOSUPPORT,          EAFNOSUPPORT    },
	{ SOCEADDRINUSE,            EADDRINUSE      },
	{ SOCEADDRNOTAVAIL,         EADDRNOTAVAIL   },
	{ SOCENETDOWN,              ENETDOWN        },
	{ SOCENETRESET,             ENETRESET       },
	{ SOCENOBUFS,               ENOBUFS         },
	{ SOCEISCONN,               EISCONN         },
	{ SOCENOTCONN,              ENOTCONN        },
	{ SOCESHUTDOWN,             ESHUTDOWN       },
	{ SOCETOOMANYREFS,          ETOOMANYREFS    },
	{ SOCELOOP,                 ELOOP           },
	{ SOCEHOSTDOWN,             EHOSTDOWN       },
	{ SOCENOTEMPTY,             ENOTEMPTY       },
	{ SOCEPIPE,                 EPIPE           }
	*/

#elif defined(WIN32) && !defined(DOXYGEN) /* !defined(OS2) */

#define APR_FROM_OS_ERROR(e) (e == 0 ? APR_SUCCESS : e + APR_OS_START_SYSERR)
#define APR_TO_OS_ERROR(e)   (e == 0 ? APR_SUCCESS : e - APR_OS_START_SYSERR)

#define apr_get_os_error()   (APR_FROM_OS_ERROR(GetLastError()))
#define apr_set_os_error(e)  (SetLastError(APR_TO_OS_ERROR(e)))

	/* A special case, only socket calls require this:
	*/
#define apr_get_netos_error()   (APR_FROM_OS_ERROR(WSAGetLastError()))
#define apr_set_netos_error(e)   (WSASetLastError(APR_TO_OS_ERROR(e)))

#define APR_STATUS_IS_SUCCESS(s)           ((s) == APR_SUCCESS \
                || (s) == APR_OS_START_SYSERR + ERROR_SUCCESS)

	/* APR CANONICAL ERROR TESTS */
#define APR_STATUS_IS_EACCES(s)         ((s) == APR_EACCES \
                || (s) == APR_OS_START_SYSERR + ERROR_ACCESS_DENIED \
                || (s) == APR_OS_START_SYSERR + ERROR_CANNOT_MAKE \
                || (s) == APR_OS_START_SYSERR + ERROR_CURRENT_DIRECTORY \
                || (s) == APR_OS_START_SYSERR + ERROR_DRIVE_LOCKED \
                || (s) == APR_OS_START_SYSERR + ERROR_FAIL_I24 \
                || (s) == APR_OS_START_SYSERR + ERROR_LOCK_VIOLATION \
                || (s) == APR_OS_START_SYSERR + ERROR_LOCK_FAILED \
                || (s) == APR_OS_START_SYSERR + ERROR_NOT_LOCKED \
                || (s) == APR_OS_START_SYSERR + ERROR_NETWORK_ACCESS_DENIED \
                || (s) == APR_OS_START_SYSERR + ERROR_SHARING_VIOLATION)
#define APR_STATUS_IS_EEXIST(s)         ((s) == APR_EEXIST \
                || (s) == APR_OS_START_SYSERR + ERROR_FILE_EXISTS \
                || (s) == APR_OS_START_SYSERR + ERROR_ALREADY_EXISTS)
#define APR_STATUS_IS_ENAMETOOLONG(s)   ((s) == APR_ENAMETOOLONG \
                || (s) == APR_OS_START_SYSERR + ERROR_FILENAME_EXCED_RANGE \
                || (s) == APR_OS_START_SYSERR + WSAENAMETOOLONG)
#define APR_STATUS_IS_ENOENT(s)         ((s) == APR_ENOENT \
                || (s) == APR_OS_START_SYSERR + ERROR_FILE_NOT_FOUND \
                || (s) == APR_OS_START_SYSERR + ERROR_PATH_NOT_FOUND \
                || (s) == APR_OS_START_SYSERR + ERROR_OPEN_FAILED \
                || (s) == APR_OS_START_SYSERR + ERROR_NO_MORE_FILES)
#define APR_STATUS_IS_ENOTDIR(s)        ((s) == APR_ENOTDIR \
                || (s) == APR_OS_START_SYSERR + ERROR_PATH_NOT_FOUND \
                || (s) == APR_OS_START_SYSERR + ERROR_BAD_NETPATH \
                || (s) == APR_OS_START_SYSERR + ERROR_BAD_NET_NAME \
                || (s) == APR_OS_START_SYSERR + ERROR_BAD_PATHNAME \
                || (s) == APR_OS_START_SYSERR + ERROR_INVALID_DRIVE)
#define APR_STATUS_IS_ENOSPC(s)         ((s) == APR_ENOSPC \
                || (s) == APR_OS_START_SYSERR + ERROR_DISK_FULL)
#define APR_STATUS_IS_ENOMEM(s)         ((s) == APR_ENOMEM \
                || (s) == APR_OS_START_SYSERR + ERROR_ARENA_TRASHED \
                || (s) == APR_OS_START_SYSERR + ERROR_NOT_ENOUGH_MEMORY \
                || (s) == APR_OS_START_SYSERR + ERROR_INVALID_BLOCK \
                || (s) == APR_OS_START_SYSERR + ERROR_NOT_ENOUGH_QUOTA \
                || (s) == APR_OS_START_SYSERR + ERROR_OUTOFMEMORY)
#define APR_STATUS_IS_EMFILE(s)         ((s) == APR_EMFILE \
                || (s) == APR_OS_START_SYSERR + ERROR_TOO_MANY_OPEN_FILES)
#define APR_STATUS_IS_ENFILE(s)         ((s) == APR_ENFILE)
#define APR_STATUS_IS_EBADF(s)          ((s) == APR_EBADF \
                || (s) == APR_OS_START_SYSERR + ERROR_INVALID_HANDLE \
                || (s) == APR_OS_START_SYSERR + ERROR_INVALID_TARGET_HANDLE)
#define APR_STATUS_IS_EINVAL(s)         ((s) == APR_EINVAL \
                || (s) == APR_OS_START_SYSERR + ERROR_INVALID_ACCESS \
                || (s) == APR_OS_START_SYSERR + ERROR_INVALID_DATA \
                || (s) == APR_OS_START_SYSERR + ERROR_INVALID_FUNCTION \
                || (s) == APR_OS_START_SYSERR + ERROR_INVALID_HANDLE \
                || (s) == APR_OS_START_SYSERR + ERROR_INVALID_PARAMETER \
                || (s) == APR_OS_START_SYSERR + ERROR_NEGATIVE_SEEK)
#define APR_STATUS_IS_ESPIPE(s)         ((s) == APR_ESPIPE \
                || (s) == APR_OS_START_SYSERR + ERROR_SEEK_ON_DEVICE \
                || (s) == APR_OS_START_SYSERR + ERROR_NEGATIVE_SEEK)
#define APR_STATUS_IS_EAGAIN(s)         ((s) == APR_EAGAIN \
                || (s) == APR_OS_START_SYSERR + ERROR_NO_DATA \
                || (s) == APR_OS_START_SYSERR + ERROR_NO_PROC_SLOTS \
                || (s) == APR_OS_START_SYSERR + ERROR_NESTING_NOT_ALLOWED \
                || (s) == APR_OS_START_SYSERR + ERROR_MAX_THRDS_REACHED \
                || (s) == APR_OS_START_SYSERR + ERROR_LOCK_VIOLATION \
                || (s) == APR_OS_START_SYSERR + WSAEWOULDBLOCK)
#define APR_STATUS_IS_EINTR(s)          ((s) == APR_EINTR \
                || (s) == APR_OS_START_SYSERR + WSAEINTR)
#define APR_STATUS_IS_ENOTSOCK(s)       ((s) == APR_ENOTSOCK \
                || (s) == APR_OS_START_SYSERR + WSAENOTSOCK)
#define APR_STATUS_IS_ECONNREFUSED(s)   ((s) == APR_ECONNREFUSED \
                || (s) == APR_OS_START_SYSERR + WSAECONNREFUSED)
#define APR_STATUS_IS_EINPROGRESS(s)    ((s) == APR_EINPROGRESS \
                || (s) == APR_OS_START_SYSERR + WSAEINPROGRESS)
#define APR_STATUS_IS_ECONNABORTED(s)   ((s) == APR_ECONNABORTED \
                || (s) == APR_OS_START_SYSERR + WSAECONNABORTED)
#define APR_STATUS_IS_ECONNRESET(s)     ((s) == APR_ECONNRESET \
                || (s) == APR_OS_START_SYSERR + ERROR_NETNAME_DELETED \
                || (s) == APR_OS_START_SYSERR + WSAECONNRESET)
#define APR_STATUS_IS_ETIMEDOUT(s)      ((s) == APR_ETIMEDOUT \
                || (s) == APR_OS_START_SYSERR + WSAETIMEDOUT \
                || (s) == APR_OS_START_SYSERR + WAIT_TIMEOUT)
#define APR_STATUS_IS_EHOSTUNREACH(s)   ((s) == APR_EHOSTUNREACH \
                || (s) == APR_OS_START_SYSERR + WSAEHOSTUNREACH)
#define APR_STATUS_IS_ENETUNREACH(s)    ((s) == APR_ENETUNREACH \
                || (s) == APR_OS_START_SYSERR + WSAENETUNREACH)
#define APR_STATUS_IS_EFTYPE(s)         ((s) == APR_EFTYPE \
                || (s) == APR_OS_START_SYSERR + ERROR_EXE_MACHINE_TYPE_MISMATCH \
                || (s) == APR_OS_START_SYSERR + ERROR_INVALID_DLL \
                || (s) == APR_OS_START_SYSERR + ERROR_INVALID_MODULETYPE \
                || (s) == APR_OS_START_SYSERR + ERROR_BAD_EXE_FORMAT \
                || (s) == APR_OS_START_SYSERR + ERROR_INVALID_EXE_SIGNATURE \
                || (s) == APR_OS_START_SYSERR + ERROR_FILE_CORRUPT \
                || (s) == APR_OS_START_SYSERR + ERROR_BAD_FORMAT)
#define APR_STATUS_IS_EPIPE(s)          ((s) == APR_EPIPE \
                || (s) == APR_OS_START_SYSERR + ERROR_BROKEN_PIPE)
#define APR_STATUS_IS_EXDEV(s)          ((s) == APR_EXDEV \
                || (s) == APR_OS_START_SYSERR + ERROR_NOT_SAME_DEVICE)
#define APR_STATUS_IS_ENOTEMPTY(s)      ((s) == APR_ENOTEMPTY \
                || (s) == APR_OS_START_SYSERR + ERROR_DIR_NOT_EMPTY)

#elif defined(NETWARE) && !defined(DOXYGEN) /* !defined(OS2) && !defined(WIN32) */

#define APR_FROM_OS_ERROR(e) (e == 0 ? APR_SUCCESS : e + APR_OS_START_SYSERR)
#define APR_TO_OS_ERROR(e)   (e == 0 ? APR_SUCCESS : e - APR_OS_START_SYSERR)

#define apr_get_os_error()    (errno)
#define apr_set_os_error(e)   (errno = (e))

	/* A special case, only socket calls require this: */
#define apr_get_netos_error()   (APR_FROM_OS_ERROR(WSAGetLastError()))
#define apr_set_netos_error(e)  (WSASetLastError(APR_TO_OS_ERROR(e)))

#define APR_STATUS_IS_SUCCESS(s)           ((s) == APR_SUCCESS)

	/* APR CANONICAL ERROR TESTS */
#define APR_STATUS_IS_EACCES(s)         ((s) == APR_EACCES)
#define APR_STATUS_IS_EEXIST(s)         ((s) == APR_EEXIST)
#define APR_STATUS_IS_ENAMETOOLONG(s)   ((s) == APR_ENAMETOOLONG)
#define APR_STATUS_IS_ENOENT(s)         ((s) == APR_ENOENT)
#define APR_STATUS_IS_ENOTDIR(s)        ((s) == APR_ENOTDIR)
#define APR_STATUS_IS_ENOSPC(s)         ((s) == APR_ENOSPC)
#define APR_STATUS_IS_ENOMEM(s)         ((s) == APR_ENOMEM)
#define APR_STATUS_IS_EMFILE(s)         ((s) == APR_EMFILE)
#define APR_STATUS_IS_ENFILE(s)         ((s) == APR_ENFILE)
#define APR_STATUS_IS_EBADF(s)          ((s) == APR_EBADF)
#define APR_STATUS_IS_EINVAL(s)         ((s) == APR_EINVAL)
#define APR_STATUS_IS_ESPIPE(s)         ((s) == APR_ESPIPE)

#define APR_STATUS_IS_EAGAIN(s)         ((s) == APR_EAGAIN \
                || (s) ==                       EWOULDBLOCK \
                || (s) == APR_OS_START_SYSERR + WSAEWOULDBLOCK)
#define APR_STATUS_IS_EINTR(s)          ((s) == APR_EINTR \
                || (s) == APR_OS_START_SYSERR + WSAEINTR)
#define APR_STATUS_IS_ENOTSOCK(s)       ((s) == APR_ENOTSOCK \
                || (s) == APR_OS_START_SYSERR + WSAENOTSOCK)
#define APR_STATUS_IS_ECONNREFUSED(s)   ((s) == APR_ECONNREFUSED \
                || (s) == APR_OS_START_SYSERR + WSAECONNREFUSED)
#define APR_STATUS_IS_EINPROGRESS(s)    ((s) == APR_EINPROGRESS \
                || (s) == APR_OS_START_SYSERR + WSAEINPROGRESS)
#define APR_STATUS_IS_ECONNABORTED(s)   ((s) == APR_ECONNABORTED \
                || (s) == APR_OS_START_SYSERR + WSAECONNABORTED)
#define APR_STATUS_IS_ECONNRESET(s)     ((s) == APR_ECONNRESET \
                || (s) == APR_OS_START_SYSERR + WSAECONNRESET)
#define APR_STATUS_IS_ETIMEDOUT(s)      ((s) == APR_ETIMEDOUT \
                || (s) == APR_OS_START_SYSERR + WSAETIMEDOUT \
                || (s) == APR_OS_START_SYSERR + WAIT_TIMEOUT)
#define APR_STATUS_IS_EHOSTUNREACH(s)   ((s) == APR_EHOSTUNREACH \
                || (s) == APR_OS_START_SYSERR + WSAEHOSTUNREACH)
#define APR_STATUS_IS_ENETUNREACH(s)    ((s) == APR_ENETUNREACH \
                || (s) == APR_OS_START_SYSERR + WSAENETUNREACH)
#define APR_STATUS_IS_ENETDOWN(s)       ((s) == APR_OS_START_SYSERR + WSAENETDOWN)
#define APR_STATUS_IS_EFTYPE(s)         ((s) == APR_EFTYPE)
#define APR_STATUS_IS_EPIPE(s)          ((s) == APR_EPIPE)
#define APR_STATUS_IS_EXDEV(s)          ((s) == APR_EXDEV)
#define APR_STATUS_IS_ENOTEMPTY(s)      ((s) == APR_ENOTEMPTY)

#else /* !defined(NETWARE) && !defined(OS2) && !defined(WIN32) */

	/*
	*  os error codes are clib error codes
	*/
#define APR_FROM_OS_ERROR(e)  (e)
#define APR_TO_OS_ERROR(e)    (e)

#define apr_get_os_error()    (errno)
#define apr_set_os_error(e)   (errno = (e))

	/* A special case, only socket calls require this:
	*/
#define apr_get_netos_error() (errno)
#define apr_set_netos_error(e) (errno = (e))

#define APR_STATUS_IS_SUCCESS(s)        ((s) == APR_SUCCESS)

#define APR_STATUS_IS_EACCES(s)         ((s) == APR_EACCES)

#define APR_STATUS_IS_EEXIST(s)         ((s) == APR_EEXIST)

#define APR_STATUS_IS_ENAMETOOLONG(s)   ((s) == APR_ENAMETOOLONG)

#ifdef EMVSCATLG
#define APR_STATUS_IS_ENOENT(s)         ((s) == APR_ENOENT \
                                      || (s) == EMVSCATLG)
#else
#define APR_STATUS_IS_ENOENT(s)         ((s) == APR_ENOENT)
#endif

#define APR_STATUS_IS_ENOTDIR(s)        ((s) == APR_ENOTDIR)

#define APR_STATUS_IS_ENOSPC(s)         ((s) == APR_ENOSPC)

#define APR_STATUS_IS_ENOMEM(s)         ((s) == APR_ENOMEM)

#define APR_STATUS_IS_EMFILE(s)         ((s) == APR_EMFILE)

#define APR_STATUS_IS_ENFILE(s)         ((s) == APR_ENFILE)

#define APR_STATUS_IS_EBADF(s)          ((s) == APR_EBADF)

#define APR_STATUS_IS_EINVAL(s)         ((s) == APR_EINVAL)

#define APR_STATUS_IS_ESPIPE(s)         ((s) == APR_ESPIPE)

#if !defined(EWOULDBLOCK) || !defined(EAGAIN)
#define APR_STATUS_IS_EAGAIN(s)         ((s) == APR_EAGAIN)
#elif (EWOULDBLOCK == EAGAIN)
#define APR_STATUS_IS_EAGAIN(s)         ((s) == APR_EAGAIN)
#else
#define APR_STATUS_IS_EAGAIN(s)         ((s) == APR_EAGAIN \
                                      || (s) == EWOULDBLOCK)
#endif

#define APR_STATUS_IS_EINTR(s)          ((s) == APR_EINTR)

#define APR_STATUS_IS_ENOTSOCK(s)       ((s) == APR_ENOTSOCK)

#define APR_STATUS_IS_ECONNREFUSED(s)   ((s) == APR_ECONNREFUSED)

#define APR_STATUS_IS_EINPROGRESS(s)    ((s) == APR_EINPROGRESS)

#ifdef EPROTO
#define APR_STATUS_IS_ECONNABORTED(s)    ((s) == APR_ECONNABORTED \
                                       || (s) == EPROTO)
#else
#define APR_STATUS_IS_ECONNABORTED(s)    ((s) == APR_ECONNABORTED)
#endif

#define APR_STATUS_IS_ECONNRESET(s)      ((s) == APR_ECONNRESET)

#define APR_STATUS_IS_ETIMEDOUT(s)       ((s) == APR_ETIMEDOUT)    

#define APR_STATUS_IS_EHOSTUNREACH(s)    ((s) == APR_EHOSTUNREACH)

#define APR_STATUS_IS_ENETUNREACH(s)     ((s) == APR_ENETUNREACH)

#define APR_STATUS_IS_EFTYPE(s)          ((s) == APR_EFTYPE)

#define APR_STATUS_IS_EPIPE(s)           ((s) == APR_EPIPE)

#define APR_STATUS_IS_EXDEV(s)           ((s) == APR_EXDEV)

#define APR_STATUS_IS_ENOTEMPTY(s)       ((s) == APR_ENOTEMPTY || \
                                          (s) == APR_EEXIST)

#endif /* !defined(NETWARE) && !defined(OS2) && !defined(WIN32) */

#ifdef __cplusplus
}
#endif

#endif  /* ! APR_ERRNO_H */