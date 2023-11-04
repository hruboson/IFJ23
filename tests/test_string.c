#include "../include/string_.h"
#include "unity.h"

String _string;
String *string = &_string;

//! always needs to be defined
void setUp() {
    init_string(string);
}
void tearDown() {
    clear_string(string);
}

/**
 * Basic tests
 */
void test_init_string(void) {
    TEST_ASSERT_NOT_NULL(string);
    TEST_ASSERT_EQUAL(STR_INITIAL_CAPACITY, string->capacity);
    TEST_ASSERT_EQUAL(0, string->length);
}

void test_clear_string(void) {
    String testString;
    init_string(&testString);

    clear_string(&testString);

    TEST_ASSERT_NOT_NULL(&testString);
    TEST_ASSERT_NULL(testString.data);
    TEST_ASSERT_EQUAL_INT(0, testString.capacity);
    TEST_ASSERT_EQUAL_INT(0, testString.length);
}

void test_string_append_to_empty(void) {
    char *appended_string = "test string";
    string_append(string, appended_string);

    TEST_ASSERT_EQUAL_STRING(appended_string, string->data);
}
void test_string_append_char(void) {
    string_append_c(string, 'c');
    TEST_ASSERT_EQUAL_STRING("c", string->data);
}

/**
 * Advanced tests
 */
void test_adv_append_long_twice(void) {
    const char *appended_string_1 = "Well, the way they make shows is, they make one show.That show 's called a pilot. Then they show that show to the people who make shows, and on the strength of that one show they decide if they' re going to make more shows.Some pilots get picked and become television programs.Some don't, become nothing. She starred in one of the ones that became nothing.";
    const char *appended_string_2 = "My money's in that office, right? If she start giving me some bullshit about it ain't there, and we got to go someplace else and get it, I'm gonna shoot you in the head then and there. Then I'm gonna shoot that bitch in the kneecaps, find out where my goddamn money is. She gonna tell me too. Hey, look at me when I'm talking to you, motherfucker. You listen: we go in there, and that nigga Winston or anybody else is in there, you the first motherfucker to get shot. You understand?";

    string_append(string, appended_string_1);
    TEST_ASSERT_EQUAL_STRING("Well, the way they make shows is, they make one show.That show 's called a pilot. Then they show that show to the people who make shows, and on the strength of that one show they decide if they' re going to make more shows.Some pilots get picked and become television programs.Some don't, become nothing. She starred in one of the ones that became nothing.", string->data);

    string_append(string, appended_string_2);
    TEST_ASSERT_EQUAL_STRING("Well, the way they make shows is, they make one show.That show 's called a pilot. Then they show that show to the people who make shows, and on the strength of that one show they decide if they' re going to make more shows.Some pilots get picked and become television programs.Some don't, become nothing. She starred in one of the ones that became nothing.My money's in that office, right? If she start giving me some bullshit about it ain't there, and we got to go someplace else and get it, I'm gonna shoot you in the head then and there. Then I'm gonna shoot that bitch in the kneecaps, find out where my goddamn money is. She gonna tell me too. Hey, look at me when I'm talking to you, motherfucker. You listen: we go in there, and that nigga Winston or anybody else is in there, you the first motherfucker to get shot. You understand?", string->data);
}

void test_adv_string_append_char(void) {
    for (int i = 0; i < 100; i++) {
        string_append_c(string, '0' + (i % 10));
    }
    TEST_ASSERT_EQUAL_STRING("0123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789", string->data);

    for (int i = 0; i < 200; i++) {
        string_append_c(string, '0' + (i % 10));
    }
    TEST_ASSERT_EQUAL_STRING("012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789", string->data);
}

int main(void) {
    UNITY_BEGIN();

    // Basic tests
    RUN_TEST(test_init_string);
    RUN_TEST(test_clear_string);
    RUN_TEST(test_string_append_to_empty);
    RUN_TEST(test_string_append_char);

    // Advanced tests
    RUN_TEST(test_adv_append_long_twice);
    RUN_TEST(test_adv_string_append_char);

    return UNITY_END();
}
