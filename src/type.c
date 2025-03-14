#include <assert.h>
#include <stdbool.h>
#include <stddef.h>
#include <string.h>
#include "util.h"
#include "cc.h"

#define INTTYPE(type, s, is_signed, property) { \
	.kind = type, .size = s, .align = s, .u.basic.issigned = is_signed, \
	.prop = PROPSCALAR|PROPARITH|PROPREAL|PROPINT|property, \
}

struct type typevoid = { .kind = TYPEVOID, .incomplete = true };

struct type typechar = INTTYPE(TYPECHAR, 1, true, PROPCHAR);

struct type typeint = INTTYPE(TYPEINT, 4, true, 0);
struct type typeuint = INTTYPE(TYPEINT, 4, false, 0);

struct type *typeadjvalist;

struct type *mktype(enum typekind kind, enum typeprop prop)
{
	struct type *type;

	type = xmalloc(sizeof(*type));
	type->kind = kind;
	type->prop = prop;
	type->value = NULL;
	type->incomplete = false;
	type->flexible = false;

	return type;
}


struct type *mkptrtype(struct type *base, enum typequal qual) {
        struct type *type;

        type = mktype(TYPEPTR, PROPSCALAR);
        type->base = base;
        type->qual = qual;
        type->size = 8;
        type->align = 8;
        if (base)
                type->prop |= base->prop & PROPVM;

        return type;
}

struct type *typeadjust(struct type *type, enum typequal *qual) {
        enum typequal ptrqual; 

        if (!type) {
                fprintf(stderr, "Error: type is NULL\n");
                return NULL;
        }

        printf("typeadjust: type->kind = %d, type->qual = %d\n", type->kind, type->qual);

        switch (type->qual) {
        case TYPEARRAY:
                ptrqual = type->u.array.ptrqual;
                
                if (!type->base) {
                        fprintf(stderr, "Error: type->base is NULL\n");
                        return NULL;
                }
                
                type = mkptrtype(type->base, *qual | type->qual);
                *qual = ptrqual;
                break;
        case TYPEFUNC:
                assert(*qual == QUALNONE);
                type = mkptrtype(type, QUALNONE);
                break;
        }
        
        return type;
}
