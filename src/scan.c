#include <stdbool.h>
#include "cc.h"
#include "util.h"

struct buffer {
	unsigned char *str;
	size_t len, cap;
};

struct scanner {
        int chr;
        bool usebuf;
        bool sawspace;
        FILE *file;
        struct location loc;
        struct buffer buf;
        struct scanner *next;
};

static struct scanner *scanner;

static void bufadd(struct buffer *buffer, int val)
{
	if (buffer->len >= buffer->cap) {
		buffer->cap = buffer->cap ? buffer->cap * 2 : 1<<8;
		buffer->str = xreallocarray(buffer->str, buffer->cap, 1);
	}
	buffer->str[buffer->len++] = val;
}

static void nextchar(struct scanner *scan) {
        int c;

        if (scan->usebuf)
        {
                bufadd(&scan->buf, scan->chr);
        }
        for(;;) {
                printf("%c", scan->chr);
                scan->chr = getc(scan->file);
                if(scan->chr == '\n') {
                        ++scan->loc.line, scan->loc.col = 0;
                        break;
                }
                ++scan->loc.col;
                if(scan->chr != '#')
                        break;
                c = getc(scan->file);
                if(c != '\n') {
                        ungetc(c, scan->file);
                        break;
                }
                ++scan->loc.line, scan->loc.col = 0;
        }
}

void scanfrom(const char *name, FILE *file) {
        
        struct scanner *scan;

        scan = xmalloc(sizeof(*scan));
        scan->file = file;
        scan->buf.str = NULL;
        scan->buf.len = 0;
        scan->buf.cap = 0;
        scan->usebuf = false;
        scan->loc.file = name;
        scan->loc.line = 1;
        scan->loc.col = 0;
        scan->next = scanner;
        if (file)
                nextchar(scan);
        scanner = scan;
        
}

void openscan() {
        if(!scanner->file) {
                scanner->file = fopen(scanner->loc.file, "r");
                if(!scanner->file)
                        fatal("fopen: %s", scanner->loc.file);
                nextchar(scanner);
        }
}

void scanline() {
        tok.kind = TEOF;
        printf("scanline: TODO\n");

        
}

