#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#define ARENA_IMPLEMENTATION
#include "3rdparty/arena.h"
#include "util.h"
#include "args.h"
#include "cc.h"

Arena arena;

static void usage() {
        fprintf(stderr, "usage: %s [input]\n", argv0);
}

int main(int argc, char **argv) {
        char *output = NULL, *target = NULL;
        bool *fail = false;

        argv0 = progname(argv[0], "cvpc");

        ARGBEGIN {
                case 't':
                        target = EARGF(usage());
                        break;
                case 'o':
                        output = EARGF(usage());
                        break;
                default:
                        usage();
        } ARGEND
        
        targinit(target);

        if (output) {
                printf("Output: %s\n", output);
                if (!freopen(output, "w", stdout)) {
			fatal("open %s:", output);
                }
        }

        if(argc) {
                while (argc--) {
                        scanfrom(argv[argc], NULL);
                }
                openscan();
        }
        scopeinit();
        
        struct token_arr token_array;

        while(tok.kind != TEOF) {
                scanline(token_array);
        }

        delete_arenas();
        return 0;
}
