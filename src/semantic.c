#include "semantic.h"

#define ERROR(msg) do { \
    fprintf(stderr, "Chyba: %s (%s:%d): %s\n", __func__, __FILE__, __LINE__, msg); \
    exit(9); \
} while (0)

#define SEMANTIC_ERROR_UNDEFINED_VARIABLE(id) do { \
    fprintf(stderr, "Chyba: %s (%s:%d): Nedefinovaná proměnná '%s'\n", __func__, __FILE__, __LINE__, id); \
    exit(5); \
} while (0)

#define SEMANTIC_ERROR_TYPE_MISMATCH do { \
    fprintf(stderr, "Chyba: %s (%s:%d): Nesoulad typů mezi proměnnou a výrazem\n", __func__, __FILE__, __LINE__); \
    exit(7); \
} while (0)

// Var x = 5
bool semantic_variable(VarTableStack *stack, FuncTable *table, Statement *statement) {
    if (!stack) {
        ERROR("Neplatný ukazatel na zásobník proměnných");
    }

    Variable *get_var = var_table_stack_get_var(stack, statement->var.exp->id);
    if (!get_var) {
        SEMANTIC_ERROR_UNDEFINED_VARIABLE(statement->var.exp->id);
    }

    switch (get_var->type.type) {
        case VARTYPE_INT:
            if (statement->type != ET_INT) {
                SEMANTIC_ERROR_TYPE_MISMATCH;
            }
            break;
        case VARTYPE_DOUBLE:
            if (statement->type != ET_DOUBLE) {
                SEMANTIC_ERROR_TYPE_MISMATCH;
            }
            break;
        case VARTYPE_STRING:
            if (statement->type != ET_STRING) {
                SEMANTIC_ERROR_TYPE_MISMATCH;
            }
            break;
        case VARTYPE_VOID:
            if (statement->type != ET_STRING && statement->type != ET_INT && statement->type != ET_DOUBLE) {
                SEMANTIC_ERROR_TYPE_MISMATCH;
            }
            break;
        default:
            ERROR("Neznámý typ proměnné");
    }

    get_var->initialized = true;
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