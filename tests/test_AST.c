#include "../include/AST.h"
#include "unity.h"

AST a_;
AST* a = &a_;

//! always needs to be defined
void setUp() {
    init_ast(a);
}
void tearDown() {
    clear_ast(a);
}

void test_ast_init(void) {
    AST init_;
    AST* init = &init_;
    init_ast(init);
    TEST_ASSERT_NULL(init->statement);
    clear_ast(init);
}

void test_ast_clear(void) {
    AST clear_;
    AST* clear = &clear_;
    init_ast(clear);
    TEST_ASSERT_NULL(clear->statement);

    Statement st1;
    st1.next = NULL;
    Statement st2;
    st2.next = NULL;
    Statement st3;
    st3.next = NULL;
    Statement st4;
    st4.next = NULL;
    ast_append(clear, &st1);
    TEST_ASSERT_NOT_NULL(clear->statement);

    ast_append(clear, &st2);
    ast_append(clear, &st3);
    ast_append(clear, &st4);

    clear_ast(clear);
    TEST_ASSERT_NULL(clear->statement);
}

void test_ast_single_append(void) {
    Statement st;
    st.next = NULL;
    ast_append(a, &st);
    TEST_ASSERT_NOT_NULL(a->statement);
    TEST_ASSERT_NULL(a->statement->next);
}

void test_ast_multiple_append(void) {
    Statement st1;
    st1.next = NULL;
    Statement st2;
    st2.next = NULL;
    Statement st3;
    st3.next = NULL;
    ast_append(a, &st1);
    ast_append(a, &st2);
    ast_append(a, &st3);
    TEST_ASSERT_NOT_NULL(a->statement);
    TEST_ASSERT_NOT_NULL(a->statement->next);
    TEST_ASSERT_NOT_NULL(a->statement->next->next);
}

void test_ast_first(void) {
    Statement st1;
    st1.next = NULL;
    Statement st2;
    st2.next = NULL;
    Statement st3;
    st3.next = NULL;
    ast_append(a, &st1);
    ast_append(a, &st2);
    ast_append(a, &st3);
    TEST_ASSERT_NOT_NULL(a->statement);
    TEST_ASSERT_NOT_NULL(a->statement->next);
    TEST_ASSERT_NOT_NULL(a->statement->next->next);

    Statement* first;
    ast_first(a, first);
    TEST_ASSERT_EQUAL_PTR(&st1, first);
}

void test_ast_delete_last(void) {
    Statement st1;
    st1.next = NULL;
    Statement st2;
    st2.next = NULL;
    Statement st3;
    st3.next = NULL;
    ast_append(a, &st1);
    ast_append(a, &st2);
    ast_append(a, &st3);
    TEST_ASSERT_NOT_NULL(a->statement);
    TEST_ASSERT_NOT_NULL(a->statement->next);
    TEST_ASSERT_NOT_NULL(a->statement->next->next);

    ast_delete_last(a);
    TEST_ASSERT_NULL(a->statement->next->next);
    ast_delete_last(a);
    TEST_ASSERT_NULL(a->statement->next);
    ast_delete_last(a);
    TEST_ASSERT_NULL(a->statement);
}

int main(void) {
    UNITY_BEGIN();
    RUN_TEST(test_ast_init);
    RUN_TEST(test_ast_clear);
    RUN_TEST(test_ast_single_append);
    RUN_TEST(test_ast_multiple_append);
    RUN_TEST(test_ast_first);
    RUN_TEST(test_ast_delete_last);
    return UNITY_END();
}