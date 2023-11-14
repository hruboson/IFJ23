#pragma once

#include "statement.h"

typedef struct AST {
    Statement* statement;
} AST;

void init_ast( AST* );
void clear_ast( AST* );
