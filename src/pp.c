#include <assert.h>
#include <stdbool.h>
#include <stdlib.h>
#include "cc.h"

struct macroparam {
        char *name;
        enum {
                PARAMTOK = 1<<0,
                PARAMSTR = 1<<1,
                PARAMVAR = 1<<2,
        };
};

struct macroarg {
        struct token *token;
        size_t ntoken;
        struct token str;
};

struct macro {
	enum {
		MACROOBJ,
		MACROFUNC,
	} kind;
	char *name;
	bool hide;
	struct macroparam *param;
	size_t nparam;
	struct macroarg *arg;
	struct token *token;
	size_t ntoken;
};

struct frame {
        struct token *token;
        size_t num_tokens;
        struct macro *macro;
};

static struct array ctx;

static size_t macrodepth;

static size_t macroparam(struct macro *macro, struct token *token) {
        if(token->kind == TIDENT) {
                for(size_t i = 0; i < macro->nparam; ++i) {
                        if(strcmp(macro->param[i].name, token->lit) == 0)
                                return i;
                }
        }
        return -1;
}

static void macrodone(struct macro *macro) {
        macro->hide = false;
        if (macro->kind == MACROFUNC && macro->nparam > 0) {
                free(macro->arg[0].token);
                free(macro->arg);
        }
        --macrodepth;
}

static struct token *framenext(struct frame *frame) {
        return frame->num_tokens--, frame->token++;
}
extern struct token tok;
