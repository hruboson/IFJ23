#include "rule_tree.h"
#include "unity.h"

RuleNode* tree;

//! always needs to be defined
void setUp() {
    init_rule_tree(&tree);
}
void tearDown() {
    dispose_rule_tree(tree);
}

void test_rule_tree_init(void) {
    RuleNode* tree_init;

    init_rule_tree(&tree_init);

    TEST_ASSERT_NOT_NULL(tree_init);
    TEST_ASSERT_NULL(tree->right);
    TEST_ASSERT_NULL(tree->left);
    TEST_ASSERT_NULL(tree->exp);
}

void test_rule_tree_clear(void) {
    RuleNode* tree_clear;

    init_rule_tree(&tree_clear);

    Expression el_1 = {.type = ET_INT};
    Expression el_2 = {.type = ET_INT};
    Expression el = {.type = ET_ADD,
                    .ops = {&el_1, &el_2}};
    rule_tree_insert_left(tree_clear, &el);
    TEST_ASSERT_EQUAL_PTR(&el, tree_clear->left->exp);
    Expression er_1 = {.type = ET_INT};
    Expression er_2 = {.type = ET_INT};
    Expression er = {.type = ET_ADD,
                    .ops = {&er_1, &er_2}};
    rule_tree_insert_right(tree_clear, &er);
    TEST_ASSERT_EQUAL_PTR(&er, tree_clear->right->exp);

	dispose_rule_tree(tree_clear);
	TEST_ASSERT_NULL(tree_clear);
}

void test_rule_tree_insert_left(void) {
    Expression e1 = {.type = ET_INT};
    Expression e2 = {.type = ET_INT};
    Expression e = {.type = ET_ADD,
                    .ops = {&e1, &e2}};
    rule_tree_insert_left(tree, &e);
    TEST_ASSERT_EQUAL_PTR(&e, tree->left->exp);
}

void test_rule_tree_insert_right(void) {
    Expression e1 = {.type = ET_INT};
    Expression e2 = {.type = ET_INT};
    Expression e = {.type = ET_ADD,
                    .ops = {&e1, &e2}};
    rule_tree_insert_right(tree, &e);
    TEST_ASSERT_EQUAL_PTR(&e, tree->right->exp);
}

int main(void) {
    UNITY_BEGIN();
	RUN_TEST(test_rule_tree_init);
	RUN_TEST(test_rule_tree_clear);
    RUN_TEST(test_rule_tree_insert_left);
    RUN_TEST(test_rule_tree_insert_right);

    return UNITY_END();
}