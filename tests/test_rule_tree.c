#include "rule_tree.h"
#include "unity.h"

Node* tree;

//! always needs to be defined
void setUp() {
    init_rule_tree(&tree);
}
void tearDown() {
    dispose_rule_tree(tree);
}

void test_rule_tree_init(void) {
    Node* tree_init;

    init_rule_tree(&tree_init);

    TEST_ASSERT_NOT_NULL(tree_init);
}

void test_rule_tree_clear(void) {
   	Node* tree_clear;

    init_rule_tree(&tree_clear);

    Expression el_1 = {.type = ET_INT};
    Expression el_2 = {.type = ET_INT};
    Expression el = {.type = ET_ADD,
                    .ops = {&el_1, &el_2}};
    
    
    Expression er_1 = {.type = ET_INT};
    Expression er_2 = {.type = ET_INT};
    Expression er = {.type = ET_ADD,
                    .ops = {&er_1, &er_2}};



	dispose_rule_tree(tree_clear);
	TEST_ASSERT_NULL(tree_clear);
}

void test_rule_tree_insert(void) {
    Expression e1 = {.type = ET_INT};
    Expression e2 = {.type = ET_INT};
    Expression e = {.type = ET_ADD,
                    .ops = {&e1, &e2}};


}

int main(void) {
    UNITY_BEGIN();
	RUN_TEST(test_rule_tree_init);
	RUN_TEST(test_rule_tree_clear);
    RUN_TEST(test_rule_tree_insert);

    return UNITY_END();
}