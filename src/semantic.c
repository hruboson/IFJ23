#include "semantic.h"
#include "vartable_stack.h"

#include <stdlib.h>

// obecná sémantická chyba
#define ERROR(msg) do { \
    fprintf(stderr, "Chyba: %s (%s:%d): %s\n", __func__, __FILE__, __LINE__, msg); \
    exit(9);  \
} while (0)

#define SEMANTIC_ERROR_UNDEFINED_VARIABLE(id) do { \
    fprintf(stderr, "Chyba: %s (%s:%d): Nedefinovaná proměnná '%s'\n", __func__, __FILE__, __LINE__, id); \
    exit(5); \
} while (0)

#define SEMANTIC_ERROR_TYPE_MISMATCH do { \
    fprintf(stderr, "Chyba: %s (%s:%d): Nesoulad typů mezi proměnnou a výrazem\n", __func__, __FILE__, __LINE__); \
    exit(7); \
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
bool semantic_variable(VarTableStack *stack, FuncTable *table, Statement *statement) {
    VarTable *var_table;
    vartable_stack_peek(stack, var_table);

    if (!stack) {
        ERROR("Neplatný ukazatel na zásobník proměnných");
    }

    Variable *get_var = var_table_stack_get_var(stack, statement->var.exp->id);
    if (!get_var) {
        SEMANTIC_ERROR_UNDEFINED_VARIABLE(statement->var.exp->id);
    }

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
            ERROR("Neznámý typ proměnné"); 
    }
    return true;
}

// Assign x = y
bool semantic_assignment(VarTableStack *stack, FuncTable *table, Statement *statement) {
    Variable *assigned_var = var_table_stack_get_var(stack, statement->assign.id);
    if (!assigned_var) {
        fprintf(stderr, "Chyba: %s (%s:%d): Nedefinovaná proměnná '%s'\n", __func__, __FILE__, __LINE__, statement->assign.id);
        exit(5);
    }

    //TODO: doplnit případné další sémantické kontroly
    //TODO: doplnit, co se stane při přiřazení
    return true;
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
