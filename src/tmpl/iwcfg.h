#pragma once
#ifndef IW_CFG_H
#define IW_CFG_H

//
/**************************************************************************************************
 * IOWOW library
 *
 * MIT License
 *
 * Copyright (c) 2012-2021 Softmotions Ltd <info@softmotions.com>
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 *  copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 *************************************************************************************************/


#include "../basedefs.h"

#undef STR_HELPER
#define STR_HELPER(x) #x

#undef STR
#define STR(x)        STR_HELPER(x)

#include <stddef.h>
#include <stdint.h>
#include <inttypes.h>
#include <assert.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <limits.h>
#include <time.h>
#include <math.h>

#include <stdarg.h>
#include <ctype.h>

typedef intmax_t ssize_t;

// DBJ
#ifndef _WIN32
#include <unistd.h>
#else
#include <io.h>
#include <process.h>
#endif

#ifndef MAXPATHLEN
#ifdef PATH_MAX
#define MAXPATHLEN PATH_MAX
#else
#define MAXPATHLEN 4096
#endif
#endif

// DBJ
#define IOWOW_VERSION       "0.0.0"
#define IOWOW_VERSION_MAJOR 0
#define IOWOW_VERSION_MINOR 0
#define IOWOW_VERSION_PATCH 0

#ifndef static_assert
#define static_assert _Static_assert
#endif


 // DBJ need it in current cl
extern char* dbj_strndup(const char*, size_t);
/*
Extension char* strndup (const char *@var{s}, size_t @var{n})

Returns a pointer to a copy of @var{s} with at most @var{n} characters
in memory obtained from @code{malloc}, or @code{NULL} if insufficient
memory was available.  The result is always @code{NULL} terminated.

*/

inline char* dbj_strndup(const char* s, size_t n)
{
	char* result;
	size_t len = strlen(s);

	if (n < len)
		len = n;

	result = malloc(len + 1);
	if (!result)
		return 0;

	result[len] = '\0';
	return memcpy(result, s, len);
}

#define strndup dbj_strndup

#include <log/iwlog.h> // DBJ

#endif // IW_CFG_H
