#include "AST.h"

void init_ast( AST* ast ) { /* TODO */ }
void clear_ast( AST* ast ) { /* TODO */ }

void print_ast( AST* ast ){
    Statement *stm = ast->statement;
    while(stm){
        
        stm = ast->statement->next;
    }
}
