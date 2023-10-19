#include "semantic.h"
#include "unity.h"

//! always needs to be defined
void setUp() {}
void tearDown() {}

void test_semantic(void) {
    TEST_ASSERT_TRUE(0);
}

int main(void) {
    UNITY_BEGIN();
    RUN_TEST(test_semantic);
    return UNITY_END();
}