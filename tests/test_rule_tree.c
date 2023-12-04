#include "rule_tree.h"
#include "unity.h"

//! always needs to be defined
void setUp() {}
void tearDown() {}

void test_(void) {
    TEST_ASSERT_TRUE(0);
}

int main(void) {
    UNITY_BEGIN();
    RUN_TEST(test_);
    return UNITY_END();
}