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

/* Hashes a value with the FNV-1a hash algorithm. */
static unsigned long hash(const void *ptr, size_t len) {
        unsigned long h;
        const unsigned char *pos, *end;

        h = 0x811c9dc5;
        for(pos = ptr, end = pos + len; pos != end; ++pos)
                h = (h ^ *pos) * 0x1000193;
        return h;
}

void mapkey(struct mapkey *key, const void *string, size_t len) {
        key->str = string;
        key->len = len;
        key->hash = hash(string, len);
}

void mapinit(struct map *map, size_t cap) {
        assert(!(cap & cap - 1));
        map->len = 0;
        map->cap = cap;
        map->keys = xreallocarray(NULL, cap, sizeof(map->keys[0]));
        map->vals = xreallocarray(NULL, cap, sizeof(map->vals[0]));

        for(size_t i = 0; i < cap; ++i)
                map->keys[i].str = NULL;
}

static bool keyequal(struct mapkey *key_1, struct mapkey *key_2) {
        if(key_1->hash != key_2->hash) 
                return false;
        return memcmp(key_1->str, key_2->str, key_1->len) == 0;
}       

static size_t keyindex(struct map *map, struct mapkey *key) {
        size_t i;

        i = key->hash & map->cap - 1;
        while (map->keys[i].str && !keyequal(&map->keys[i], key))
                i = i + 1 & map->cap - 1;
        return i;
}

void **mapput(struct map *map, struct mapkey *key) {
        struct mapkey *old_keys;
        void **old_vals;
        size_t old_cap;

        if(map->cap / 2 < map->len) {
                old_keys = map->keys;
                old_vals = map->vals;
                old_cap = map->cap;
                map->cap *= 2;
                map->keys = xreallocarray(NULL, map->cap, sizeof(map->keys[0]));
                map->vals = xreallocarray(NULL, map->cap, sizeof(map->vals[0]));
                for(size_t i = 0; i < map->cap; ++i)
                        map->keys[i].str = NULL;
                for(size_t i = 0; i < old_cap; ++i) {
                        if(old_keys[i].str) {
                                size_t j = keyindex(map, &old_keys[i]);
                                map->keys[j] = old_keys[i];
                                map->vals[j] = old_vals[i];
                        }
                }
                free(old_keys);
                free(old_vals);
        }
        size_t i = keyindex(map, key);
        if(!map->keys[i].str) {
                map->keys[i] = *key;
                map->vals[i] = NULL;
                ++map->len;
        }

        return &map->vals[i];
}