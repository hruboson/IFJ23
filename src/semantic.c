#include "semantic.h"
#include "vartable_stack.h"

#include <stdlib.h>

// obecná sémantická chyba
#define ERROR do { \
    return 9;  \
} while (0)

#define SEMANTIC_ERROR_UNDEFINED_VARIABLE do { \
    return 5; \
} while (0)

#define SEMANTIC_ERROR_TYPE_MISMATCH do { \
    return 7; \
} while (0)

// Helper function
void insert_to_var_table(Statement *statement, VarTable *var_table){
    // pridani do vartable
    //ternary op je tam kvuli tomu kdyby se allow_nil rovnalo NULL aby se do .nil_allowed nepriradilo null ale false
    DataType dt = { .type = statement->var.data_type, .nil_allowed = statement->var.allow_nil == true ? true : false}; 
    Variable var = { .id = statement->var.id, .type = dt, .initialized = !(statement->var.exp == NULL) };
    var_table_insert(var_table, var);
}

// Var x = 5
int semantic_variable(VarTableStack *stack, FuncTable *table, Statement *statement) {
    // pokud jsi ve funkci, přejmenuj proměnnou na func_name%var_name
    // func_name = statement.func.id.string + "%" + var.id.string

    VarTable var_table_;
    VarTable *var_table = &var_table_;
    
    if (!stack) {
        ERROR;
    }

    vartable_stack_peek(stack, var_table);

    // Variable *get_var = var_table_stack_get_var(stack, statement->var.exp->id);
    // if (!get_var) {
    //     SEMANTIC_ERROR_UNDEFINED_VARIABLE(statement->var.exp->id);
    // }

    switch (statement->var.data_type) {
        case VARTYPE_INT:
            if (statement->var.exp->type != ET_INT) {
                SEMANTIC_ERROR_TYPE_MISMATCH;
            } else {
                insert_to_var_table(statement, var_table);
            }
            break;
        case VARTYPE_DOUBLE:
            if (statement->var.exp->type != ET_DOUBLE) {
                SEMANTIC_ERROR_TYPE_MISMATCH;
            } else {
                insert_to_var_table(statement, var_table);
            }
            break;
        case VARTYPE_STRING:
            if (statement->var.exp->type != ET_STRING) {
                SEMANTIC_ERROR_TYPE_MISMATCH;
            } else {
                insert_to_var_table(statement, var_table);
            }
            break;
        case VARTYPE_VOID:
            switch(statement->var.exp->type){
                case ET_STRING:
                    statement->var.data_type = VARTYPE_STRING;
                    break;
                case ET_INT:
                    statement->var.data_type = VARTYPE_INT;
                    break;
                case ET_DOUBLE:
                    statement->var.data_type = VARTYPE_DOUBLE;
                    break; 
                default:
                    SEMANTIC_ERROR_TYPE_MISMATCH;
            }
            insert_to_var_table(statement, var_table);
                
        default:
            ERROR; 
    }
    return 0;
}

int set_type(VarTableStack *stack, Expression *exp){
    switch(exp->type){
        case ET_ID:
            //Variable *id_var = var_table_stack_get_var(stack, exp->id);
            //TODO: dodělat implementaci pro stav ET_ID
        case ET_ADD:
            set_type(stack, exp->ops[0]);
            set_type(stack, exp->ops[1]);
            VarType d0 = exp->ops[0]->data_type.type;
            VarType d1 = exp->ops[1]->data_type.type;
            if(d0 == d1){
                exp->data_type = exp->ops[0]->data_type;  
                return 0;              
            }
            return 7;

        case ET_SUB:
        case ET_MULT:
        case ET_DIV:
            set_type(stack, exp->ops[0]);
            set_type(stack, exp->ops[1]);
            VarType d0 = exp->ops[0]->data_type.type;
            VarType d1 = exp->ops[1]->data_type.type;
            if((d0 == ET_INT || ET_DOUBLE) && d0 == d1)
            {
                exp->data_type = exp->ops[0]->data_type;  
                return 0;              
            }
            return 7;

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
            exp->data_type.type = VARTYPE_NIL;
            exp->data_type.nil_allowed = true;
            break;
    }
}

// Assign x = y
int semantic_assignment(VarTableStack *stack, FuncTable *table, Statement *statement) {
    assert(statement->type == ST_ASSIGN);

    Variable *get_var = var_table_stack_get_var(stack, statement->assign.id);
    if (!get_var) {
        SEMANTIC_ERROR_UNDEFINED_VARIABLE;
    }

    int ret = set_type(stack, statement->assign.exp);
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

// If (condition) { ... } else { ... }
bool semantic_if(VarTableStack *stack, FuncTable *table, Statement *statement) {
    //TODO: doplnit sémantickou analýzu pro if
    //TODO: doplnit sémantickou analýzu pro kontrolu podmínky
    //TODO: doplnit kontrolu, při if else

    return true;
}

// While (condition) { ... }
bool semantic_while(VarTableStack *stack, FuncTable *table, Statement *statement) {
    //TODO: doplnit sémantickou analýzu pro while
    //TODO: doplnit sémantickou analýzu pro kontrolu podmínky

    return true;
}

// Return expression;
bool semantic_return(VarTableStack *stack, FuncTable *table, Statement *statement) {
    //TODO: doplnit sémantickou analýzu pro return
    //TODO: kontrola správnosti výrazu návratové funkce a jeho typu

    return true;
}

// Expression;
bool semantic_expression(VarTableStack *stack, FuncTable *table, Statement *statement) {
    //TODO: bude sémantická analýza pro výraz? Popkud ano, doplnit

    return true;
}

// Function declaration
bool semantic_function(VarTableStack *stack, FuncTable *table, Statement *statement) {
    //TODO: doplnit sémantikcou analýzu pro správnost deklarace funkce
    //TODO: doplnit kontrolu, zda nebyla funkce již deklarována
    //TODO: kontrola parametrů a návratových typů

    return true;
}
