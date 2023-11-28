#include "table.h"
#include "stdlib.h"

void init_func_table(FuncTable* table) {
    table->funcs_cap = 0;
    table->funcs_size = 0;
    table->funcs = NULL;
}

void clear_func_table(FuncTable* table) {
    free(table->funcs);
    table->funcs_cap = 0;
    table->funcs_size = 0;
    table->funcs = NULL;
}

void func_table_insert(FuncTable* table, Function func) {
    if (table->funcs == NULL) {
        table->funcs = malloc(sizeof(Function) * FUNC_TABLE_INIT_SIZE);
        table->funcs_cap = FUNC_TABLE_INIT_SIZE;
    }

    if (table->funcs_size >= (table->funcs_cap / 2)) {
        table->funcs = realloc(table->funcs, sizeof(Function) * (table->funcs_cap * 2));
        table->funcs_cap = table->funcs_cap * 2;
    }

    table->funcs[table->funcs_size] = func;
    table->funcs_size++;
}

Function* func_table_get(FuncTable* table, SymbolRecord* id) {
    for (size_t i = 0; i < table->funcs_size; i++) {
        Function* list = &(table->funcs[i]);
        if (list) {
            if (list->id->hash == id->hash) {
                return list;
            }
        }
        i++;
    }
}

void init_var_table(VarTable* table) {
    table->vars_cap = 0;
    table->vars_size = 0;
    table->vars = NULL;
}
void clear_var_table(VarTable* table) {
    free(table->vars);
    table->vars_cap = 0;
    table->vars_size = 0;
    table->vars = NULL;
}
void var_table_insert(VarTable* table, Variable var) {
}
Variable* var_table_get(VarTable* table, SymbolRecord* id) {
}
