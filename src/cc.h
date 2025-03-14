#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include "util.h"

struct func;

enum tokenkind {
        TNONE,

        TEOF,
        TNEWLINE,
        TOTHER,

        TIDENT,
        TNUMBER,
        TCHARCONST,
        TSTRINGLIT,

        TRETURN
};


enum typekind {
        TYPENONE,
        
        TYPEVOID,
        TYPECHAR,
        TYPEINT,
        TYPEPTR,
        TYPEARRAY,
        TYPEFUNC,
        TYPESTRUCT
};

enum typeprop {
        PROPNONE,

        PROPCHAR = 1<<0,
        PROPINT = 1<<1,
	PROPREAL = 1<<2,
	PROPARITH = 1<<3,
        PROPSCALAR = 1<<4,
        PROPFLOAT = 1<<5,
        PROPVM = 1<<6
};

enum typequal {
	QUALNONE,

	QUALCONST    = 1<<1,
	QUALRESTRICT = 1<<2,
	QUALVOLATILE = 1<<3,
	QUALATOMIC   = 1<<4
};


struct type {
	enum typekind kind;
	enum typeprop prop;
	int align;
	unsigned long long size;
	struct value *value;
	struct type *base;
	struct list link;
	enum typequal qual;
	bool incomplete, flexible;
	union {
		struct {
			bool issigned, iscomplex;
		} basic;
		struct {
			struct expr *length;
			enum typequal ptrqual;
			struct value *size;
		} array;
		struct {
			bool isvararg;
			struct decl *params;
			size_t nparam;
		} func;
		struct {
			char *tag;
			struct member *members;
		} structunion;
	} u;
};

enum declkind {
        DECLTYPE,
        DECLOBJECT,
        DECLBUILTIN
};

enum builtinkind {
        BUILTINALLOC,
        BUILTINISCONSTANT,
        BUILTINEXPECT,
        BUILTININFINITE,
        BUILTINNIL,
        BUILTINOFFSETOF,
        BUILTINTYPESCOMPATIBLE,
        BUILTINUNREACHABLE,
        BUILTINVAARG,
        BUILTINVACOPY,
        BUILTINVAEND,
        BUILTINVASTART
};

struct decl {
        char *name;
        enum declkind kind;
        struct type *type;

        union {
                enum builtinkind builtin;
        } u;
        
};

struct scope {
        struct map tags;
        struct map decls;
};

struct location {
	const char *file;
	size_t line, col;
};

struct token {
        enum tokenkind kind;
        struct location loc;
};

struct token_arr {
        struct token *tokens;
        size_t size;
        size_t capacity;
};

extern struct token tok;

void token_arr_init(struct token_arr)

void scanfrom(const char *, FILE *);

void targinit(const char *name);

void scopeinit();
void scopeputdecl(struct scope *scope, struct decl *decl);

void openscan();
void scanline();

extern struct type typevoid;
extern struct type typechar;
extern struct type typeint;
extern struct type typeuint;
extern struct type *typeadjvalist;

struct target {
        const char *name;
        struct type *typevalist;
        struct type *typewchar;
        int signedchar;
};

extern const struct target *targ;

struct type *typeadjust(struct type *type, enum typequal *qual);
