#include "../include/AST.h"
#include "unity.h"
#include "parser.h"

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

void test_ast_var_print(void) {
    const char* data = "var num1 : Int = 5\n var num2 : Double = 5.0\n";
    Input in = {
        .type = INT_STRING,
        .string = {
            .s = data,
            .i = 0,
            .store = 0,
        },
    };

    SymbolTable symtab;

    init_symboltable(&symtab);

    AST ast;
    init_ast(&ast);

    int ret = parse(&in, &ast);

    print_ast(&ast);
}

void test_ast_while_print(void) {
    const char* data = "while 1 { var a : Int = 3\n }";
    Input in = {
        .type = INT_STRING,
        .string = {
            .s = data,
            .i = 0,
            .store = 0,
        },
    };

    SymbolTable symtab;

    init_symboltable(&symtab);

    AST ast;
    init_ast(&ast);

    int ret = parse(&in, &ast);

    print_ast(&ast);
}

int main(void) {
    UNITY_BEGIN();
    RUN_TEST(test_ast_init);
    RUN_TEST(test_ast_clear);
    RUN_TEST(test_ast_single_append);
    RUN_TEST(test_ast_multiple_append);

    // check these in ./build/results/test_AST.txt
    RUN_TEST(test_ast_var_print);
    RUN_TEST(test_ast_while_print);

    return UNITY_END();
}