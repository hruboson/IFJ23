#include "parser.h"
#include "scanner.h"
#include "symtable.h"
#include "unity.h"
#include "table.h"

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

	int ret = parse_statement(&in, &symtab, &st, NULL, NULL);
	TEST_ASSERT_EQUAL_INT(0, ret);

	TEST_ASSERT(st->type == ST_VAR);
	TEST_ASSERT(st->var.modifiable == true);
	TEST_ASSERT(st->var.allow_nil == false);
	TEST_ASSERT(st->var.data_type == VARTYPE_VOID);
	TEST_ASSERT(st->var.id != NULL);
	TEST_ASSERT_EQUAL_STRING("num", st->var.id->symbol.data);
	TEST_ASSERT(st->var.exp != NULL);
	TEST_ASSERT_EQUAL_INT(5, st->var.exp->int_);
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

	int ret = parse_statement(&in, &symtab, &st, NULL, NULL);
	TEST_ASSERT_EQUAL_INT(0, ret);

	TEST_ASSERT(st->type == ST_VAR);
	TEST_ASSERT(st->var.modifiable == false);
	TEST_ASSERT(st->var.allow_nil == false);
	TEST_ASSERT(st->var.data_type == VARTYPE_VOID);
	TEST_ASSERT(st->var.id != NULL);
	TEST_ASSERT_EQUAL_STRING("num", st->var.id->symbol.data);
	TEST_ASSERT(st->var.exp != NULL);
	TEST_ASSERT_EQUAL_INT(5, st->var.exp->int_);
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

	int ret = parse_statement(&in, &symtab, &st, NULL, NULL);
	TEST_ASSERT_EQUAL_INT(0, ret);

	TEST_ASSERT(st->type == ST_VAR);
	TEST_ASSERT(st->var.modifiable == true);
	TEST_ASSERT(st->var.allow_nil == false);
	TEST_ASSERT(st->var.data_type == VARTYPE_VOID);
	TEST_ASSERT(st->var.id != NULL);
	TEST_ASSERT_EQUAL_STRING("num", st->var.id->symbol.data);
	TEST_ASSERT(st->var.exp != NULL);
	TEST_ASSERT_EQUAL_DOUBLE(5.0, st->var.exp->double_);
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

	int ret = parse_statement(&in, &symtab, &st, NULL, NULL);
	TEST_ASSERT_EQUAL_INT(0, ret);

	TEST_ASSERT(st->type == ST_VAR);
	TEST_ASSERT(st->var.modifiable == false);
	TEST_ASSERT(st->var.allow_nil == false);
	TEST_ASSERT(st->var.data_type == VARTYPE_VOID);
	TEST_ASSERT(st->var.id != NULL);
	TEST_ASSERT_EQUAL_STRING("num", st->var.id->symbol.data);
	TEST_ASSERT(st->var.exp != NULL);
	TEST_ASSERT_EQUAL_DOUBLE(5.0, st->var.exp->double_);
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

	int ret = parse_statement(&in, &symtab, &st, NULL, NULL);
	TEST_ASSERT_EQUAL_INT(0, ret);

	TEST_ASSERT(st->type == ST_VAR);
	TEST_ASSERT(st->var.modifiable == true);
	TEST_ASSERT(st->var.allow_nil == false);
	TEST_ASSERT(st->var.data_type == VARTYPE_VOID);
	TEST_ASSERT(st->var.id != NULL);
	TEST_ASSERT_EQUAL_STRING("str", st->var.id->symbol.data);
	TEST_ASSERT(st->var.exp != NULL);
	TEST_ASSERT_EQUAL_STRING(st->var.exp->str_.data, "retezec");
}

void test_let_string_simple(void) {
    const char* data = "let str = \"retezec\"\n";
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

	int ret = parse_statement(&in, &symtab, &st, NULL, NULL);
	TEST_ASSERT_EQUAL_INT(0, ret);

	TEST_ASSERT(st->type == ST_VAR);
	TEST_ASSERT(st->var.modifiable == false);
	TEST_ASSERT(st->var.allow_nil == false);
	TEST_ASSERT(st->var.data_type == VARTYPE_VOID);
	TEST_ASSERT(st->var.id != NULL);
	TEST_ASSERT_EQUAL_STRING("str", st->var.id->symbol.data);
	TEST_ASSERT(st->var.exp != NULL);
	TEST_ASSERT_EQUAL_STRING(st->var.exp->str_.data, "retezec");
}

void test_var_int_simple_explicit_type(void) {
    const char* data = "var num : Int = 5\n";
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

	int ret = parse_statement(&in, &symtab, &st, NULL, NULL);
	TEST_ASSERT_EQUAL_INT(0, ret);

	TEST_ASSERT(st->type == ST_VAR);
	TEST_ASSERT(st->var.modifiable == true);
	TEST_ASSERT(st->var.allow_nil == false);
	TEST_ASSERT(st->var.data_type == VARTYPE_INT);
	TEST_ASSERT(st->var.id != NULL);
	TEST_ASSERT_EQUAL_STRING("num", st->var.id->symbol.data);
	TEST_ASSERT(st->var.exp != NULL);
	TEST_ASSERT_EQUAL_INT(5, st->var.exp->int_);
}

void test_let_int_simple_explicit_type(void) {
    const char* data = "let num : Int = 5\n";
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

	int ret = parse_statement(&in, &symtab, &st, NULL, NULL);
	TEST_ASSERT_EQUAL_INT(0, ret);

	TEST_ASSERT(st->type == ST_VAR);
	TEST_ASSERT(st->var.modifiable == false);
	TEST_ASSERT(st->var.allow_nil == false);
	TEST_ASSERT(st->var.data_type == VARTYPE_INT);
	TEST_ASSERT(st->var.id != NULL);
	TEST_ASSERT_EQUAL_STRING("num", st->var.id->symbol.data);
	TEST_ASSERT(st->var.exp != NULL);
	TEST_ASSERT_EQUAL_INT(5, st->var.exp->int_);
}

void test_var_double_simple_explicit_type(void) {
    const char* data = "var num : Double = 5.0\n";
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

	int ret = parse_statement(&in, &symtab, &st, NULL, NULL);
	TEST_ASSERT_EQUAL_INT(0, ret);

	TEST_ASSERT(st->type == ST_VAR);
	TEST_ASSERT(st->var.modifiable == true);
	TEST_ASSERT(st->var.allow_nil == false);
	TEST_ASSERT(st->var.data_type == VARTYPE_DOUBLE);
	TEST_ASSERT(st->var.id != NULL);
	TEST_ASSERT_EQUAL_STRING("num", st->var.id->symbol.data);
	TEST_ASSERT(st->var.exp != NULL);
	TEST_ASSERT_EQUAL_DOUBLE(5.0, st->var.exp->double_);
}

void test_let_double_simple_explicit_type(void) {
    const char* data = "let num : Double = 5.0\n";
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

	int ret = parse_statement(&in, &symtab, &st, NULL, NULL);
	TEST_ASSERT_EQUAL_INT(0, ret);

	TEST_ASSERT(st->type == ST_VAR);
	TEST_ASSERT(st->var.modifiable == false);
	TEST_ASSERT(st->var.allow_nil == false);
	TEST_ASSERT(st->var.data_type == VARTYPE_DOUBLE);
	TEST_ASSERT(st->var.id != NULL);
	TEST_ASSERT_EQUAL_STRING("num", st->var.id->symbol.data);
	TEST_ASSERT(st->var.exp != NULL);
	TEST_ASSERT_EQUAL_DOUBLE(5.0, st->var.exp->double_);
}

void test_var_string_simple_explicit_type(void) {
    const char* data = "var str : String = \"retezec\"\n";
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

	int ret = parse_statement(&in, &symtab, &st, NULL, NULL);
	TEST_ASSERT_EQUAL_INT(0, ret);

	TEST_ASSERT(st->type == ST_VAR);
	TEST_ASSERT(st->var.modifiable == true);
	TEST_ASSERT(st->var.allow_nil == false);
	TEST_ASSERT(st->var.data_type == VARTYPE_STRING);
	TEST_ASSERT(st->var.id != NULL);
	TEST_ASSERT_EQUAL_STRING("str", st->var.id->symbol.data);
	TEST_ASSERT(st->var.exp != NULL);
	TEST_ASSERT_EQUAL_STRING("retezec", st->var.exp->str_.data);
}

void test_let_string_simple_explicit_type(void) {
    const char* data = "let str : String = \"retezec\"\n";
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

	int ret = parse_statement(&in, &symtab, &st, NULL, NULL);
	TEST_ASSERT_EQUAL_INT(0, ret);

	TEST_ASSERT(st->type == ST_VAR);
	TEST_ASSERT(st->var.modifiable == false);
	TEST_ASSERT(st->var.allow_nil == false);
	TEST_ASSERT(st->var.data_type == VARTYPE_STRING);
	TEST_ASSERT(st->var.id != NULL);
	TEST_ASSERT_EQUAL_STRING("str", st->var.id->symbol.data);
	TEST_ASSERT(st->var.exp != NULL);
	TEST_ASSERT_EQUAL_STRING("retezec", st->var.exp->str_.data);
}

void test_var_int_simple_nil_allowed(void) {
    const char* data = "var num : Int? = 5\n";
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

	int ret = parse_statement(&in, &symtab, &st, NULL, NULL);
	TEST_ASSERT_EQUAL_INT(0, ret);

	TEST_ASSERT(st->type == ST_VAR);
	TEST_ASSERT(st->var.modifiable == true);
	TEST_ASSERT(st->var.allow_nil == true);
	TEST_ASSERT(st->var.data_type == VARTYPE_INT);
	TEST_ASSERT(st->var.id != NULL);
	TEST_ASSERT_EQUAL_STRING(st->var.id->symbol.data, "num");
	TEST_ASSERT(st->var.exp != NULL);
	TEST_ASSERT_EQUAL_INT(5, st->var.exp->int_);
}

void test_let_int_simple_nil_type(void) {
    const char* data = "let num : Int? = 5\n";
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

	int ret = parse_statement(&in, &symtab, &st, NULL, NULL);
	TEST_ASSERT_EQUAL_INT(0, ret);

	TEST_ASSERT(st->type == ST_VAR);
	TEST_ASSERT(st->var.modifiable == false);
	TEST_ASSERT(st->var.allow_nil == true);
	TEST_ASSERT(st->var.data_type == VARTYPE_INT);
	TEST_ASSERT(st->var.id != NULL);
	TEST_ASSERT_EQUAL_STRING("num", st->var.id->symbol.data);
	TEST_ASSERT(st->var.exp != NULL);
	TEST_ASSERT_EQUAL_INT(5, st->var.exp->int_);
}

void test_var_double_simple_nil_allowed(void) {
    const char* data = "var num : Double? = 5.0\n";
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

	int ret = parse_statement(&in, &symtab, &st, NULL, NULL);
	TEST_ASSERT_EQUAL_INT(0, ret);

	TEST_ASSERT(st->type == ST_VAR);
	TEST_ASSERT(st->var.modifiable == true);
	TEST_ASSERT(st->var.allow_nil == true);
	TEST_ASSERT(st->var.data_type == VARTYPE_DOUBLE);
	TEST_ASSERT(st->var.id != NULL);
	TEST_ASSERT_EQUAL_STRING("num", st->var.id->symbol.data);
	TEST_ASSERT(st->var.exp != NULL);
	TEST_ASSERT_EQUAL_DOUBLE(5.0, st->var.exp->double_);
}

void test_let_double_simple_nil_allowed(void) {
    const char* data = "let num : Double? = 5.0\n";
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

	int ret = parse_statement(&in, &symtab, &st, NULL, NULL);
	TEST_ASSERT_EQUAL_INT(0, ret);

	TEST_ASSERT(st->type == ST_VAR);
	TEST_ASSERT(st->var.modifiable == false);
	TEST_ASSERT(st->var.allow_nil == true);
	TEST_ASSERT(st->var.data_type == VARTYPE_DOUBLE);
	TEST_ASSERT(st->var.id != NULL);
	TEST_ASSERT_EQUAL_STRING("num", st->var.id->symbol.data);
	TEST_ASSERT(st->var.exp != NULL);
	TEST_ASSERT_EQUAL_DOUBLE(5.0, st->var.exp->double_);
}

void test_var_string_simple_nil_allowed(void) {
    const char* data = "var str : String? = \"retezec\"\n";
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
	TEST_ASSERT(st->var.allow_nil == true);
	TEST_ASSERT(st->var.data_type == VARTYPE_STRING);
	TEST_ASSERT(st->var.id != NULL);
	TEST_ASSERT_EQUAL_STRING("str", st->var.id->symbol.data);
	TEST_ASSERT(st->var.exp != NULL);
	TEST_ASSERT_EQUAL_STRING("retezec", st->var.exp->str_.data);
}

void test_let_string_simple_nil_allowed(void) {
    const char* data = "let str : String? = \"retezec\"\n";
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

	int ret = parse_statement(&in, &symtab, &st, NULL, NULL);
	TEST_ASSERT_EQUAL_INT(0, ret);

	TEST_ASSERT(st->type == ST_VAR);
	TEST_ASSERT(st->var.modifiable == false);
	TEST_ASSERT(st->var.allow_nil == true);
	TEST_ASSERT(st->var.data_type == VARTYPE_STRING);
	TEST_ASSERT(st->var.id != NULL);
	TEST_ASSERT_EQUAL_STRING("str", st->var.id->symbol.data);
	TEST_ASSERT(st->var.exp != NULL);
	TEST_ASSERT_EQUAL_STRING("retezec", st->var.exp->str_.data);
}

//TODO: test na dve promenne zasebou a zkontrolovat:
// jestli se ulozi druha do statement->next prvni a druhy statement->next bude null

void test_two_variables(void) {
	const char* data = "var num1 : Int = 5\n var num2 : Double = 5.0\n";
	Input in = {
		.type = INT_STRING,
		.string = {
			.s = data,
			.i = 0, .store = 0,
		},
	};

	SymbolTable symtab;

	init_symboltable( &symtab );

	AST ast;
	init_ast(&ast);

	int ret = parse(&in, &ast);

	Statement *st = ast.statement;

	TEST_ASSERT(st != NULL);
	//FIX: tady je taky ret = 2
	TEST_ASSERT_EQUAL_INT(0, ret);


	printf("LINE: %d\n", __LINE__);

	TEST_ASSERT(st->type == ST_VAR);
	TEST_ASSERT(st->var.modifiable == true);
	TEST_ASSERT(st->var.allow_nil == false);
	TEST_ASSERT(st->var.data_type == VARTYPE_INT);
	TEST_ASSERT(st->var.id != NULL);
	TEST_ASSERT_EQUAL_STRING(st->var.id->symbol.data, "num1");
	TEST_ASSERT(st->var.exp != NULL);
	TEST_ASSERT_EQUAL_INT(5, st->var.exp->int_);

	st = st->next;

	printf("LINE: %d\n", __LINE__);

	TEST_ASSERT(st->type == ST_VAR);
	TEST_ASSERT(st->var.modifiable == true);
	TEST_ASSERT(st->var.allow_nil == false);
	TEST_ASSERT(st->var.data_type == VARTYPE_DOUBLE);
	TEST_ASSERT(st->var.id != NULL);
	TEST_ASSERT_EQUAL_STRING(st->var.id->symbol.data, "num2");
	TEST_ASSERT(st->var.exp != NULL);
	TEST_ASSERT_EQUAL_DOUBLE(5.0, st->var.exp->double_);

}

//==========WHILE==========
void test_while_simple_int_exp(void) {
	const char* data = "while 1 { }";
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

	VarTableStack var_table_stack;
	init_vartable_stack(&var_table_stack);

	int ret = parse_statement(&in, &symtab, &st, &var_table_stack, NULL);

	TEST_ASSERT_EQUAL_INT(0, ret);

	//TODO: test na st->while_.body
	TEST_ASSERT(st->while_.exp->type == ET_INT);
	TEST_ASSERT_EQUAL_INT(1, st->while_.exp->int_);

}


int main(void) {
    UNITY_BEGIN();

    RUN_TEST(test_var_int_simple);
    RUN_TEST(test_let_int_simple);
    RUN_TEST(test_var_double_simple);
    RUN_TEST(test_let_double_simple);
    RUN_TEST(test_var_string_simple);
    RUN_TEST(test_let_string_simple);

    RUN_TEST(test_var_int_simple_explicit_type);
    RUN_TEST(test_let_int_simple_explicit_type);
    RUN_TEST(test_var_double_simple_explicit_type);
    RUN_TEST(test_let_double_simple_explicit_type);
    RUN_TEST(test_var_string_simple_explicit_type);
    RUN_TEST(test_let_string_simple_explicit_type);

    RUN_TEST(test_var_double_simple_nil_allowed);
    RUN_TEST(test_let_double_simple_nil_allowed);
    RUN_TEST(test_var_string_simple_nil_allowed);
    RUN_TEST(test_let_string_simple_nil_allowed);
	printf("LINE: %d\n", __LINE__);
	RUN_TEST(test_two_variables);
	printf("LINE: %d\n", __LINE__);
	RUN_TEST(test_while_simple_int_exp);

    return UNITY_END();
}
