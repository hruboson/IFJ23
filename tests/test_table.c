#include "../include/table.h"
#include "unity.h"

FuncTable func_table_;
FuncTable* func_table = &func_table_;

VarTable var_table_;
VarTable* var_table = &var_table_;

//! always needs to be defined
void setUp() {
    init_func_table(func_table);
    init_var_table(var_table);
}
void tearDown() {
    clear_func_table(func_table);
    clear_var_table(var_table);
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

void test_func_table_get() {
    Function f;
    SymbolRecord r;
    r.hash = 12030098;
    f.id = &r;

    func_table_insert(func_table, f);

    Function* f_get = func_table_get(func_table, &r);
    TEST_ASSERT_NOT_NULL(f_get);
    TEST_ASSERT_EQUAL_size_t(r.hash, f_get->id->hash);
}

void test_func_table_get_nonexisting() {
    Function f;
    SymbolRecord r;
    r.hash = 12030098;
    f.id = &r;

    SymbolRecord r_not_inserted;
    r_not_inserted.hash = 5613123;

    func_table_insert(func_table, f);

    Function* f_get = func_table_get(func_table, &r_not_inserted);
    TEST_ASSERT_NULL(f_get);
}

void test_var_table_init() {
    VarTable init_table_;
    VarTable* init_table = &init_table_;
    init_var_table(init_table);
    TEST_ASSERT_NULL(init_table->vars);
    TEST_ASSERT_EQUAL_size_t(0, init_table->vars_cap);
    TEST_ASSERT_EQUAL_size_t(0, init_table->vars_size);
}

void test_var_table_clear() {
    VarTable clear_table_;
    VarTable* clear_table = &clear_table_;
    init_var_table(clear_table);

    Variable v;
    SymbolRecord r;
    r.hash = 9129030;
    v.id = &r;

    for (size_t i = 0; i < 65; i++) {
        var_table_insert(clear_table, v);
    }

    clear_var_table(clear_table);

    TEST_ASSERT_NULL(clear_table->vars);
    TEST_ASSERT_EQUAL_size_t(0, clear_table->vars_cap);
    TEST_ASSERT_EQUAL_size_t(0, clear_table->vars_size);
}

void test_var_table_insert_empty() {
    Variable v;
    SymbolRecord r;
    v.id = &r;
    var_table_insert(var_table, v);
    TEST_ASSERT_EQUAL_size_t(VAR_TABLE_INIT_SIZE, var_table->vars_cap);
    TEST_ASSERT_EQUAL_size_t(1, var_table->vars_size);
    TEST_ASSERT_NOT_NULL(var_table->vars);

    TEST_ASSERT_NOT_NULL(&(var_table->vars[0]));
    TEST_ASSERT_NOT_NULL(var_table->vars[0].id);
}

void test_var_table_insert() {
    Variable v;
    SymbolRecord r;
    r.hash = 712390;
    v.id = &r;
    var_table_insert(var_table, v);
    TEST_ASSERT_EQUAL_size_t(VAR_TABLE_INIT_SIZE, var_table->vars_cap);
    TEST_ASSERT_EQUAL_size_t(1, var_table->vars_size);
    TEST_ASSERT_NOT_NULL(var_table->vars);

    TEST_ASSERT_NOT_NULL(&(var_table->vars[0]));
    TEST_ASSERT_NOT_NULL(var_table->vars[0].id);
    TEST_ASSERT_EQUAL_size_t(712390, var_table->vars[0].id->hash);
}

void test_var_table_insert_multiple() {
    Variable v;
    SymbolRecord r;
    r.hash = 2903790;
    v.id = &r;

    var_table_insert(var_table, v);
    var_table_insert(var_table, v);
    var_table_insert(var_table, v);
    var_table_insert(var_table, v);

    TEST_ASSERT_EQUAL_size_t(VAR_TABLE_INIT_SIZE, var_table->vars_cap);
    TEST_ASSERT_EQUAL_size_t(4, var_table->vars_size);
    TEST_ASSERT_NOT_NULL(var_table->vars);

    TEST_ASSERT_NOT_NULL(&(var_table->vars[0]));
    TEST_ASSERT_NOT_NULL(var_table->vars[0].id);
    TEST_ASSERT_EQUAL_size_t(2903790, var_table->vars[3].id->hash);
}

void test_var_table_insert_over_init_size() {
    Variable v;
    SymbolRecord r;
    r.hash = 4120390;
    v.id = &r;

    for (size_t i = 0; i < 65; i++) {
        var_table_insert(var_table, v);
    }

    TEST_ASSERT_EQUAL_size_t(VAR_TABLE_INIT_SIZE * 2, var_table->vars_cap);
    TEST_ASSERT_EQUAL_size_t(65, var_table->vars_size);
    TEST_ASSERT_NOT_NULL(var_table->vars);

    TEST_ASSERT_NOT_NULL(&(var_table->vars[0]));
    TEST_ASSERT_NOT_NULL(var_table->vars[0].id);
    TEST_ASSERT_EQUAL_size_t(4120390, var_table->vars[64].id->hash);
}

void test_var_table_get() {
    Variable v;
    SymbolRecord r;
    r.hash = 678178;
    v.id = &r;

    var_table_insert(var_table, v);

    Variable* v_get = var_table_get(var_table, &r);
    TEST_ASSERT_NOT_NULL(v_get);
    TEST_ASSERT_EQUAL_size_t(r.hash, v_get->id->hash);
}

void test_var_table_get_nonexisting() {
    Variable v;
    SymbolRecord r;
    r.hash = 7712389;
    v.id = &r;

    SymbolRecord r_not_inserted;
    r_not_inserted.hash = 1812589;

    var_table_insert(var_table, v);

    Variable* v_get_exists = var_table_get(var_table, &r);
    TEST_ASSERT_NOT_NULL(v_get_exists);

    Variable* v_get = var_table_get(var_table, &r_not_inserted);
    TEST_ASSERT_NULL(v_get);
}

int main(void) {
    UNITY_BEGIN();

    // FuncTable
    RUN_TEST(test_func_table_init);
    RUN_TEST(test_func_table_clear);
    RUN_TEST(test_func_table_insert_empty);
    RUN_TEST(test_func_table_insert);
    RUN_TEST(test_func_table_insert_multiple);
    RUN_TEST(test_func_table_get);
    RUN_TEST(test_func_table_get_nonexisting);

    // VarTable
    RUN_TEST(test_var_table_init);
    RUN_TEST(test_var_table_clear);
    RUN_TEST(test_var_table_insert_empty);
    RUN_TEST(test_var_table_insert);
    RUN_TEST(test_var_table_insert_multiple);
    RUN_TEST(test_var_table_get);
    RUN_TEST(test_var_table_get_nonexisting);

    UNITY_END();
}
// todo
