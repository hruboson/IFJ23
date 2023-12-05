#include "exp_stack.h"
#include "unity.h"

ExpStack stack_;
ExpStack* stack = &stack_;

//! always needs to be defined
void setUp() {
    init_exp_stack(stack);
}
void tearDown() {
    clear_exp_stack(stack);
}

void test_exp_stack_init_clear(void) {
    ExpStack stack_;
    ExpStack* stack = &stack_;
    init_exp_stack(stack);

    TEST_ASSERT_NOT_NULL(stack->exps);
    TEST_ASSERT_EQUAL_INT(-1, stack->size);
    TEST_ASSERT_EQUAL_INT(STACK_SIZE, stack->capacity);

    Expression exp_;
    Expression* exp = &exp_;
    exp_stack_push(stack, exp);
    TEST_ASSERT_EQUAL_PTR(exp, stack->exps[0]);

    clear_exp_stack(stack);
    TEST_ASSERT_NULL(stack->exps);
}

void test_exp_stack_push(void) {
    Expression exp_;
    Expression* exp = &exp_;
    exp_stack_push(stack, exp);
    TEST_ASSERT_EQUAL_PTR(exp, stack->exps[0]);
}

void test_exp_stack_push_multiple(void) {
    Expression exp_;
    Expression* exp = &exp_;
    int i = 0;
    for (; i < STACK_SIZE + 2; i++) {
        exp_stack_push(stack, exp);
    }
    TEST_ASSERT_EQUAL_PTR(exp, stack->exps[0]);
    TEST_ASSERT_EQUAL_PTR(exp, stack->exps[1]);
    TEST_ASSERT_EQUAL_PTR(exp, stack->exps[STACK_SIZE]);
    TEST_ASSERT_EQUAL_PTR(exp, stack->exps[STACK_SIZE + 1]);
}

void test_exp_stack_pop(void) {
    Expression exp_;
    Expression* exp = &exp_;
    exp_stack_push(stack, exp);

    Expression* r_pop;
    exp_stack_pop(stack, &r_pop);
    TEST_ASSERT_EQUAL_PTR(exp, r_pop);
}

void test_exp_stack_is_empty(void) {
    Expression exp_;
    Expression* exp = &exp_;
    exp_stack_push(stack, exp);

    Expression* r_pop;
    exp_stack_pop(stack, &r_pop);

    bool empty = exp_stack_is_empty(stack);
    TEST_ASSERT_TRUE(empty);

    exp_stack_push(stack, exp);
    empty = exp_stack_is_empty(stack);
    TEST_ASSERT_FALSE(empty);
}

int main(void) {
    UNITY_BEGIN();
    RUN_TEST(test_exp_stack_init_clear);
    RUN_TEST(test_exp_stack_push);
    RUN_TEST(test_exp_stack_push_multiple);
    RUN_TEST(test_exp_stack_pop);
    RUN_TEST(test_exp_stack_is_empty);
    return UNITY_END();
}