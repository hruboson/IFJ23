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
    //"$"+statement.func.id.string+"%"+var.id.string

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
        
        case ET_ADD:
            set_type(stack, exp->ops[0]);
            set_type(stack, exp->ops[1]);
            VarType d0_add = exp->ops[0]->data_type.type;
            VarType d1_add = exp->ops[1]->data_type.type;
            if(d0_add == d1_add){
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
            if((d0 == VARTYPE_INT || VARTYPE_DOUBLE) && d0 == d1)
            {
                exp->data_type = exp->ops[0]->data_type;  
                return 0;              
            }
            return 7;

        case ET_ID: {
            Variable *id_var = var_table_stack_get_var(stack, exp->id);
            exp->data_type.type = id_var->type.type;
            exp->data_type.nil_allowed = false;
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
            exp->data_type.type = VARTYPE_NIL;
            exp->data_type.nil_allowed = true;
            break;

        default:
            ERROR;
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

// If (expression) { ... } else { ... }
int semantic_if(VarTableStack *stack, FuncTable *table, Statement *statement) {
    if(!statement->if_.exp){
        SEMANTIC_ERROR_UNDEFINED_VARIABLE;
    }

    //TODO: je špatně if(x){}else{}?
    
    int ret = set_type(stack, statement->if_.exp);
    if(ret != 0){
        return ret;
    }
    
    Variable *get_expression_var1 = var_table_stack_get_var(stack, statement->if_.exp->ops[0]->id);
    if (!get_expression_var1) {
        SEMANTIC_ERROR_UNDEFINED_VARIABLE;
    }
    Variable *get_expression_var2 = var_table_stack_get_var(stack, statement->if_.exp->ops[1]->id);
    if (!get_expression_var2) {
        SEMANTIC_ERROR_UNDEFINED_VARIABLE;
    }

    //TODO: implementovat sračky expressionu

    //if(10 == "10"){} else {}
    //if(10 < 10)
    //if(10 <= 10)
    //if(10 > 10)
    //if(10 >= 10)
    if(get_expression_var1->type.type == get_expression_var2->type.type){
        return 0;
    }

    SEMANTIC_ERROR_TYPE_MISMATCH;
    //TODO: doplnit kontrolu, při if else?

}

// While (expression) { ... }
int semantic_while(VarTableStack *stack, FuncTable *table, Statement *statement) {

    if(!statement->while_.exp){
        SEMANTIC_ERROR_UNDEFINED_VARIABLE;
    }

    int ret = set_type(stack, statement->while_.exp);
    if(ret != 0){
        SEMANTIC_ERROR_UNDEFINED_VARIABLE;
    }

    Variable *get_while_condition_var = var_table_stack_get_var(stack, statement->assign.id);
    if (!get_while_condition_var) {
        SEMANTIC_ERROR_UNDEFINED_VARIABLE;
    }
    //TODO: kontrolovat všechny věci ohledně expressionu
    
    SEMANTIC_ERROR_TYPE_MISMATCH;
}

// Return expression;
int semantic_return(VarTableStack *stack, FuncTable *table, Statement *statement) {
    if(!statement->return_.exp){
        SEMANTIC_ERROR_UNDEFINED_VARIABLE;
    }

    int ret = set_type(stack, statement->return_.exp);
    if(ret != 0){
        return ret;
    }

    //TODO: kontrola návratového typu, něco takového?
    // Získání deklarovaného návratového typu funkce
    // VarType expected_return_type = get_return_type_of_current_function(table); // Doplnit podle implementace

    // Kontrola, zda typ návratové hodnoty odpovídá očekávanému typu
    // if (statement->return_exp->data_type.type != expected_return_type) {
           // Chyba: Nesouhlasí datové typy
    //     SEMANTIC_ERROR_TYPE_MISMATCH;
    // }

    return 0;
}

// Expression;
int semantic_expression(VarTableStack *stack, FuncTable *table, Statement *statement) {
    if(!statement){
        ERROR; 
    }

    //TODO: bude implementace takto?

    switch (statement->type) {
        case ST_ASSIGN:
            // Volání sémantické analýzy pro přiřazení
            return semantic_assignment(stack, table, statement);

        case ST_IF:
            // Volání sémantické analýzy pro podmíněné větvení
            return semantic_if(stack, table, statement);
        
        case ST_VAR:
            return semantic_variable(stack, table, statement);
        
        case ST_EXP:
            return semantic_expression(stack, table, statement);
        
        case ST_WHILE:
            // Volání sémantické analýzy pro cyklus while
            return semantic_while(stack, table, statement);
        
        case ST_RETURN:
            // Volání sémantické analýzy pro návratový výraz
            return semantic_return(stack, table, statement);
        case ST_FUNC:
            return semantic_function(stack, table, statement);

        default:
            // Chyba: Neznámý typ výrazu
            ERROR;
    }
}

// Function declaration
int semantic_function(VarTableStack *stack, FuncTable *table, Statement *statement) {
    if(func_table_get(table, statement->func.id)){
        //TODO: funkce byla již deklarována
        ERROR;
    }
    // Vytvoření nové tabulky symbolů pro funkci
    VarTable func_var_table;
    init_var_table(&func_var_table);
    vartable_stack_push(stack, &func_var_table);

    // Kontrola parametrů a jejich přidání do nové tabulky symbolů
    for (size_t i = 0; i < statement->func.param_count; ++i) {
        Parameter param = statement->func.parameters[i];
        // TODO: Kontrola typu parametru a přidání do tabulky symbolů

        // Přidání parametru do tabulky symbolů
        Variable var = { .id = param.intern_id, .type = param.type, .initialized = false };
        var_table_insert(&func_var_table, var);
    }

    // Přidání funkce do tabulky funkcí
    Function func_info = {
        .id = statement->func.id,
        .param_count = statement->func.param_count,
        .parameters = statement->func.parameters,
        .return_type = statement->func.return_type
    };
    func_table_insert(table, func_info);

    // TODO: Sémantická analýza těla funkce (např. volání parse_statement_list)

    // Odstranění tabulky symbolů pro funkci ze zásobníku
    vartable_stack_pop(stack, &func_var_table);

    return 0;
}
