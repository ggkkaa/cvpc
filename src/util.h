#pragma once
#include <stddef.h>

struct list {
	struct list *prev, *next;
};

#define LEN(a) (sizeof(a) / sizeof((a)[0]))

extern char *argv0;

void fatal(const char *fmt, ...);

void *xreallocarray(void *buf, size_t n, size_t m);
void *xmalloc(size_t len);  

char *progname(char *name, char *fallback);

struct map {
        size_t len, cap;
        struct mapkey *keys;
        void **vals;
};

struct mapkey {
        unsigned long hash;
        const void *str;
        size_t len;
};

void **mapput(struct map *map, struct mapkey *key);