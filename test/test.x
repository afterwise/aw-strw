
#include "aw-strw.h"
#include <assert.h>

static char mem[5];

int main(int argc, char *argv[]) {
	(void) argc;
	(void) argv;

	struct strwbuf buf;

	strwbuf_init(&buf, mem, sizeof mem);
	assert(strlw(&buf, "abc123") == -1);
	assert(strlw(&buf, "abc12") == -1);
	assert(strlw(&buf, "abc1") == 4);
	assert(strlw(&buf, "") == 0);

	strwbuf_init(&buf, mem, sizeof mem);
	assert(strnw(&buf, "abc123", 6) == -1);
	assert(strnw(&buf, "abc123", 5) == -1);
	assert(strnw(&buf, "abc123", 4) == 4);
	assert(strnw(&buf, "abc123", 0) == 0);

	strwbuf_init(&buf, mem, sizeof mem);
	assert(strw(&buf, "abc123") == -1);
	assert(strw(&buf, "abc12") == -1);
	assert(strw(&buf, "abc1") == 4);
	assert(strw(&buf, "") == 0);

	strwbuf_init(&buf, mem, sizeof mem);
	assert(strwf(&buf, "%s", "abc123") == -1);
	assert(strwf(&buf, "%.5s", "abc123") == -1);
	assert(strwf(&buf, "%.4s", "abc123") == 4);
	assert(strwf(&buf, "%.0s", "abc123") == 0);

	printf("OK\n");
	return 0;
}

