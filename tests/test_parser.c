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

void test_parser(void) {
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
	TEST_ASSERT(st->var.allow_nil == false);
	TEST_ASSERT(st->var.data_type == KEYWORD_INT);
	TEST_ASSERT_EQUAL_STRING(st->var.id->symbol.data, "num");
	TEST_ASSERT_EQUAL_INT(st->var.exp->int_, 5);
}

int main(void) {
    UNITY_BEGIN();
    RUN_TEST(test_parser);
    return UNITY_END();
}