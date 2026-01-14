
/*
   Copyright (c) 2014-2025 Malte Hildingsson, malte (at) afterwi.se

   Permission is hereby granted, free of charge, to any person obtaining a copy
   of this software and associated documentation files (the "Software"), to deal
   in the Software without restriction, including without limitation the rights
   to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
   copies of the Software, and to permit persons to whom the Software is
   furnished to do so, subject to the following conditions:

   The above copyright notice and this permission notice shall be included in
   all copies or substantial portions of the Software.

   THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
   IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
   FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
   AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
   LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
   OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
   THE SOFTWARE.
 */

#ifndef AW_STRW_H
#define AW_STRW_H

#if !_MSC_VER || _MSC_VER >= 1800
# include <stdbool.h>
#endif

#include <sys/types.h>
#include <stdarg.h>
#include <stdio.h>
#include <string.h>

#if defined(__GNUC__)
# define _strw_format(a,b) __attribute__((format(printf,a,b)))
# define _strw_alwaysinline __attribute__((always_inline))
# define _strw_malloc __attribute__((malloc))
# define _strw_unused __attribute__((unused))
#elif defined(_MSC_VER)
# define _strw_format(a,b)
# define _strw_alwaysinline __forceinline
# define _strw_malloc
# define _strw_unused
#endif

#ifdef __cplusplus
extern "C" {
#endif

#if defined(_MSC_VER)
typedef signed __int64 strw_ssize_t;
#else
typedef ssize_t strw_ssize_t;
#endif

struct strwbuf {
	char *__restrict str;
	size_t size;
	size_t len;
};

_strw_unused _strw_alwaysinline
static void strwbuf_init(struct strwbuf *__restrict buf, char *__restrict str, size_t size) {
	buf->str = str;
	buf->size = size;
	buf->len = 0;
}

/* test if there are at least n+1 bytes left in the buffer */

_strw_unused
static inline bool strwsz(struct strwbuf *__restrict buf, size_t n) {
	return buf->size > buf->len + n;
}

/* allocate buffer space, return pointer */

_strw_unused _strw_malloc
static inline char *strwp(struct strwbuf *__restrict buf, size_t n) {
	if (strwsz(buf, n))
		return buf->len += n, buf->str;
	return NULL;
}

/* write literal string */

#define strlw(buf,str) strnw((buf), (str), sizeof (str) - 1)

/* write n characters */

_strw_unused
static inline strw_ssize_t strnw(struct strwbuf *__restrict buf, const char *__restrict str, size_t n) {
	if (strwsz(buf, n)) {
		memcpy(buf->str + buf->len, str, n);
		return buf->str[buf->len += n] = 0, n;
	}
	return -1;
}

/* write string */

_strw_unused
static inline strw_ssize_t strw(struct strwbuf *__restrict buf, const char *__restrict str) {
	return strnw(buf, str, strlen(str));
}

/* write formatted string */

_strw_unused _strw_format(2, 3)
static inline strw_ssize_t strwf(struct strwbuf *__restrict buf, const char *__restrict fmt, ...) {
	strw_ssize_t err;
	va_list ap;
	va_start(ap, fmt);
	err = vsnprintf(buf->str + buf->len, buf->size - buf->len, fmt, ap);
	va_end(ap);
	return (err >= 0 && strwsz(buf, err)) ? buf->len += err, err : -1;
}

/* assemble (join) string from pieces */

struct strwap {
	const char *__restrict str;
	size_t len;
};

_strw_unused
static inline strw_ssize_t strwa(struct strwbuf *__restrict buf, const struct strwap* ap) {
	size_t n = 0;
	for (size_t i = 0; ap[i].str != NULL; ++i)
		n += ap[i].len;
	char *__restrict p = strwp(buf, n);
	if (p != NULL) {
		for (size_t i = 0; ap[i].str != NULL; ++i, p += ap[i].len)
			memcpy(p, ap[i].str, ap[i].len);
		return n;
	}
	return -1;
}

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif /* AW_STRW_H */

