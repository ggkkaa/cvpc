#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include "util.h"
#include "args.h"
#include "cc.h"

static void usage() {
        fprintf(stderr, "usage: %s [input]\n", argv0);
}

int main(int argc, char **argv) {
        char *output = NULL, *target = NULL;
        
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

        if (target) {
                printf("Target: %s\n", target);
        } else {
                printf("No target specified\n");
        }
        
        targinit(target);

        if (output) {
                printf("Output: %s\n", output);
                if (!freopen(output, "w", stdout)) {
                        fatal("open %s:", output);
                }
        } else {
                printf("No output specified\n");
        }


	if (output && !freopen(output, "w", stdout))
                fatal("open %s:", output);

        if(argc) {
                printf("scanning args\n");
                while (argc--) {
                        printf("Scanning: %s\n", argv[argc]);
                        scanfrom(argv[argc], NULL);
                }
                openscan();
        } else {
                printf("No arguments to scan\n");
        }

        return 0;
}