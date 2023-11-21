#pragma once
#include "symtable.h"

typedef enum VarType {
    VARTYPE_VOID,  // for functions
    VARTYPE_STRING,
    VARTYPE_INT,
    VARTYPE_DOUBLE,
} VarType;

typedef struct DataType {
    VarType type;
    bool nil_allowed;
} DataType;

typedef struct Parameter {
    SymbolRecord* extern_id;
    SymbolRecord* intern_id;
    DataType type;
} Parameter;

typedef struct Variable {
    SymbolRecord* id;
    DataType type;
    bool initialized;
} Variable;

typedef struct Function {
    SymbolRecord* id;
    size_t param_count;
    Parameter* parameters;
    DataType return_type;
} Function;

typedef struct VarTable {
    // linear for now
    size_t vars_size;
    size_t vars_cap;
    Variable* vars;
} VarTable;

typedef struct FuncTable {  // todo rename to better suiting name
    // linear for now
    // global table
    size_t funcs_size;
    size_t funcs_cap;
    Function* funcs;
} FuncTable;

void init_func_table(FuncTable* table);
void clear_func_table(FuncTable* table);
void func_table_insert(FuncTable* table, Function func);
Function* func_table_get(FuncTable* table, SymbolRecord* id);

void init_var_table(VarTable* table);
void clear_var_table(VarTable* table);
void var_table_insert(VarTable* table, Variable var);
Variable* var_table_get(VarTable* table, SymbolRecord* id);
