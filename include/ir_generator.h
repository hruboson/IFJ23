#pragma once

#include "AST.h"

typedef struct IR {
    // TODO
} IR;

void init_ir( IR* );
void clear_ir( IR* );

int convert_to_ir( const AST*, IR* );
