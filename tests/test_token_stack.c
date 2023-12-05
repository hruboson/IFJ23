#include "token_stack.h"
#include "unity.h"

TokenStack stack_;
TokenStack* stack = &stack_;

//! always needs to be defined
void setUp() {
    init_token_stack(stack);
}
void tearDown() {
    clear_token_stack(stack);
}

void test_token_stack_init_clear(void) {
    TokenStack stack_;
    TokenStack* stack = &stack_;
    init_token_stack(stack);

    TEST_ASSERT_NOT_NULL(stack->tokens);
    TEST_ASSERT_EQUAL_INT(-1, stack->size);
    TEST_ASSERT_EQUAL_INT(STACK_SIZE, stack->cap);

    Token token_;
    Token* token = &token_;
    token_stack_push(stack, token);
    TEST_ASSERT_EQUAL_PTR(token, stack->tokens[0]);

    clear_token_stack(stack);
    TEST_ASSERT_NULL(stack->tokens);
}

void test_token_stack_push(void) {
    Token token_;
    Token* token = &token_;
    token_stack_push(stack, token);
    TEST_ASSERT_EQUAL_PTR(token, stack->tokens[0]);
}

void test_token_stack_push_multiple(void) {
    Token token_;
    Token* token = &token_;
    int i = 0;
    for (; i < STACK_SIZE + 2; i++) {
        token_stack_push(stack, token);
    }
    TEST_ASSERT_EQUAL_PTR(token, stack->tokens[0]);
    TEST_ASSERT_EQUAL_PTR(token, stack->tokens[1]);
    TEST_ASSERT_EQUAL_PTR(token, stack->tokens[STACK_SIZE]);
    TEST_ASSERT_EQUAL_PTR(token, stack->tokens[STACK_SIZE + 1]);
}

void test_token_stack_pop(void) {
    Token token_;
    Token* token = &token_;
    token_stack_push(stack, token);

    Token* r_pop;
    token_stack_pop(stack, &r_pop);
    TEST_ASSERT_EQUAL_PTR(token, r_pop);
}

void test_token_stack_is_empty(void) {
    Token token_;
    Token* token = &token_;
    token_stack_push(stack, token);

    Token* r_pop;
    token_stack_pop(stack, &r_pop);

    bool empty = token_stack_is_empty(stack);
    TEST_ASSERT_TRUE(empty);

    token_stack_push(stack, token);
    empty = token_stack_is_empty(stack);
    TEST_ASSERT_FALSE(empty);
}

int main(void) {
    UNITY_BEGIN();
    RUN_TEST(test_token_stack_init_clear);
    RUN_TEST(test_token_stack_push);
    RUN_TEST(test_token_stack_push_multiple);
    RUN_TEST(test_token_stack_pop);
    RUN_TEST(test_token_stack_is_empty);
    return UNITY_END();
}