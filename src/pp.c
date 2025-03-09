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

static struct frame* ctxpush(struct token *token, size_t num, struct macro *macro, bool space) {
        struct frame *frame;

        frame = arrayadd(&ctx, sizeof(*frame));
        frame->token = token;
        frame->num_tokens = num;
        frame->macro = macro;
        if (num > 0)
                token[0].space = space;
        return frame;
}

/* Get the next token from context */
static struct token *ctxnext() {
        struct frame *frame;
        struct token *token;
        struct macro *macro;
        bool space;
        size_t i;

        for(;;) {
                for (frame = arraylast(&ctx, sizeof(*frame)); ctx.len; --f, ctx.len -= sizeof(*frame)) {
                        if (frame->num_tokens)
                                break;
                        if (frame->macro)
                                macrodone(frame->macro);
                }
                if(ctx.len == 0)
                        return NULL;
                macro = frame->macro;
                if (macro && macro->kind == MACROFUNC) {
                        space = frame->token->space;
                        switch(frame->token->kind) {
                                case THASH:
                                        framenext(frame);
                                        token = framenext(frame);
                                        assert(token);
                                        i = macroparam(macro, token);
                                        assert(i != -1);
                                        frame = ctxpush(&macro->arg[i].str, 1, NULL, space);
                                        break;
                                case TIDENT:
                                        i = macroparam(macro, frame->token);
                                        if (i == -1)
                                                break;
                                        framenext(frame);
                                        if (macro->arg[i].ntoken == 0)
                                                continue;
                                        frame = ctxpush(macro->arg[i].token, macro->arg[i].ntoken, NULL, space);
                                        break;

                        }
                }
                return framenext(frame);
        }
}

static struct token *rawnext() {
        struct token *token;

        token = ctxnext();
}

void next() {
        struct token *token;

        do token = rawnext();
}

bool consume(int kind) {
        if (tok.kind != kind)
                return false;
        next();
        return true;
}