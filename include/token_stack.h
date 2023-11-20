#pragma once

#include "scanner.h"

#define STACK_SIZE 64

typedef struct TokenStack {
    TokenType** tokens;
    int size;
    int cap;
} TokenStack;

void init_token_stack(TokenStack* stack);
void clear_token_stack(TokenStack* stack);

void token_stack_push(TokenStack* stack, TokenType* token);
void token_stack_pop(TokenStack* stack, TokenType* token);
void token_stack_peek(TokenStack* stack, TokenType* token);

bool token_stack_is_empty(TokenStack* stack);
