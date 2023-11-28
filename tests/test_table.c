#include "../include/table.h"
#include "unity.h"

FuncTable func_table_;
FuncTable* func_table = &func_table_;

//! always needs to be defined
void setUp() {
    init_func_table(func_table);
}
void tearDown() {
    clear_func_table(func_table);
}

void test_func_table_init() {
    FuncTable init_table_;
    FuncTable* init_table = &init_table_;
    init_func_table(init_table);
    TEST_ASSERT_NULL(init_table->funcs);
    TEST_ASSERT_EQUAL_size_t(0, init_table->funcs_cap);
    TEST_ASSERT_EQUAL_size_t(0, init_table->funcs_size);
}

void test_func_table_clear() {
    FuncTable clear_table_;
    FuncTable* clear_table = &clear_table_;
    init_func_table(clear_table);

    Function f;
    SymbolRecord r;
    r.hash = 9129030;
    f.id = &r;

    for (size_t i = 0; i < 65; i++) {
        func_table_insert(clear_table, f);
    }

	clear_func_table(clear_table);

	TEST_ASSERT_NULL(clear_table->funcs);
	TEST_ASSERT_EQUAL_size_t(0, clear_table->funcs_cap);
    TEST_ASSERT_EQUAL_size_t(0, clear_table->funcs_size);
}

void test_func_table_insert_empty() {
    Function f;
    SymbolRecord r;
    f.id = &r;
    func_table_insert(func_table, f);
    TEST_ASSERT_EQUAL_size_t(FUNC_TABLE_INIT_SIZE, func_table->funcs_cap);
    TEST_ASSERT_EQUAL_size_t(1, func_table->funcs_size);
    TEST_ASSERT_NOT_NULL(func_table->funcs);

    TEST_ASSERT_NOT_NULL(&(func_table->funcs[0]));
    TEST_ASSERT_NOT_NULL(func_table->funcs[0].id);
}

void test_func_table_insert() {
    Function f;
    SymbolRecord r;
    r.hash = 11029;
    f.id = &r;

    func_table_insert(func_table, f);
    TEST_ASSERT_EQUAL_size_t(FUNC_TABLE_INIT_SIZE, func_table->funcs_cap);
    TEST_ASSERT_EQUAL_size_t(1, func_table->funcs_size);
    TEST_ASSERT_NOT_NULL(func_table->funcs);

    TEST_ASSERT_NOT_NULL(&(func_table->funcs[0]));
    TEST_ASSERT_NOT_NULL(func_table->funcs[0].id);
    TEST_ASSERT_EQUAL_size_t(11029, func_table->funcs[0].id->hash);
}

void test_func_table_insert_multiple() {
    Function f;
    SymbolRecord r;
    r.hash = 112308120;
    f.id = &r;

    func_table_insert(func_table, f);
    func_table_insert(func_table, f);
    func_table_insert(func_table, f);
    func_table_insert(func_table, f);

    TEST_ASSERT_EQUAL_size_t(FUNC_TABLE_INIT_SIZE, func_table->funcs_cap);
    TEST_ASSERT_EQUAL_size_t(4, func_table->funcs_size);
    TEST_ASSERT_NOT_NULL(func_table->funcs);

    TEST_ASSERT_NOT_NULL(&(func_table->funcs[0]));
    TEST_ASSERT_NOT_NULL(func_table->funcs[0].id);
    TEST_ASSERT_EQUAL_size_t(112308120, func_table->funcs[3].id->hash);
}

void test_func_table_insert_over_init_size() {
    Function f;
    SymbolRecord r;
    r.hash = 9129030;
    f.id = &r;

    for (size_t i = 0; i < 65; i++) {
        func_table_insert(func_table, f);
    }

    TEST_ASSERT_EQUAL_size_t(FUNC_TABLE_INIT_SIZE * 2, func_table->funcs_cap);
    TEST_ASSERT_EQUAL_size_t(65, func_table->funcs_size);
    TEST_ASSERT_NOT_NULL(func_table->funcs);

    TEST_ASSERT_NOT_NULL(&(func_table->funcs[0]));
    TEST_ASSERT_NOT_NULL(func_table->funcs[0].id);
    TEST_ASSERT_EQUAL_size_t(9129030, func_table->funcs[64].id->hash);
}

int main(void) {
    UNITY_BEGIN();
    RUN_TEST(test_func_table_init);
	RUN_TEST(test_func_table_clear);
    RUN_TEST(test_func_table_insert_empty);
    RUN_TEST(test_func_table_insert);
    RUN_TEST(test_func_table_insert_multiple);
    UNITY_END();
}
// todo
