#include "example.h"
#include "unity.h"

//! always needs to be defined
void setUp() {}
void tearDown() {}

void test_AverageThreeBytes_should_AverageMidRangeValues(void) {
    TEST_ASSERT_EQUAL_HEX8(39, AverageThreeBytes(30, 40, 50));
    TEST_ASSERT_EQUAL_HEX8(54, AverageThreeBytes(10, 70, 40));
    TEST_ASSERT_EQUAL_HEX8(33, AverageThreeBytes(33, 33, 33));
}

void test_AverageThreeBytes_should_AverageHighValues(void) {
    TEST_ASSERT_EQUAL_HEX8(81, AverageThreeBytes(70, 80, 90));
    TEST_ASSERT_EQUAL_HEX8(127, AverageThreeBytes(127, 127, 127));
    TEST_ASSERT_EQUAL_HEX8(84, AverageThreeBytes(0, 126, 126));
}

int main(void) {
    UNITY_BEGIN();
    RUN_TEST(test_AverageThreeBytes_should_AverageMidRangeValues);
    RUN_TEST(test_AverageThreeBytes_should_AverageHighValues);
    return UNITY_END();
}