#include "../include/string.h"

#include "unity.h"

String _string;
String *string = &_string;

//! always needs to be defined
void setUp() {
    init_string(string);
}
void tearDown() {
    free_string(string);
}

void test_init_string(void) {
    TEST_ASSERT_NOT_NULL(string);
    TEST_ASSERT_EQUAL(STR_INITIAL_CAPACITY, string->capacity);
    TEST_ASSERT_EQUAL(0, string->length);
}

void test_free_string(void) {
    String testString;
    init_string(&testString);

    free_string(&testString);

    TEST_ASSERT_NOT_NULL(&testString);
    TEST_ASSERT_NULL(testString.data);
    TEST_ASSERT_EQUAL_INT(0, testString.capacity);
    TEST_ASSERT_EQUAL_INT(0, testString.length);
}

void test_append_string_to_empty(void) {
    char *appended_string = "test string";
    append_string(string, appended_string);
    
    TEST_ASSERT_EQUAL_STRING(appended_string, string->data);
}

// void test_append_string_char(void) {
//     append_string_c(string, 'c');
    
//     TEST_ASSERT_EQUAL_STRING('c', string->data);
// }


int main(void) {
    UNITY_BEGIN();
    RUN_TEST(test_init_string);
    RUN_TEST(test_free_string);
    RUN_TEST(test_append_string_to_empty);
    return UNITY_END();
}