#include "rule_stack.h"
#include "unity.h"

RuleStack stack_;
RuleStack* stack = &stack_;

//! always needs to be defined
void setUp() {
    init_rule_stack(stack);
}
void tearDown() {
    clear_rule_stack(stack);
}

void test_rule_stack_init_clear(void) {
    RuleStack stack_;
    RuleStack* stack = &stack_;
    init_rule_stack(stack);

    TEST_ASSERT_NOT_NULL(stack->rules);
    TEST_ASSERT_EQUAL_INT(-1, stack->size);
    TEST_ASSERT_EQUAL_INT(STACK_SIZE, stack->capacity);

    Rule r_;
    Rule* r = &r_;
    rule_stack_push(stack, r);
    TEST_ASSERT_EQUAL_PTR(r, stack->rules[0]);

    clear_rule_stack(stack);
    TEST_ASSERT_NULL(stack->rules);
}

void test_rule_stack_push(void) {
    Rule r_;
    Rule* r = &r_;
    rule_stack_push(stack, r);
    TEST_ASSERT_EQUAL_PTR(r, stack->rules[0]);
}

void test_rule_stack_push_multiple(void) {
    Rule r_;
    Rule* r = &r_;
    int i = 0;
    for (; i < STACK_SIZE + 2; i++) {
        rule_stack_push(stack, r);
    }
    TEST_ASSERT_EQUAL_PTR(r, stack->rules[0]);
    TEST_ASSERT_EQUAL_PTR(r, stack->rules[1]);
    TEST_ASSERT_EQUAL_PTR(r, stack->rules[STACK_SIZE]);
    TEST_ASSERT_EQUAL_PTR(r, stack->rules[STACK_SIZE + 1]);
}

void test_rule_stack_pop(void) {
    Rule r_;
    Rule* r = &r_;
    rule_stack_push(stack, r);

    Rule* r_pop;
    rule_stack_pop(stack, r_pop);
    TEST_ASSERT_EQUAL_PTR(r, r_pop);
}

void test_rule_stack_is_empty(void) {
    Rule r_;
    Rule* r = &r_;
    rule_stack_push(stack, r);

    Rule* r_pop;
    rule_stack_pop(stack, r_pop);

    bool empty = rule_stack_is_empty(stack);
    TEST_ASSERT_TRUE(empty);

    rule_stack_push(stack, r);
    empty = rule_stack_is_empty(stack);
    TEST_ASSERT_FALSE(empty);
}

int main(void) {
    UNITY_BEGIN();
    RUN_TEST(test_rule_stack_init_clear);
    RUN_TEST(test_rule_stack_push);
    RUN_TEST(test_rule_stack_push_multiple);
    RUN_TEST(test_rule_stack_pop);
    RUN_TEST(test_rule_stack_is_empty);
    return UNITY_END();
}