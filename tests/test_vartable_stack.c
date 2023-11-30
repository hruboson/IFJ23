#include "../include/vartable_stack.h"
#include "stdlib.h"
#include "symtable.h"
#include "unity.h"
#include "table.h"

//! always needs to be defined
void setUp() {}
void tearDown() {}

void test_init_symtable_stack_with_valid_pointer(void) {
    // test that the function initializes the stack correctly when the stack pointer is valid
    VarTableStack* stack = malloc(sizeof(VarTableStack));
    init_vartable_stack(stack);
    TEST_ASSERT_EQUAL_INT(-1, stack->size);  // check that the size is -1
    TEST_ASSERT_NOT_NULL(stack->vartables);  // check that the symtables pointer is not NULL
    free(stack->vartables);                  // free the allocated memory
    free(stack);                             // free the allocated memory
}

void test_get_var_from_stack() {
    VarTableStack* stack = malloc(sizeof(VarTableStack));
    init_vartable_stack(stack);
    TEST_ASSERT_EQUAL_INT(-1, stack->size);  // check that the size is -1
    TEST_ASSERT_NOT_NULL(stack->vartables);  // check that the symtables pointer is not NULL

    VarTable table_;
    VarTable* table = &table_;
    init_var_table(table);

    SymbolRecord id0 = {
        .hash = 99999,
        .symbol = NULL};
    SymbolRecord id1 = {
        .hash = 213123,
        .symbol = NULL};
    Variable var1 = {
        .id = &id1,
        .initialized = true,
        .type = VARTYPE_INT};
    SymbolRecord id2 = {
        .hash = 112312,
        .symbol = NULL};
    Variable var2 = {
        .id = &id2,
        .initialized = false,
        .type = VARTYPE_DOUBLE};
    var_table_insert(table, var1);
    var_table_insert(table, var2);
    vartable_stack_push(stack, table);

    Variable* find = var_table_stack_get_var(stack, &id1);
    TEST_ASSERT_NOT_NULL(find);
    find = var_table_stack_get_var(stack, &id2);
    TEST_ASSERT_NOT_NULL(find);
    find = var_table_stack_get_var(stack, &id0);
    TEST_ASSERT_NULL(find);

    clear_var_table(table);
    free(stack->vartables);  // free the allocated memory
    free(stack);             // free the allocated memory
}

int main(void) {
    UNITY_BEGIN();

    RUN_TEST(test_init_symtable_stack_with_valid_pointer);
    RUN_TEST(test_get_var_from_stack);

    return UNITY_END();
}
// todo