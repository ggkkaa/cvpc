#include <stdlib.h>
#include "3rdparty/arena.h"
#include "cc.h"

struct token tok;

void token_arr_init(struct token_arr *array) {
        array->tokens = (struct token *)aalloc(INITIAL_SIZE * sizeof(struct token));

        if(array->tokens = NULL) {
                perror("token_init: Out of memory!");
                exit(EXIT_FAILURE);
        }

        memset(array->tokens, 0, INITIAL_SIZE * sizeof(struct token));
        array->capacity = INITIAL_SIZE;
        array->size = 0;
}

void token_arr_add(struct token_arr *array, struct token token) {
        if (array->size >= array->capacity) {
                array->capacity *= 2;
                array->tokens = arena_realloc(&arena, array->tokens, array->size, array->capacity * sizeof(token));

                if(array->tokens = NULL) {
                        perror("token_add: Out of memory!");
                        exit(EXIT_FAILURE);
                }
        }

        array->tokens[array->size] = token;
        array->size++;
}