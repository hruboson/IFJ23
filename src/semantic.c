#include "semantic.h"
#include "vartable_stack.h"
#include <assert.h>
#include <stdlib.h>

// Obecná sémantická chyba
#define ERROR do { \
    return 9; \
} while (0)

// Sémantické chyby odpovídající kódům specifikovaným v zadání
#define SEMANTIC_ERROR_UNDEFINED_VARIABLE do { \
    return 5; \
} while (0)

#define SEMANTIC_ERROR_COUNT_OR_TYPE_OF_PARAM_IS_WRONG do { \
    return 4; \
} while(0)

#define SEMANTIC_ERROR_WRONG_RETURN_TYPE_OF_FUNCTION do { \
    return 4; \
} while (0)

#define SEMANTIC_ERROR_WRONG_EXP_OF_FUNC do { \
    return 6; \
} while (0)

#define SEMANTIC_ERROR_UNDEFINED_FUNCTION do { \
    return 3; \
} while (0)

#define SEMANTIC_ERROR_TYPE_MISMATCH do { \
    return 7; \
} while (0)


int set_type(VarTableStack *stack, FuncTable *table, Expression *exp){
    DataType d0, d1;

    Function *func = NULL;
    switch(exp->type){
        case ET_ADD:
            set_type(stack, table, exp->ops[0]);
            set_type(stack, table, exp->ops[1]);
            DataType d0_add = exp->ops[0]->data_type;
            DataType d1_add = exp->ops[1]->data_type;
            if(d0_add.type == d1_add.type){
                exp->data_type = exp->ops[0]->data_type; 
                return 0;              
            }
            SEMANTIC_ERROR_TYPE_MISMATCH;

        case ET_SUB:
        case ET_MULT:
        case ET_DIV:
            set_type(stack, table, exp->ops[0]);
            set_type(stack, table, exp->ops[1]);
            DataType d0_div = exp->ops[0]->data_type;
            DataType d1_div = exp->ops[1]->data_type;
            if((d0_div.type == VARTYPE_INT || VARTYPE_DOUBLE) && d0_div.type == d1_div.type)
            {
                exp->data_type = exp->ops[0]->data_type;  
                return 0;              
            }
           SEMANTIC_ERROR_TYPE_MISMATCH;

        case ET_ID: {
            Variable *id_var = var_table_stack_get_var(stack, exp->id);
            if(id_var == NULL){
                SEMANTIC_ERROR_UNDEFINED_VARIABLE;
            }
            exp->data_type = id_var->type;
            exp->id = id_var->unique_id;

            break;

        }
            
        case ET_INT:
            exp->data_type.type = VARTYPE_INT;
            exp->data_type.nil_allowed = false;
            break;
            
        case ET_STRING:
            exp->data_type.type = VARTYPE_STRING;
            exp->data_type.nil_allowed = false;
            break;

        case ET_DOUBLE:
            exp->data_type.type = VARTYPE_DOUBLE;
            exp->data_type.nil_allowed = false;
            break;

        case ET_NIL:
            exp->data_type.type = VARTYPE_VOID;
            exp->data_type.nil_allowed = true;
            break;
    
        case ET_EQUAL:
        case ET_N_EQUAL:
            exp->data_type.type = VARTYPE_BOOL;
            set_type(stack, table, exp->ops[0]);
            set_type(stack, table, exp->ops[1]);
            d0 = exp->ops[0]->data_type;
            d1 = exp->ops[1]->data_type;
            if( d0.nil_allowed == false && d1.nil_allowed == false &&
                d0.type == d1.type 
            ) {
                if((d0.type == VARTYPE_INT || d0.type == VARTYPE_DOUBLE ||
                 d0.type == VARTYPE_STRING) && d0.type == d1.type)
                {
                    return 0;   
                }
            }
            SEMANTIC_ERROR_TYPE_MISMATCH;

        case ET_LT:
        case ET_GT:
        case ET_LTE:
        case ET_GTE:
            exp->data_type.type = VARTYPE_BOOL;
            set_type(stack, table, exp->ops[0]);
            set_type(stack, table, exp->ops[1]);
            d0 = exp->ops[0]->data_type;
            d1 = exp->ops[1]->data_type;
            if( d0.nil_allowed == false && d1.nil_allowed == false &&
                d0.type == d1.type 
            ) {
                if((d0.type == VARTYPE_INT || d0.type == VARTYPE_DOUBLE ||
                 d0.type == VARTYPE_STRING) && d0.type == d1.type)
                {
                    return 0;   
                }
            }
            SEMANTIC_ERROR_TYPE_MISMATCH;

        case ET_FUNC:
            func = func_table_get(table, exp->fn_call.id);
            if(func == NULL){
                Function new_func;
                new_func.id = exp->fn_call.id;
                new_func.is_defined = false;
                new_func.param_count = exp->fn_call.arg_count;
                new_func.parameters = malloc(sizeof(Parameter) * (exp->fn_call.arg_count));
                if(new_func.parameters == NULL){
                    exit(99);
                }

                for (size_t i = 0; i < exp->fn_call.arg_count; ++i) {
                    Parameter *param_in = new_func.parameters + i;
                    const Argument *param_out = exp->fn_call.args + i;

                    param_in->extern_id = param_out->id;
                    param_in->intern_id = NULL;
                    param_in->type.type = param_out->exp->data_type.type;
                    param_in->type.nil_allowed = param_out->exp->data_type.nil_allowed;
                }

                new_func.return_type.type = VARTYPE_VOID;
                func_table_insert(table, new_func);
            }
            else{
                if(func->param_count == exp->fn_call.arg_count){
                    for(size_t i = 0; i < func->param_count; i++){
                        if(func->parameters[i].extern_id != exp->fn_call.args[i].id){
                            ERROR;
                        }
                        
                        if(func->parameters[i].type.type != exp->fn_call.args[i].exp->data_type.type){
                            if(func->parameters[i].type.type != VARTYPE_VOID){
                                ERROR;
                            }
                            func->parameters[i].type.type = exp->fn_call.args[i].exp->data_type.type;
                        }
                        
                        if (func->parameters[i].type.nil_allowed == false) {
                            if (exp->fn_call.args[i].exp->data_type.nil_allowed) {
                                func->parameters[i].type.nil_allowed = true;
                            } else {
                                ERROR;
                            }
                        }
                        else{
                            if(func->parameters[i].type.nil_allowed == true){
                                ERROR;
                            }
                        }
                    }
                }
                else{
                    SEMANTIC_ERROR_COUNT_OR_TYPE_OF_PARAM_IS_WRONG;
                }
            }
            return 0;

        default:
            printf("default error\n");
            ERROR;
    }
    return 0;
}


// Helper function
void insert_to_var_table(Statement *statement, VarTable *var_table){
    // pridani do vartable
    //ternary op je tam kvuli tomu kdyby se allow_nil rovnalo NULL aby se do .nil_allowed nepriradilo null ale false
    DataType dt = { .type = statement->var.data_type.type, .nil_allowed = statement->var.data_type.nil_allowed == true ? true : false}; 
    Variable var = { .id = statement->var.id, .type = dt, .initialized = !(statement->var.exp == NULL) };
    var_table_insert(var_table, var);
}

// Var x = 5

int semantic_type_match(DataType* L, DataType* R) {
    if (L->type == VARTYPE_VOID) {
        if (R->type == VARTYPE_VOID) {
            SEMANTIC_ERROR_TYPE_MISMATCH;     
        }
        *L = *R;
        return 0;
    } else {
        if (L->nil_allowed == false) {
            if (L->type != R->type || R->nil_allowed) {
                SEMANTIC_ERROR_TYPE_MISMATCH;
            }
            return 0;
        } else {
            if (L->type != R->type) {
                if (R->type == VARTYPE_VOID && R->nil_allowed) {
                    return 0;
                } else {
                    SEMANTIC_ERROR_TYPE_MISMATCH;
                }
            } else {
                if (R->nil_allowed) {
                    SEMANTIC_ERROR_TYPE_MISMATCH;
                }
            } 
            return 0;
        }
    }
    return 0;
}

int semantic_variable(VarTableStack *stack, Statement *statement, SymbolTable *symtab, FuncTable* func_table) {
    // unique_id = "$" + statement->var.id_prefix.func_id->symbol.data + "%" + statement->var.id_prefix.block_counter + "%" + statement->var.id->symbol.data
    
    if(symtab == NULL){
        ERROR;
    }

    //untested
    String string;
    init_string(&string);
    string_append(&string, "$");
    if(statement->var.id_prefix.func_id != NULL){
        string_append(&string, statement->var.id_prefix.func_id->symbol.data);
    }
    string_append(&string, "%");

    char f[64];
    snprintf(f, 64, "%lu", statement->var.id_prefix.block_counter);
    string_append(&string, f);
    string_append(&string, "%");
    string_append(&string, statement->var.id->symbol.data);

    symboltable_insert(symtab ,&string, &statement->var.unique_id);
    
    int ret = set_type(stack, func_table, statement->var.exp);
    if (ret != 0) return ret;

    DataType *L = &statement->var.data_type;
    DataType *R = &statement->var.exp->data_type;

    ret = semantic_type_match(L, R);
    if ( ret != 0){
        return ret;
    }

    VarTable *var_table;
    
    if (!stack) {
        ERROR;
    }

    vartable_stack_peek(stack, &var_table);
    
    if(var_table == NULL){
        ERROR;
    }
    Variable var;
    
    var.id = statement->var.id;
    var.unique_id = statement->var.unique_id;
    var.type = statement->var.data_type;
    var.initialized = statement->var.exp != NULL;

    var_table_insert(var_table, var);
    return 0;
}

// Assign x = y
int semantic_assignment(VarTableStack *stack, FuncTable *table, Statement *statement) {
    assert(statement->type == ST_ASSIGN);

    Variable *get_var = var_table_stack_get_var(stack, statement->assign.id);
    if (!get_var) {
        SEMANTIC_ERROR_UNDEFINED_VARIABLE;
    }

    int ret = set_type(stack, table, statement->assign.exp);
    if(ret != 0){
        return ret;
    }

    if(get_var->type.type == statement->assign.exp->data_type.type){
        if(statement->assign.exp->data_type.nil_allowed){
            if(get_var->type.nil_allowed == false){
                SEMANTIC_ERROR_TYPE_MISMATCH;
            }
        }
        return 0;
    }
    SEMANTIC_ERROR_TYPE_MISMATCH;    
}

// If (expression) { ... } else { ... }
int semantic_if(VarTableStack *stack, FuncTable *table, Statement *statement) {
    if(!statement->if_.exp){
        SEMANTIC_ERROR_UNDEFINED_VARIABLE;
    }
    
    int ret = set_type(stack, table, statement->if_.exp);
    if(ret != 0){
        return ret;
    }
    
    if(statement->if_.exp->data_type.type == VARTYPE_BOOL){
        SEMANTIC_ERROR_TYPE_MISMATCH;
    }

    return 0;
}

// While (expression) { ... }
int semantic_while(VarTableStack *stack, FuncTable *table, Statement *statement) {

    if(!statement->while_.exp){
        SEMANTIC_ERROR_UNDEFINED_VARIABLE;
    }

    int ret = set_type(stack, table, statement->while_.exp);
    if(ret != 0){
        return ret;
    }
    
    if(statement->while_.exp->data_type.type == VARTYPE_BOOL){
        SEMANTIC_ERROR_TYPE_MISMATCH;
    }

    return 0;
}

// Return expression;
int semantic_return(VarTableStack *stack, FuncTable *table, Statement *statement, SymbolRecord *funcId) {
    Function *func_table = func_table_get(table, funcId);
    if(func_table == NULL){
        exit(99);
    }
    
    if(!statement->return_.exp){
        if(func_table->return_type.type == VARTYPE_VOID){
            return 0;
        }
        SEMANTIC_ERROR_WRONG_RETURN_TYPE_OF_FUNCTION;
    }

    int ret = set_type(stack, table, statement->return_.exp);
    if(ret != 0){
        return ret;
    }

    if(statement->return_.exp->data_type.nil_allowed){
        if(func_table->return_type.nil_allowed == false){
            SEMANTIC_ERROR_WRONG_RETURN_TYPE_OF_FUNCTION;
        }
    }

    if(statement->return_.exp->data_type.type != func_table->return_type.type){
        SEMANTIC_ERROR_TYPE_MISMATCH;
    }

    return 0;
}

// Expression;
int semantic_expression(VarTableStack *stack, FuncTable *table, Statement *statement) {
    int ret = set_type(stack, table, statement->exp.exp);

    return ret;  
}

// Function declaration
int semantic_function(VarTableStack *stack, FuncTable *table, Statement *statement) {
    Function *func = func_table_get(table, statement->func.id);
    
    if(func == NULL){
        Function new_function;
        new_function.id = statement->func.id;
        new_function.is_defined = true;
        new_function.param_count = statement->func.param_count;
        new_function.parameters = malloc(sizeof(Parameter) * (statement->func.param_count));
        if(new_function.parameters == NULL){
            exit(99);
        }

        for (size_t i = 0; i < statement->func.param_count; ++i) {
            Parameter *param_in = new_function.parameters + i;
            const Parameter *param_out = statement->func.parameters + i;

            param_in->extern_id = param_out->extern_id;
            param_in->intern_id = param_out->intern_id;
            param_in->type = param_out->type;
        }

        new_function.return_type.type = statement->func.return_type.type;
        func_table_insert(table, new_function);
    }
    else{
        if(func->is_defined == true){
            ERROR;
        }
        else{
            func->is_defined = true;

            //check_params1
            if(func->param_count == statement->func.param_count){
                for(size_t i = 0; i < func->param_count; i++){
                    if(func->parameters[i].extern_id != statement->func.parameters[i].extern_id){
                        ERROR;
                    }
                    
                    if(func->parameters[i].type.type != statement->func.parameters[i].type.type){
                        if(func->parameters[i].type.type != VARTYPE_VOID){
                            ERROR;
                        }
                        func->parameters[i].type.type = statement->func.parameters[i].type.type;
                    }
                    
                    if(statement->func.parameters[i].type.nil_allowed){
                        func->parameters[i].type.nil_allowed = true;
                    }
                    else{
                        if(func->parameters[i].type.nil_allowed == true){
                            ERROR;
                        }
                    }
                }
            }
            else{
                SEMANTIC_ERROR_COUNT_OR_TYPE_OF_PARAM_IS_WRONG;
            }
        }
    }

    VarTable *var_table;
    vartable_stack_peek(stack, &var_table);
    for(size_t i = 0; i < statement->func.param_count; i++){
        DataType dt = {.type = statement->func.parameters[i].type.type, .nil_allowed = statement->func.parameters[i].type.nil_allowed };
        Variable var = {.id = statement->func.parameters[i].intern_id, .initialized = true, .type = dt};
        var_table_insert(var_table, var);
    }
    return 0;
}
