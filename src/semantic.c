#include "semantic.h"

// var x = 5
bool semantic_variable(VarTableStack *stack, FuncTable *table, Statement *statement)
{
    Variable *get_var = var_table_stack_get_var(&stack, statement->var.exp->id);
    if (!get_var)
    {
        fprintf(stderr, "Chyba: %s (%s:%d): var_table_stack_get_var je neprázný\n", __func__, __FILE__, __LINE__);
        return false;
    }

    switch (get_var->type.type){
        
        case VARTYPE_INT: // Pokud je proměnná typu int
            if (statement->type == ET_INT){
                //&statement->var.exp->int_; // v int_ bude uložena 5ka
                get_var->initialized = true;
                return true;
            }
            else{
                fprintf(stderr, "Chyba: %s (%s:%d): nesoulad typů mezi proměnnou a výrazem\n", __func__, __FILE__, __LINE__);
                return false;
            }
            break;
        
        case VARTYPE_DOUBLE: // Pokud je proměnná typu double
            if (statement->type == ET_DOUBLE){
                //&statement->var.exp->double_;
                get_var->initialized = true;
                return true;
            }
            else{
                fprintf(stderr, "Chyba: %s (%s:%d): nesoulad typů mezi proměnnou a výrazem\n", __func__, __FILE__, __LINE__);
                return false;
            }
            break;
        
        case VARTYPE_STRING: // Pokud je proměnná typu string
            if (statement->type == ET_STRING) 
            {
                //&statement->var.exp->str_;
                get_var->initialized = true;
                return true;
            }
            else
            {
                fprintf(stderr, "Chyba: %s (%s:%d): nesoulad typů mezi proměnnou a výrazem\n", __func__, __FILE__, __LINE__);
                return false;
            }
            break;
        
        case VARTYPE_VOID: // Pokud je proměnná typu void
            if (statement->type == ET_STRING || statement->type == ET_INT || statement->type == ET_DOUBLE) // Zkontrolujte, zda je výraz typu string, int nebo double
            {
                get_var->initialized = true;
                return true;
            }
            else 
            {
                fprintf(stderr, "Chyba: %s (%s:%d): nesoulad typů mezi proměnnou a výrazem\n", __func__, __FILE__, __LINE__);
                return false;
            }
            break;
        default: // Pokud je proměnná neznámého typu
            fprintf(stderr, "Chyba: %s (%s:%d): neznámý typ proměnné\n", __func__, __FILE__, __LINE__);
            return false;
            break;
    }
    return false;
}