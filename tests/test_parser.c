#include "parser.h"
#include "scanner.h"
#include "symtable.h"
#include "unity.h"

//! always needs to be defined
void setUp() {
    // setup file
}
void tearDown() {
    // close file
}

void test_var_int_simple(void) {
    const char* data = "var num = 5\n";
	Input in = {
		.type = INT_STRING,
		.string = {
			.s = data,
			.i = 0, .store = 0,
		},
	};

	SymbolTable symtab;
	init_symboltable( &symtab );

	Statement *st;

	parse_statement(&in, &symtab, &st, NULL, NULL);

	TEST_ASSERT(st->type == ST_VAR);
	TEST_ASSERT(st->var.modifiable == true);
	TEST_ASSERT(st->var.allow_nil == false);
	TEST_ASSERT(st->var.data_type == KEYWORD_INT);
	TEST_ASSERT_EQUAL_STRING(st->var.id->symbol.data, "num");
	TEST_ASSERT_EQUAL_INT(st->var.exp->int_, 5);
}

void test_let_int_simple(void) {
    const char* data = "let num = 5\n";
	Input in = {
		.type = INT_STRING,
		.string = {
			.s = data,
			.i = 0, .store = 0,
		},
	};

	SymbolTable symtab;
	init_symboltable( &symtab );

	Statement *st;

	parse_statement(&in, &symtab, &st, NULL, NULL);

	TEST_ASSERT(st->type == ST_VAR);
	TEST_ASSERT(st->var.modifiable == false);
	TEST_ASSERT(st->var.allow_nil == false);
	TEST_ASSERT(st->var.data_type == KEYWORD_INT);
	TEST_ASSERT_EQUAL_STRING(st->var.id->symbol.data, "num");
	TEST_ASSERT_EQUAL_INT(st->var.exp->int_, 5);
}

void test_var_double_simple(void) {
    const char* data = "var num = 5.0\n";
	Input in = {
		.type = INT_STRING,
		.string = {
			.s = data,
			.i = 0, .store = 0,
		},
	};

	SymbolTable symtab;
	init_symboltable( &symtab );

	Statement *st;

	parse_statement(&in, &symtab, &st, NULL, NULL);

	TEST_ASSERT(st->type == ST_VAR);
	TEST_ASSERT(st->var.modifiable == true);
	TEST_ASSERT(st->var.allow_nil == false);
	TEST_ASSERT(st->var.data_type == KEYWORD_DOUBLE);
	TEST_ASSERT_EQUAL_STRING(st->var.id->symbol.data, "num");
	TEST_ASSERT_EQUAL_INT(st->var.exp->double_, 5.0);
}

void test_let_double_simple(void) {
    const char* data = "let num = 5.0\n";
	Input in = {
		.type = INT_STRING,
		.string = {
			.s = data,
			.i = 0, .store = 0,
		},
	};

	SymbolTable symtab;
	init_symboltable( &symtab );

	Statement *st;

	parse_statement(&in, &symtab, &st, NULL, NULL);

	TEST_ASSERT(st->type == ST_VAR);
	TEST_ASSERT(st->var.modifiable == false);
	TEST_ASSERT(st->var.allow_nil == false);
	TEST_ASSERT(st->var.data_type == KEYWORD_DOUBLE);
	TEST_ASSERT_EQUAL_STRING(st->var.id->symbol.data, "num");
	TEST_ASSERT_EQUAL_INT(st->var.exp->double_, 5.0);
}

void test_var_string_simple(void) {
    const char* data = "var str = \"retezec\"\n";
	Input in = {
		.type = INT_STRING,
		.string = {
			.s = data,
			.i = 0, .store = 0,
		},
	};

	SymbolTable symtab;
	init_symboltable( &symtab );

	Statement *st;

	parse_statement(&in, &symtab, &st, NULL, NULL);

	TEST_ASSERT(st->type == ST_VAR);
	TEST_ASSERT(st->var.modifiable == true);
	TEST_ASSERT(st->var.allow_nil == false);
	TEST_ASSERT(st->var.data_type == KEYWORD_STRING);
	TEST_ASSERT_EQUAL_STRING(st->var.id->symbol.data, "str");
	TEST_ASSERT_EQUAL_STRING(st->var.exp->str_.data, "retezec");
}

int main(void) {
    UNITY_BEGIN();
    RUN_TEST(test_var_int_simple);
    RUN_TEST(test_let_int_simple);
    return UNITY_END();
}