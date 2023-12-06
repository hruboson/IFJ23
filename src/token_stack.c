/**
 * Implementace překladače imperativního jazyka IFJ23
 * xhrubo01 Ondřej Hruboš
*/

#include "token_stack.h"

#include "stdlib.h"

void init_token_stack(TokenStack* stack) {
    if (stack == NULL) {
        exit(99);
    } else {
        stack->size = -1;
		stack->cap = STACK_SIZE;
        stack->tokens = malloc(sizeof(Token*) * STACK_SIZE);
        if (stack->tokens == NULL) {
            exit(99);
        }
    }
}

void clear_token_stack(TokenStack* stack) {
    if (stack->tokens != NULL) {
        free(stack->tokens);
        stack->tokens = NULL;
        stack->size = 0;
        stack->cap = 0;
        stack = NULL;  //! only clear or delete ?
    }
}

void token_stack_push(TokenStack* stack, Token* token) {
    if (stack->size >= stack->cap - 1) {
        stack->cap *= 2;
        stack->tokens = realloc(stack->tokens, sizeof(Token*) * stack->cap);
        if (!stack->tokens) exit(99);
    }

    if (stack->size == -1) {
        stack->size = 1;
    } else {
        stack->size++;
    }

    stack->tokens[stack->size - 1] = token;
}

void token_stack_pop(TokenStack* stack, Token** token) {
    if (!token_stack_is_empty(stack)) {
        *token = stack->tokens[stack->size - 1];
        stack->size--;
    }
}

void token_stack_peek(TokenStack* stack, Token** token) {
    if (!token_stack_is_empty(stack)) {
        *token = stack->tokens[stack->size - 1];
    }
}

bool token_stack_is_empty(TokenStack* stack) {
    if (!stack) exit(99);
    return stack->size == 0;
}