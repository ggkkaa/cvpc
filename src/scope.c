#include "util.h"
#include "cc.h"

struct scope filescope;

void scopeinit() {
        static struct decl builtins[] = {
                {.name = "__builtin_alloc", .kind = DECLBUILTIN, .u.builtin = BUILTINALLOC},
                {.name = "__builtin_is_constant", .kind = DECLBUILTIN, .u.builtin = BUILTINISCONSTANT},
                {.name = "__builtin_expect", .kind = DECLBUILTIN, .u.builtin = BUILTINEXPECT},
                {.name = "__builtin_infinite", .kind = DECLBUILTIN, .u.builtin = BUILTININFINITE},
                {.name = "__builtin_nil", .kind = DECLBUILTIN, .u.builtin = BUILTINNIL},
                {.name = "__builtin_offset_of", .kind = DECLBUILTIN, .u.builtin = BUILTINOFFSETOF},
                {.name = "__builtin_types_compatible", .kind = DECLBUILTIN, .u.builtin = BUILTINTYPESCOMPATIBLE},
                {.name = "__builtin_unreachable", .kind = DECLBUILTIN, .u.builtin = BUILTINUNREACHABLE},
                {.name = "__builtin_va_arg", .kind = DECLBUILTIN, .u.builtin = BUILTINVAARG},
                {.name = "__builtin_va_copy", .kind = DECLBUILTIN, .u.builtin = BUILTINVACOPY},
                {.name = "__builtin_va_end", .kind = DECLBUILTIN, .u.builtin = BUILTINVAEND},
                {.name = "__builtin_va_start", .kind = DECLBUILTIN, .u.builtin = BUILTINVASTART},
        };
        static struct decl valist;
        struct decl *decl;

        for (decl = builtins; decl < builtins + LEN(builtins); ++decl) 
                scopeputdecl(&filescope, decl);
        valist.name = "__builtin_va_list";
        valist.kind = DECLTYPE;
        valist.type = targ->typevalist;
        scopeputdecl(&filescope, &valist);
}


void scopeputdecl(struct scope *scope, struct decl *decl) {
	struct mapkey key;

	if (!scope->decls.len)
		mapinit(&scope->decls, 32);
	mapkey(&key, decl->name, strlen(decl->name));
	*mapput(&scope->decls, &key) = decl;
}