#pragma once

#include "expression.h"

typedef struct RuleNode {
    RuleNode* left;
    RuleNode* right;
    Expression* exp;
} RuleNode;