#include "string.h"
#include "unity.h"

//! always needs to be defined
void setUp() {}
void tearDown() {}

void string_init(void){
    TEST_ASSERT_TRUE(0);
}

int main(void) {
    UNITY_BEGIN();
    RUN_TEST(string_init);
    return UNITY_END();
}