#include "table.h"

#include <stdlib.h>
#include <assert.h>

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
    Function* f;
    for (size_t i = 0; i < table->funcs_size; i++) {
        f = &(table->funcs[i]);
        if (f) {
            if (f->id->hash == id->hash) {
                return f;
            }
        }
        i++;
    }

    return NULL;
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
    if (table->vars == NULL) {
        table->vars = malloc(sizeof(Variable) * VAR_TABLE_INIT_SIZE);
        table->vars_cap = VAR_TABLE_INIT_SIZE;
    }

    if (table->vars_size >= (table->vars_cap / 2)) {
        table->vars = realloc(table->vars, sizeof(Variable) * (table->vars_cap * 2));
        table->vars_cap = table->vars_cap * 2;
    }

    table->vars[table->vars_size] = var;
    table->vars_size++;
}

Variable* var_table_get(VarTable* table, SymbolRecord* id) {
	if (!table) return NULL;
	for (size_t i = 0; i < table->vars_size; i++) {
		Variable* v = table->vars + i;
		assert( v->id );
		if (
			v->id->hash == id->hash &&
			string_eq( &id->symbol, &v->id->symbol )
		) {
			return v;
		}
	}

	return NULL;  // variable not found in table
}
