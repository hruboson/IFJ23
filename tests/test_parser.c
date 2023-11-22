#include "parser.h"
#include "unity.h"

//! always needs to be defined
void setUp() {
    // setup file
}
void tearDown() {
    // close file
}

void test_parser(void) {
    TEST_ASSERT_TRUE(0);
}

int main(void) {
    UNITY_BEGIN();
    RUN_TEST(test_parser);
    return UNITY_END();
}