#pragma once

#include "AST.h"
#include <stdio.h>
#include "vartable_stack.h"

// int semantic( Input*, AST* );

int semantic_variable(VarTableStack* , Statement* , SymbolTable*, FuncTable* );
int semantic_assignment(VarTableStack* , FuncTable*, Statement* );
int semantic_if(VarTableStack*, FuncTable*, Statement* );
int semantic_while(VarTableStack*, FuncTable*, Statement* );
int semantic_return(VarTableStack*, FuncTable*, Statement*, SymbolRecord *funcId);
int semantic_expression(VarTableStack*, FuncTable*, Statement*);
int semantic_function(VarTableStack*, FuncTable*, Statement* );
int semantic_type_match(DataType* L, DataType* R);
