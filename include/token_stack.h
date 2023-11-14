#include "scanner.h"

typedef struct TokenStack {
    TokenType* token;
    int size;
    int cap;
} TokenStack;

int init_token_stack(TokenStack* stack);
void clear_token_stack(TokenStack* stack);

int token_stack_push(TokenStack* stack, TokenType token);
int token_stack_pop(TokenStack* stack, TokenType* token);
int token_stack_peek(TokenStack* stack, TokenType* token);

bool token_stack_is_empty(TokenStack* stack);
