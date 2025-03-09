#include <assert.h>
#include "cc.h"

struct qualtype {
        struct type *type;
        enum typequal qual;
        struct expr *expr;
};

static bool staticasset(struct scope *scope) {
        if(!consume(TSTATIC_ASSERT))
                return false;
        expect();
}

bool decl(struct scope *scope, struct func *function) {
        struct qualtype base, qt;
        struct type *type;
        enum typequal tq;

        if(staticassert(scope))
}