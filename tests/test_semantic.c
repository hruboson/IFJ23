#include "parser.h"
#include "scanner.h"
#include "symtable.h"
#include "unity.h"
#include "table.h"
#include "semantic.h"

#define arraysize(x) (sizeof(x)/sizeof(x[0]))

//! always needs to be defined
void setUp() {
    // setup file
}
void tearDown() {
    // close file
}



void test_type_match(void) {
	struct {
		DataType L, R;
		int ret;
	} tests[] = {
		{ {VARTYPE_VOID, false}, {VARTYPE_VOID, false}, 7},
		{ {VARTYPE_INT, false}, {VARTYPE_INT, false}, 0},
		{ {VARTYPE_INT, true}, {VARTYPE_INT, false}, 0},
		{ {VARTYPE_INT, true}, {VARTYPE_VOID, true}, 0},
		{ {VARTYPE_INT, false}, {VARTYPE_STRING, false}, 7},
		{ {VARTYPE_INT, false}, {VARTYPE_VOID, true}, 7},
		{ {VARTYPE_VOID, false}, {VARTYPE_INT, false}, 0},
		{ {VARTYPE_INT, true}, {VARTYPE_STRING, false}, 7},
		{ {VARTYPE_VOID, false}, {VARTYPE_INT, true}, 0},
		{ {VARTYPE_INT, false}, {VARTYPE_INT, true}, 7},
	};

	for (size_t i = 0; i < arraysize(tests); i++) {
		int ret = semantic_type_match(&tests[i].L, &tests[i].R);
		if (ret != tests[i].ret) {
			printf("NOT EQUAL: %lu\n", i);
		}
		TEST_ASSERT_EQUAL_INT(ret, tests[i].ret);
	}

	
}

void test_type_match_2(void) {
	struct {
		const char* str;
		DataType dt;
		int ret;
	} tests[] = {
		{ "let a : Int = nil\n", {}, 7},
		{ "let a = nil\n", {}, 7},
		{ "let a : Int = 1\n", {VARTYPE_INT, false}, 0},
		{ "let a : Int? = 1\n", {VARTYPE_INT, true}, 0},
		{ "let a : Int? = nil\n", {VARTYPE_INT, true}, 0},
		{ "let a : Int = \"a\"\n", {}, 7},
		{ "let a : Int = nil\n", {}, 7},
		{ "let a = 1\n", {VARTYPE_INT, false}, 0},
		{ "let a : Int? = \"a\"\n", {}, 7}
	};

	for (size_t i = 0; i < arraysize(tests); i++) {
		
		Input in = {
			.type = INT_STRING,
			.string = {
				.s = tests[i].str,
				.i = 0, .store = 0,
			},
		};

		AST ast;
		init_ast(&ast);

		int ret = parse(&in, &ast);
		if (ret != tests[i].ret) {
			printf("NOT EQUAL: %lu\n", i);
		}
		TEST_ASSERT_EQUAL_INT(tests[i].ret, ret);
		if (ret != 0) continue;

		DataType data_type = ast.statement->var.data_type;

		if (data_type.type != tests[i].dt.type) {
			printf("NOT EQUAL TYPE: %lu\n", i);
		}
		if (data_type.nil_allowed != tests[i].dt.nil_allowed) {
			printf("NOT EQUAL NIL ALLOWED: %lu\n", i);
		}
		TEST_ASSERT_EQUAL_INT(tests[i].dt.type, data_type.type);
		TEST_ASSERT_EQUAL_INT(tests[i].dt.nil_allowed, data_type.nil_allowed);
	}
}

void test_assign(void) {
	struct {
		const char* str;
		DataType dt;
		int ret;
	} tests[] = {
		{ "var a : Int = 1\n a = 3.0", {}, 7},
		{ "var a : Double = 1.0\n a = 3.0\n", {VARTYPE_DOUBLE, false}, 0},
		{ "var a : Int? = 1\n a = nil\n", {VARTYPE_INT, true}, 0},
		{ "var a : Int = 1.0\n a = nil\n", {}, 7},

		{ "let a : Int = 1\n a = 10\n", {}, 9}, // asign to let
		{ "let a : Int? = 1\n a = 10\n", {}, 9}, // asign to let
		{ "let a : Double? = 1.0\n a = 10.0\n", {}, 9}, // asign to let
	};

	for (size_t i = 0; i < arraysize(tests); i++) {
		
		Input in = {
			.type = INT_STRING,
			.string = {
				.s = tests[i].str,
				.i = 0, .store = 0,
			},
		};

		AST ast;
		init_ast(&ast);

		int ret = parse(&in, &ast);
		if (ret != tests[i].ret) {
			printf("NOT EQUAL: %lu\n", i);
		}
		TEST_ASSERT_EQUAL_INT(tests[i].ret, ret);
		if (ret != 0) continue;

		DataType data_type = ast.statement->var.data_type;

		if (data_type.type != tests[i].dt.type) {
			printf("NOT EQUAL TYPE: %lu\n", i);
		}
		if (data_type.nil_allowed != tests[i].dt.nil_allowed) {
			printf("NOT EQUAL NIL ALLOWED: %lu\n", i);
		}
		TEST_ASSERT_EQUAL_INT(tests[i].dt.type, data_type.type);
		TEST_ASSERT_EQUAL_INT(tests[i].dt.nil_allowed, data_type.nil_allowed);
	}
}

void test_assign_exp(void) {
	struct {
		const char* str;
		DataType dt;
		int ret;
	} tests[] = {
		{ "var a : Int = 1 + 1\n", {VARTYPE_INT, false}, 0},
		{ "var a : Int = 1 - 1\n", {VARTYPE_INT, false}, 0},

		{ "var a : Int = 1 + 1\n", {VARTYPE_INT, false}, 0},
		{ "var a : Int = 1 - 1\n", {VARTYPE_INT, false}, 0},
		{ "var a : Int = 1 / 1\n", {VARTYPE_INT, false}, 0},
		{ "var a : Int = 1 * 1\n", {VARTYPE_INT, false}, 0},

		{ "var a : Int? = 1 + 1\n", {VARTYPE_INT, true}, 0},
		{ "var a : Int? = 1 + 1\n", {VARTYPE_INT, true}, 0},
		{ "var a : Int? = 1 + 1\n", {VARTYPE_INT, true}, 0},
		{ "var a : Int? = 1 + 1\n", {VARTYPE_INT, true}, 0},

		{ "var a : Double = 1.0 + 1.0\n", {VARTYPE_DOUBLE, false}, 0},
		{ "var a : Double = 1.0 - 1.0\n", {VARTYPE_DOUBLE, false}, 0},
		{ "var a : Double = 1.0 * 1.0\n", {VARTYPE_DOUBLE, false}, 0},
		{ "var a : Double = 1.0 / 1.0\n", {VARTYPE_DOUBLE, false}, 0},

		{ "var a : Double? = 1.0 + 1.0\n", {VARTYPE_DOUBLE, true}, 0},
		{ "var a : Double? = 1.0 - 1.0\n", {VARTYPE_DOUBLE, true}, 0},
		{ "var a : Double? = 1.0 * 1.0\n", {VARTYPE_DOUBLE, true}, 0},
		{ "var a : Double? = 1.0 / 1.0\n", {VARTYPE_DOUBLE, true}, 0},

		{ "var a : String = \"ah\" + \"oj\"\n", {VARTYPE_STRING, false}, 0},
		{ "var a : String = \"ah\" - \"oj\"\n", {VARTYPE_STRING, false}, 7},
		{ "var a : String = \"ah\" * \"oj\"\n", {VARTYPE_STRING, false}, 7},
		{ "var a : String = \"ah\" / \"oj\"\n", {VARTYPE_STRING, false}, 7},

		// zavorky
		{ "var a : Int = (1 + 1)\n", {VARTYPE_INT, false}, 0},
		{ "var a : Int = (1 - 1)\n", {VARTYPE_INT, false}, 0},
		{ "var a : Int = (1 * 1)\n", {VARTYPE_INT, false}, 0},
		{ "var a : Int = (1 / 1)\n", {VARTYPE_INT, false}, 0},

		{ "var a : Int = 1 + (1 - 1)\n", {VARTYPE_INT, false}, 0},
		{ "var a : Int = 1 - (1 + 1)\n", {VARTYPE_INT, false}, 0},
		{ "var a : Int = 1 / (1 + 1)\n", {VARTYPE_INT, false}, 0},
		{ "var a : Int = 1 * (1 + 1)\n", {VARTYPE_INT, false}, 0},
		{ "var a : Int = 1 - (1 / 1)\n", {VARTYPE_INT, false}, 0},
		{ "var a : Int = 1 * (1 / 1)\n", {VARTYPE_INT, false}, 0},

		// priority
		{ "var a : Int = 1 + 1 * 1\n", {VARTYPE_INT, false}, 0},
		{ "var a : Int = 1 * 1 + 1\n", {VARTYPE_INT, false}, 0},
		{ "var a : Int = 1 / 2 * 1\n", {VARTYPE_INT, false}, 0},
		{ "var a : Int = 1 * 4 / 2\n", {VARTYPE_INT, false}, 0},
		{ "var a : Int = 1 - 4 / 2\n", {VARTYPE_INT, false}, 0},
		{ "var a : Int = 1 - 4 / 2\n", {VARTYPE_INT, false}, 0},

		//{ "var a : Double = 1.0\n a = 3.0\n", {VARTYPE_DOUBLE, false}, 0},
	};

	for (size_t i = 0; i < arraysize(tests); i++) {
		
		Input in = {
			.type = INT_STRING,
			.string = {
				.s = tests[i].str,
				.i = 0, .store = 0,
			},
		};

		AST ast;
		init_ast(&ast);

		int ret = parse(&in, &ast);
		if (ret != tests[i].ret) {
			printf("NOT EQUAL: %lu\n", i);
		}
		TEST_ASSERT_EQUAL_INT(tests[i].ret, ret);
		if (ret != 0) continue;

		DataType data_type = ast.statement->var.data_type;

		if (data_type.type != tests[i].dt.type) {
			printf("NOT EQUAL TYPE: %lu\n", i);
		}
		if (data_type.nil_allowed != tests[i].dt.nil_allowed) {
			printf("NOT EQUAL NIL ALLOWED: %lu\n", i);
		}
		TEST_ASSERT_EQUAL_INT(tests[i].dt.type, data_type.type);
		TEST_ASSERT_EQUAL_INT(tests[i].dt.nil_allowed, data_type.nil_allowed);
	}
}

void test_write(void) {
	struct {
		const char* str;
		int ret;
	} tests[] = {
		{ "write()\n", 4},
		{ "write(1)\n", 0},
		{ "write(1, 2 + 2)\n", 0},
		{ "write(nil)\n", 0},
		{ "write(\"ahoj\")\n", 0},
		{ "write(1, 2, 3, 4.0)\n", 0},
	};

	for (size_t i = 0; i < arraysize(tests); i++) {
		
		Input in = {
			.type = INT_STRING,
			.string = {
				.s = tests[i].str,
				.i = 0, .store = 0,
			},
		};

		AST ast;
		init_ast(&ast);

		int ret = parse(&in, &ast);
		if (ret != tests[i].ret) {
			printf("NOT EQUAL: %lu\n", i);
		}
		TEST_ASSERT_EQUAL_INT(tests[i].ret, ret);
		if (ret != 0) continue;

	}
}

void test_function(void) {
	struct {
		const char* str;
		int ret;
	} tests[] = {
		{ "func a() -> Int { return 1\n } \n var b : Int = a()\n", 0},
	};

	for (size_t i = 0; i < arraysize(tests); i++) {
		
		Input in = {
			.type = INT_STRING,
			.string = {
				.s = tests[i].str,
				.i = 0, .store = 0,
			},
		};

		AST ast;
		init_ast(&ast);

		int ret = parse(&in, &ast);
		if (ret != tests[i].ret) {
			printf("NOT EQUAL: %lu\n", i);
		}
		TEST_ASSERT_EQUAL_INT(tests[i].ret, ret);
		if (ret != 0) continue;

	}
}

void test_multiple(void) {
	//TODO: kontrolovat vysledky obou statementu 		
	// nil test
	// { "var a : Int? = 5\n var b : Int = a ?? 5\n", {VARTYPE_INT, false}, 0},
}


void test_return(void) {
	struct {
		const char* str;
		int ret;
	} tests[] = {
		{ "func a() -> Int { return 1\n }", 0},
		{ "func a() -> Int { return \n }", 4},
		{ "func a() -> Double { return 1\n }", 4},
		{ "func a() -> Double { return \"str\"\n }", 4},
		{ "func a() -> Double? { return nil\n }", 0},
		{ "func a() -> Int? { return nil\n }", 0},
		{ "func a() -> String? { return nil\n }", 0},
		{ "func a() -> Int? { return 1\n }", 0},
		{ "func a() -> Double? { return 1.0\n }", 0},
		{ "func a() -> String? { return \"str\"\n }", 0},
		{ "func a() -> Int? { return \n }", 4},
		{ "func a() -> Double? { return \n }", 4},
		{ "func a() -> String? { return \n }", 4},
		{ "func a(_ b : Int) -> Int { return 1\n }", 0},
		{ "func a(_ b : Int) -> Int { return b\n }", 0},
		{ "func a(_ b : Int?) -> Int? { return b\n }", 0},
		//TODO: { "func a(_ b : Int?) -> Int { return b\n }", 4},
	};

	for (size_t i = 0; i < arraysize(tests); i++) {
		
		Input in = {
			.type = INT_STRING,
			.string = {
				.s = tests[i].str,
				.i = 0, .store = 0,
			},
		};

		AST ast;
		init_ast(&ast);

		int ret = parse(&in, &ast);
		if (ret != tests[i].ret) {
			printf("NOT EQUAL: %lu\n", i);
		}

		TEST_ASSERT_EQUAL_INT(tests[i].ret, ret);
		if (ret != 0) continue;
	}
}

void test_while(void){
    struct {
        const char* str;
        int ret;
    } tests[] = {
        { "while a {}\n", 5}, //var not defined

        { "var a : Int = 5\n while a {}\n", 7},
        { "let a : Int = 5\n while a {}\n", 7},
        { "var a : Int? = 5\n while a {}\n", 7},
        { "let a : Int? = 5\n while a {}\n", 7},
		
        { "let a : Int = 1\n while a {}", 7},
		//TODO: následující test je správně, ale parser neumí bool
        //{ "let a : Int = 10\n while a == 10 {}\n", 0},
		//TODO: další testy 
    };

    for(size_t i = 0; i < arraysize(tests); i++){
        Input in = {
            .type = INT_STRING,
            .string = {
                .s = tests[i].str,
                .i = 0, .store = 0,
            },
        };

        AST ast;
        init_ast(&ast);

        int ret = parse(&in, &ast);
        if (ret != tests[i].ret) {
            printf("NOT EQUAL: %li\n", i);
        }
        TEST_ASSERT_EQUAL_INT(tests[i].ret, ret);
    }
}

void test_if(void){
    struct {
        const char* str;
        int ret;
    } tests[] = {
        { "let a : Int? = 5\n if let a {}\n", 0},
        { "let a : Int = 5\n if let a {}\n", 8},
		{ "var a : Int = 5\n if let a {}\n", 8},

        { "let a : Int? = 5\n if a {}\n", 7},
        { "let a : Int = 5\n if a {}\n", 7},
		{ "var a : Int = 5\n if a {}\n", 7},
    };

    for(size_t i = 0; i < arraysize(tests); i++){
        Input in = {
            .type = INT_STRING,
            .string = {
                .s = tests[i].str,
                .i = 0, .store = 0,
            },
        };

        AST ast;
        init_ast(&ast);

        int ret = parse(&in, &ast);
        if (ret != tests[i].ret) {
            printf("NOT EQUAL: %lu\n", i);
        }
        TEST_ASSERT_EQUAL_INT(tests[i].ret, ret);
    }
}

int main(void) {
	do_semantic_analysis = true;

    UNITY_BEGIN();

	RUN_TEST(test_type_match);
	RUN_TEST(test_type_match_2);
	RUN_TEST(test_assign);
	RUN_TEST(test_assign_exp);
	RUN_TEST(test_return);
	RUN_TEST(test_while);
	RUN_TEST(test_if);

	RUN_TEST(test_write);

	RUN_TEST(test_function);
    
	return UNITY_END();
}
