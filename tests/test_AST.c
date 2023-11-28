#include "../include/AST.h"
#include "unity.h"

//! always needs to be defined
void setUp() {
}
void tearDown() {
}

void test_init(void) {
    TEST_ASSERT(0 == "TODO TESTS FOR AST");
}

int main(void) {
    UNITY_BEGIN();
    RUN_TEST(test_init);
    return UNITY_END();
}