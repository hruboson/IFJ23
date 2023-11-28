#include "unity.h"
#include "../include/vartable_stack.h"
#include "stdlib.h"
#include "symtable.h"

//! always needs to be defined
void setUp() {}
void tearDown() {}

void test_init_symtable_stack_with_valid_pointer(void) {
    // test that the function initializes the stack correctly when the stack pointer is valid
    VarTableStack* stack = malloc(sizeof(VarTableStack));
    init_vartable_stack(stack);
    TEST_ASSERT_EQUAL_INT(-1, stack->size); // check that the size is -1
    TEST_ASSERT_NOT_NULL(stack->vartables); // check that the symtables pointer is not NULL
    free(stack->vartables); // free the allocated memory
    free(stack); // free the allocated memory
}

int main(void){
    UNITY_BEGIN();

    RUN_TEST(test_init_symtable_stack_with_valid_pointer);

    return UNITY_END();
}
//todo