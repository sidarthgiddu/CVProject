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

#ifndef APR_LIB_H
#define APR_LIB_H

#include "apr.h"
#include "apr_errno.h"

#if APR_HAVE_CTYPE_H
#include <ctype.h>
#endif
#if APR_HAVE_STDARG_H
#include <stdarg.h>
#endif

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

#define HUGE_STRING_LEN 8


/*
 * Define the structures used by the APR general-purpose library.
 */

typedef struct apr_vformatter_buff_t apr_vformatter_buff_t;

struct apr_vformatter_buff_t {
    char *curpos;
    char *endpos;
};

APR_DECLARE(const char *) apr_filepath_name_get(const char *pathname);

APR_DECLARE(const char *) apr_filename_of_pathname(const char *pathname);

#ifdef WIN32
#define apr_killpg(x, y)
#else /* WIN32 */
#ifdef NO_KILLPG
#define apr_killpg(x, y)        (kill (-(x), (y)))
#else /* NO_KILLPG */
#define apr_killpg(x, y)        (killpg ((x), (y)))
#endif /* NO_KILLPG */
#endif /* WIN32 */

APR_DECLARE(int) apr_vformatter(int (*flush_func)(apr_vformatter_buff_t *b),
                                apr_vformatter_buff_t *c, const char *fmt,
                                va_list ap);

APR_DECLARE(apr_status_t) apr_password_get(const char *prompt, char *pwbuf, 
                                           apr_size_t *bufsize);

#define apr_isalnum(c) (isalnum(((unsigned char)(c))))

#define apr_isalpha(c) (isalpha(((unsigned char)(c))))

#define apr_iscntrl(c) (iscntrl(((unsigned char)(c))))

#define apr_isdigit(c) (isdigit(((unsigned char)(c))))

#define apr_isgraph(c) (isgraph(((unsigned char)(c))))

#define apr_islower(c) (islower(((unsigned char)(c))))

#ifdef isascii
#define apr_isascii(c) (isascii(((unsigned char)(c))))
#else
#define apr_isascii(c) (((c) & ~0x7f)==0)
#endif

#define apr_isprint(c) (isprint(((unsigned char)(c))))

#define apr_ispunct(c) (ispunct(((unsigned char)(c))))

#define apr_isspace(c) (isspace(((unsigned char)(c))))

#define apr_isupper(c) (isupper(((unsigned char)(c))))

#define apr_isxdigit(c) (isxdigit(((unsigned char)(c))))

#define apr_tolower(c) (tolower(((unsigned char)(c))))

#define apr_toupper(c) (toupper(((unsigned char)(c))))

#ifdef __cplusplus
}
#endif

#endif  /* ! APR_LIB_H */