
/*
   Copyright (c) 2014 Malte Hildingsson, malte (at) afterwi.se

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

#include <sys/types.h>

#if __GNUC__
# define _strw_format(a,b) __attribute__((format(__printf__,a,b)))
# define _strw_alwaysinline inline __attribute__((always_inline))
#elif _MSC_VER
# define _strw_format(a,b)
# define _strw_alwaysinline __forceinline
#endif

#ifdef __cplusplus
extern "C" {
#endif

struct strwbuf {
	char *ptr;
	size_t size;
	size_t len;
};

#define strkw(buf,str) strnw((buf), (str), sizeof (str) - 1)

static _strw_alwaysinline void strwbuf_init(struct strwbuf *buf, char *ptr, size_t size) {
	buf->ptr = ptr;
	buf->size = size;
	buf->len = 0;
}

ssize_t strw(struct strwbuf *buf, char *str);
ssize_t strnw(struct strwbuf *buf, char *str, size_t n);
ssize_t strwf(struct strwbuf *buf, char *fmt, ...) _strw_format(2, 3);

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif /* AW_STRW_H */

