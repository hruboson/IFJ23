/**
 * Implementace překladače imperativního jazyka IFJ23
 * xsatin03 Ondřej Šatinský
*/

#pragma once

#include "scanner.h"

#define STACK_SIZE 64

typedef struct TokenStack {
    Token** tokens;
    int size;
    int cap;
} TokenStack;

void init_token_stack(TokenStack* stack);
void clear_token_stack(TokenStack* stack);

void token_stack_push(TokenStack* stack, Token* token);
void token_stack_pop(TokenStack* stack, Token** token);
void token_stack_peek(TokenStack* stack, Token** token);

bool token_stack_is_empty(TokenStack* stack);
