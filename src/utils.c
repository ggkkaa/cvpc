#include <assert.h>
#include <errno.h>
#include <stdarg.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "util.h"

char* argv0;

static void vwarn(const char *fmt, va_list args) {
        fprintf(stderr, "%s: ", argv0);
        vfprintf(stderr, fmt, args);
        if (fmt[0] && fmt[strlen(fmt) - 1] == ':') {
                fputc(' ', stderr);
                perror(NULL);
        } else {
                fputc('\n', stderr);
        }
        
}

void fatal(const char *fmt, ...) {
        va_list args;
        
        va_start(args, fmt);
        vwarn(fmt, args);
        va_end(args);
        exit(1);
}

void *reallocarray(void *buf, size_t n, size_t m) {
	if (n > 0 && SIZE_MAX / n < m) {
		errno = ENOMEM;
		return NULL;
	}
	return realloc(buf, n * m);
}

void *xreallocarray(void *buf, size_t n, size_t m) {
	buf = reallocarray(buf, n, m);
	if (!buf && n && m)
		fatal("reallocarray:");

	return buf;
}

void *xmalloc(size_t len) {
        void *buf;
        buf = malloc(len);
        if(!buf && len) {
                fatal("malloc:");
        }
        return buf;
}


char *progname(char *name, char *fallback) {
	char *slash;

	if (!name)
		return fallback;
	slash = strrchr(name, '/');
	return slash ? slash + 1 : name;
}