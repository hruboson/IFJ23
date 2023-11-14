#include "parser.h"
#include "token_stack.h"

int parse(FILE* f, AST* ast){
    TokenStack stack;
    init_token_stack(&stack);
    token_stack_push(&stack, TOKENTYPE_EOF);

    int ret = 0;

    Statement* st;
    Statement** next_st = &ast->statement;

    while ( ret == 0 ) {

        ret = parse_statement(f, &stack, &st );
        if ( ret )
            break;

        *next_st = st;
        next_st = &st->next; 
    }

    clear_token_stack(&stack);

    if ( ret == -1 )
        ret = 0;
    else
        ret = 99;

    return ret;
}
