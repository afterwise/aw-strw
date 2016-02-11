
/*
   Copyright (c) 2014-2016 Malte Hildingsson, malte (at) afterwi.se

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

#include <stdarg.h>
#include <stdio.h>
#include <string.h>

#if __GNUC__
# define _strw_format(a,b) __attribute__((format(printf,a,b)))
# define _strw_alwaysinline inline __attribute__((always_inline))
# define _strw_unused __attribute__((unused))
#elif _MSC_VER
# define _strw_format(a,b)
# define _strw_alwaysinline __forceinline
# define _strw_unused
#endif

#ifdef __cplusplus
extern "C" {
#endif

struct strwbuf {
	char *str;
	size_t size;
	size_t len;
};

static _strw_alwaysinline void strwbuf_init(struct strwbuf *buf, char *str, size_t size) {
	buf->str = str;
	buf->size = size;
	buf->len = 0;
}

#define strlw(buf,str) strnw((buf), (str), sizeof (str) - 1)

_strw_unused
static ssize_t strnw(struct strwbuf *buf, char *str, size_t n) {
	if (buf->size > buf->len + n) {
		memcpy(buf->str + buf->len, str, n);
		return buf->str[buf->len += n] = 0, n;
	}
	return -1;
}

_strw_unused
static ssize_t strw(struct strwbuf *buf, char *str) {
	return strnw(buf, str, strlen(str));
}

_strw_unused _strw_format(2, 3)
static ssize_t strwf(struct strwbuf *buf, char *fmt, ...) {
	ssize_t err;
	va_list ap;
	va_start(ap, fmt);
	err = vsnprintf(buf->str + buf->len, buf->size - buf->len, fmt, ap);
	va_end(ap);
	return (err >= 0 && buf->size > buf->len + err) ? buf->len += err, err : -1;
}

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif /* AW_STRW_H */

