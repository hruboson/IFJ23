#include "rule_tree.h"

#include "rule_stack.h"
#include "rules.h"
#include "stdlib.h"

void init_rule_tree(RuleNode* tree) {
    tree->exp = NULL;
    tree->left = NULL;
    tree->right = NULL;
}
void dispose_rule_tree(RuleNode* tree) {
    dispose_rule_tree(tree->left);
    dispose_rule_tree(tree->right);
}
void rule_tree_insert_left(RuleNode* root, RuleNode* rule) {
    if (!rule) exit(99);
    rule->left = NULL;
    rule->right = NULL;
    root->left = rule;
}
void rule_tree_insert_right(RuleNode* root, RuleNode* rule) {
    if (!rule) exit(99);
    rule->left = NULL;
    rule->right = NULL;
    root->right = rule;
}
void rule_tree_postorder(RuleNode* tree, RuleStack* stack) {
    if (!tree) return;

    rule_tree_postorder(tree->left, stack);
    rule_tree_postorder(tree->right, stack);
    rule_stack_push(stack, tree);
}