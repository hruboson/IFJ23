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

#define SEMANTIC_ERROR_TYPE_INFERENCE do { \
    return 8; \
} while (0)


/// @brief Metoda set_type nastaví expression.data_type podle získaného typu
/// @param stack 
/// @param table 
/// @param exp 
/// @return pokud prošel, vrací se 0, jinak číslo chyby
int set_type(VarTableStack *stack, FuncTable *func_table, Expression *exp){
    DataType d0, d1;

    Function *func = NULL;
    switch(exp->type){
        case ET_ADD:
            set_type(stack, func_table, exp->ops[0]);
            set_type(stack, func_table, exp->ops[1]);
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
            set_type(stack, func_table, exp->ops[0]);
            set_type(stack, func_table, exp->ops[1]);
            DataType d0_div = exp->ops[0]->data_type;
            DataType d1_div = exp->ops[1]->data_type;

            if((d0_div.type == VARTYPE_INT || d0_div.type == VARTYPE_DOUBLE) && d0_div.type == d1_div.type)
            {
                exp->data_type = exp->ops[0]->data_type;  
                return 0;              
            }
           SEMANTIC_ERROR_TYPE_MISMATCH;

        case ET_ID: {
            Variable *id_var = var_table_stack_get_var(stack, exp->id);
            if(id_var == NULL) {
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
            set_type(stack, func_table, exp->ops[0]);
            set_type(stack, func_table, exp->ops[1]);
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
            set_type(stack, func_table, exp->ops[0]);
            set_type(stack, func_table, exp->ops[1]);
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
            Function *func = func_table_get(func_table, exp->fn_call.id);

            int ret;

            if(func == NULL){
                Function func;
                func.id = exp->fn_call.id;
                func.is_defined = false;
                func.param_count = exp->fn_call.arg_count;

                // naplni argumenty, se kterymi byla zavolana
                func.parameters = malloc(sizeof(Parameter) * (exp->fn_call.arg_count));
                    if(func.parameters == NULL){
                        exit(99);
                    }

                    for (size_t i = 0; i < exp->fn_call.arg_count; ++i) {
                        //CHECK
                        Parameter *param_in = func.parameters + i;
                        const Argument *argument = exp->fn_call.args + i;

                        param_in->extern_id = argument->id;
                        param_in->type = argument->exp->data_type;
                    }

                    func_table_insert(func_table, func);
                
            } else { // uz je definovana
                // muzou se zkontrolovat argumenty
                if (func->is_write && exp->fn_call.arg_count < 1) {
                    printf("FAILED HERE: %d\n", __LINE__);
                    SEMANTIC_ERROR_COUNT_OR_TYPE_OF_PARAM_IS_WRONG;
                }

                for (size_t i = 0; i < func->param_count; ++i) {
                    ret = set_type(stack, func_table, exp);

                    if (ret != 0)
                        return ret;

                    if (func->is_write) continue; // na write se vola jenom set_var

                    //check_params1
                    if(func->param_count == exp->fn_call.arg_count){
                        for(size_t i = 0; i < func->param_count; i++){
                            if(func->parameters[i].extern_id != exp->fn_call.args[i].id){
                                ERROR;
                            }
                            
                            //CHECK
                            ret = semantic_type_match(&func->parameters[i].type, &exp->fn_call.args[i].exp->data_type);
                            if (ret != 0)
                                return ret;
                        }
                    }
                    else{
                        printf("FAILED HERE: %d\n", __LINE__);
                        SEMANTIC_ERROR_COUNT_OR_TYPE_OF_PARAM_IS_WRONG;
                    }
                }
                
            }
            return 0;

        case ET_NIL_TEST:
            set_type(stack, func_table, exp->ops[0]);
            set_type(stack, func_table, exp->ops[1]);
            DataType left = exp->ops[0]->data_type;
            DataType right = exp->ops[1]->data_type;

            if (right.type == VARTYPE_VOID || right.nil_allowed)
                SEMANTIC_ERROR_TYPE_MISMATCH;
            
            if (left.nil_allowed == false)
                SEMANTIC_ERROR_TYPE_MISMATCH;

            if ( left.type == right.type ||
                (left.type == VARTYPE_VOID && left.nil_allowed)
            ) {
                exp->data_type = exp->ops[1]->data_type; 
                return 0; 
            }

            SEMANTIC_ERROR_TYPE_MISMATCH;
            break;

        default:
            printf("default error\n");
            ERROR;
    }
    return 0;
}

/// @brief Porovnává 2 datové typy, jestli jsou sémanticky správně
/// @param L 
/// @param R 
/// @return Pokud jsou sémanticky správně, vrátí se 0, jinak odpovídající error
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

/// @brief Tato metoda ověřuje, zda je proměnná sémanticky korektní(Var x = 5)
/// @param stack 
/// @param statement 
/// @param symtab 
/// @param func_table 
/// @return Pokud je metoda korektní, vrací se 0, jinak odpovídající error
int semantic_variable(VarTableStack *stack, Statement *statement, SymbolTable *symtab, FuncTable* func_table) {
    // unique_id = "$" + statement->var.id_prefix.func_id->symbol.data + "%" + statement->var.id_prefix.block_counter + "%" + statement->var.id->symbol.data
    
    if(symtab == NULL){
        ERROR;
    }

    // Vytvoří se nový string, do kterého se přidává nové jméno proměnné, aby se dala
    // korektně vygenerovat proměnná a uloží se do var.unique_id
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
    var.is_const = !statement->var.modifiable;

    var_table_insert(var_table, var);
    return 0;
}

/// @brief Sémantické ověření uložení proměnné do proměnné (x = y)
/// @param stack 
/// @param statement 
/// @param func_table 
/// @return pokud je správně, vrátí se 0, jinak error
int semantic_assignment(VarTableStack *stack, Statement* statement, FuncTable *func_table) {
    assert(statement->type == ST_ASSIGN);

    Variable *get_var = var_table_stack_get_var(stack, statement->assign.id);
    if (!get_var) {
        SEMANTIC_ERROR_UNDEFINED_VARIABLE;
    }

    int ret = set_type(stack, func_table, statement->assign.exp);
    if(ret != 0){
        return ret;
    }

    DataType *L = &get_var->type;
    DataType *R = &statement->assign.exp->data_type;

    if (get_var->is_const) {
        ERROR;
    }

    ret = semantic_type_match(L, R);
    if ( ret != 0){
        return ret;
    }

    return 0;    
}

/// @brief Sémantická kontrola, zda je podmínka korektní (If (expression) { ... } else { ... })
/// @param stack 
/// @param func_table 
/// @param statement 
/// @return pokud je korektní, vrací se 0, jinak error
int semantic_if(VarTableStack *stack, FuncTable *func_table, Statement *statement){
    if(!statement->if_.exp){
        SEMANTIC_ERROR_UNDEFINED_VARIABLE;
    }
    
    if(statement->if_.check_nil){
        Variable *var = var_table_stack_get_var(stack, statement->if_.exp->id);
        if (!var) {
            SEMANTIC_ERROR_UNDEFINED_VARIABLE;
        }

        if(var->type.nil_allowed && var->is_const) {
            return 0;
        }

        SEMANTIC_ERROR_TYPE_INFERENCE; 
    }

    int ret = set_type(stack, func_table, statement->if_.exp);
    if(ret != 0){
        return ret;
    }
    
    if(statement->if_.exp->data_type.type != VARTYPE_BOOL){
        SEMANTIC_ERROR_TYPE_MISMATCH;
    }

    return 0;
}

/// @brief ověřuje, zda je podmínka sémanticky korektní (While (expression) { ... })
/// @param stack 
/// @param table 
/// @param statement 
/// @return pokud ano, vrací se 0, jinak error
int semantic_while(VarTableStack *stack, FuncTable *table, Statement *statement) {

    if(!statement->while_.exp){
        SEMANTIC_ERROR_TYPE_MISMATCH;
    }

    int ret = set_type(stack, table, statement->while_.exp);
    if(ret != 0){
        return ret;
    }
    
    if(statement->while_.exp->data_type.type != VARTYPE_BOOL){
        SEMANTIC_ERROR_TYPE_MISMATCH;
    }

    return 0;
}

/// @brief ověřuje se, zda návratový typ odpovídá funkci, ve které se nachází return
/// @param stack 
/// @param table 
/// @param statement 
/// @param funcId 
/// @return pokud ano, vrací se 0, jinak error
int semantic_return(VarTableStack *stack, FuncTable *table, Statement *statement, SymbolRecord *funcId) {

    if (funcId == NULL) {
        ERROR;
    }
    
    Function *function = func_table_get(table, funcId);
    if(function == NULL){
        exit(99);
    }
    
    if(statement->return_.exp == NULL){
        if(function->return_type.type == VARTYPE_VOID){
            return 0;
        }
        printf("FAILED HERE: %d\n", __LINE__);
        SEMANTIC_ERROR_WRONG_RETURN_TYPE_OF_FUNCTION;
    }

    int ret = set_type(stack, table, statement->return_.exp);
    if(ret != 0){
        return ret;
    }

    if(statement->return_.exp->data_type.type == VARTYPE_VOID &&
        statement->return_.exp->data_type.nil_allowed
    ){
        if(function->return_type.nil_allowed == false){
            printf("FAILED HERE: %d\n", __LINE__);
            SEMANTIC_ERROR_WRONG_RETURN_TYPE_OF_FUNCTION;
        }

        return 0;
    }

    if(statement->return_.exp->data_type.type != function->return_type.type){
        printf("FAILED HERE: %d\n", __LINE__);
        SEMANTIC_ERROR_WRONG_RETURN_TYPE_OF_FUNCTION;
    }

    return 0;
}

/// @brief sémantické ověření, zda expression je sémanticky správně. ověření pomocí set_type
/// @param stack 
/// @param table 
/// @param statement 
/// @return vrací se výstup set_type.
int semantic_expression(VarTableStack *stack, FuncTable *table, Statement *statement) {
    int ret = set_type(stack, table, statement->exp.exp);

    return ret;  
}

/// @brief sémantické ověření, jestli je implementace funkce korektní, jestli jsou korektní parametry a jestli funkce existuje
/// @param stack 
/// @param table 
/// @param statement 
/// @return pokud je metoda sémanticky správně, vrátí se 0, jinak číslo erroru
int semantic_function(VarTableStack *stack, FuncTable *func_table, Statement *statement) {
    int ret;

    Function *func = func_table_get(func_table, statement->func.id);

    if(func == NULL){ // funkce jeste nebyla ani definovana, ani zavolana

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

        new_function.return_type = statement->func.return_type;
        func_table_insert(func_table, new_function);
    }
    else {
        if(func->is_defined == true){
            ERROR;
        }
        else{ // funkce jiz byla zavolana
            Function called_function = *func;

            func->is_defined = true;
            func->param_count = statement->func.param_count;
            func->parameters = malloc(sizeof(Parameter) * (statement->func.param_count));
            if(func->parameters == NULL){
                exit(99);
            }

            if(func->param_count != called_function.param_count){
                printf("FAILED HERE: %d\n", __LINE__);
                SEMANTIC_ERROR_COUNT_OR_TYPE_OF_PARAM_IS_WRONG;
            }

            for (size_t i = 0; i < statement->func.param_count; ++i) {
                Parameter *param_in = func->parameters + i;
                const Parameter *param_out = statement->func.parameters + i;

                param_in->extern_id = param_out->extern_id;
                param_in->intern_id = param_out->intern_id;
                param_in->type = param_out->type;
            }

            func->return_type = statement->func.return_type;
            func_table_insert(func_table, called_function);

            //check_params1
            for(size_t i = 0; i < func->param_count; i++){
                if(func->parameters[i].extern_id != called_function.parameters[i].extern_id){
                    ERROR;
                }

                ret = semantic_type_match(&func->parameters[i].type, &called_function.parameters[i].type);
                if (ret != 0)
                    return ret;

            }
        }
    }

    VarTable *var_table;
    vartable_stack_peek(stack, &var_table);
    for(size_t i = 0; i < statement->func.param_count; i++){
        DataType dt = statement->func.parameters[i].type;
        Variable var = {
            .id = statement->func.parameters[i].intern_id,
            .initialized = true,
            .type = dt
        };
        var_table_insert(var_table, var);
    }

    return 0;
}

// Function call
int semantic_function_call(VarTableStack *stack, FuncTable *func_table, Expression *exp) {

    Function *func = func_table_get(func_table, exp->fn_call.id);

    int ret;

    if(func == NULL){
        Function func;
        func.id = exp->fn_call.id;
        func.is_defined = false;
        func.param_count = exp->fn_call.arg_count;

        // naplni argumenty, se kterymi byla zavolana
        func.parameters = malloc(sizeof(Parameter) * (exp->fn_call.arg_count));
            if(func.parameters == NULL){
                exit(99);
            }

            for (size_t i = 0; i < exp->fn_call.arg_count; ++i) {
                //CHECK
                Parameter *param_in = func.parameters + i;
                const Argument *argument = exp->fn_call.args + i;

                param_in->extern_id = argument->id;
                param_in->type = argument->exp->data_type;
            }

            func_table_insert(func_table, func);
        
    } else { // uz je definovana
        // muzou se zkontrolovat argumenty
        if (func->is_write && func->param_count < 1) {
            printf("FAILED HERE: %d\n", __LINE__);
            SEMANTIC_ERROR_COUNT_OR_TYPE_OF_PARAM_IS_WRONG;
        }

        for (size_t i = 0; i < func->param_count; ++i) {
            ret = set_type(stack, func_table, exp);

            if (ret != 0)
                return ret;

            if (func->is_write) continue; // na write se vola jenom set_var

            //check_params1
            if(func->param_count == exp->fn_call.arg_count){
                for(size_t i = 0; i < func->param_count; i++){
                    if(func->parameters[i].extern_id != exp->fn_call.args[i].id){
                        ERROR;
                    }
                    
                    //CHECK
                    ret = semantic_type_match(&func->parameters[i].type, &exp->fn_call.args[i].exp->data_type);
                    if (ret != 0)
                        return ret;
                }
            }
            else{
                printf("FAILED HERE: %d\n", __LINE__);
                SEMANTIC_ERROR_COUNT_OR_TYPE_OF_PARAM_IS_WRONG;
            }
        }
        
    }
    return 0;
}

// zkontroluje jestli volane funkce byly definovany
int semantic_were_all_functions_defined( FuncTable *func_table ) {
    
    for (size_t i = 0; i < func_table->funcs_size; i++) {
        if (func_table->funcs[i].is_defined == false) {
            printf("Funkce: '%s' neni definovana: Line: %d\n", func_table->funcs[i].id->symbol.data, __LINE__);
            SEMANTIC_ERROR_UNDEFINED_FUNCTION;
        }
    }

    return 0;
}
