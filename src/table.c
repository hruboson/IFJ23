#include "table.h"

void init_func_table(FuncTable* table);
void clear_func_table(FuncTable* table);
void func_table_insert(FuncTable* table, Function func);
Function* func_table_get(FuncTable* table, SymbolRecord* id);

void init_var_table(VarTable* table);
void clear_var_table(VarTable* table);
void var_table_insert(VarTable* table, Variable var);
Variable* var_table_get(VarTable* table, SymbolRecord* id);
