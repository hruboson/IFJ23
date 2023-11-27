#include "scanner.h"
#include "unity.h"

#define _XOPEN_SOURCE

#include <string.h>
#include <unistd.h>
#include <stdio.h>

#include <assert.h>

#define arraysize( x ) ( sizeof( x ) / sizeof( x[ 0 ] ) )

//! always needs to be defined
void setUp() {}
void tearDown() {}

void test_keywords(void) {
	const char* data = "Double else func if Int let nil return String var while";
	Input in = {
		.type = INT_STRING,
		.string = {
			.s = data,
			.i = 0, .store = 0,
		},
	};

	Keyword kws[] = {
		KEYWORD_DOUBLE, KEYWORD_ELSE, KEYWORD_FUNC,
		KEYWORD_IF, KEYWORD_INT, KEYWORD_LET,
		KEYWORD_NIL, KEYWORD_RETURN,
		KEYWORD_STRING, KEYWORD_VAR, KEYWORD_WHILE
	};
	size_t kws_i = 0;


	bool run = true;
	int ret;
	while ( run ) {
		Token token;
		ret = get_token( &in, NULL, &token );
		if ( ret != 0 ) {
			fprintf( stderr, "error: get_token %i\n", ret );
			break;
		}
		if ( token.type == TOKENTYPE_EOF )
			break;

		TEST_ASSERT_TRUE( token.type == TOKENTYPE_KEYWORD );
		TEST_ASSERT_TRUE( token.value.keyword == kws[ kws_i ] );
		kws_i++;
	}
}

void test_identifiers(void) {
	const char* data = "a b c _asd string_ asd";
	Input in = {
		.type = INT_STRING,
		.string = {
			.s = data,
			.i = 0, .store = 0,
		},
	};

	const char* ids[] = {
		"a", "b", "c", "_asd", "string_", "asd"
	};
	size_t id_i = 0;

	SymbolTable s;
	init_symboltable( &s );

	bool run = true;
	int ret;
	while ( run ) {
		Token token;
		ret = get_token( &in, &s, &token );
		if ( ret != 0 ) {
			fprintf( stderr, "error: get_token %i\n", ret );
			break;
		}
		if ( token.type == TOKENTYPE_EOF )
			break;

		TEST_ASSERT_TRUE( token.type == TOKENTYPE_ID );
		TEST_ASSERT_TRUE( strcmp( token.value.id->symbol.data, ids[ id_i ] ) == 0 );
		id_i++;
	}

	clear_symboltable( &s );
}

void test_numbers(void) {
	const char* data = "0 1 01 0.0 1.0 3.14 314e-2";
	Input in = {
		.type = INT_STRING,
		.string = {
			.s = data,
			.i = 0, .store = 0,
		},
	};

	Token ts[] = {
		{ .type = TOKENTYPE_INT, .value.int_ = 0 },
		{ .type = TOKENTYPE_INT, .value.int_ = 1 },
		{ .type = TOKENTYPE_INT, .value.int_ = 1 },
		{ .type = TOKENTYPE_DOUBLE, .value.double_ = 0.0 },
		{ .type = TOKENTYPE_DOUBLE, .value.double_ = 1.0 },
		{ .type = TOKENTYPE_DOUBLE, .value.double_ = 3.14 },
		{ .type = TOKENTYPE_DOUBLE, .value.double_ = 3.14 },
	};
	size_t t_i = 0;

	bool run = true;
	int ret;
	while ( run ) {
		Token token;
		ret = get_token( &in, NULL, &token );
		if ( ret != 0 ) {
			fprintf( stderr, "error: get_token %i\n", ret );
			break;
		}
		if ( token.type == TOKENTYPE_EOF )
			break;

		TEST_ASSERT_TRUE( token.type == ts[ t_i ].type );
		if ( token.type == TOKENTYPE_INT ) {
			if ( token.value.int_ != ts[ t_i ].value.int_ ) {
				fprintf( stderr, "error: not equal: t_i = %lu: %i == %i\n",
					t_i,
					token.value.int_, ts[ t_i ].value.int_
				);

			}

			TEST_ASSERT_EQUAL_INT(
				token.value.int_, ts[ t_i ].value.int_
			);
		} else {
			char d0[ 64 ], d1[ 64 ];

			snprintf( d0, 64, "%f", token.value.double_ );
			snprintf( d1, 64, "%f", ts[ t_i ].value.double_ );

			bool eq = ( strncmp( d0, d1, 64 ) == 0 );

			if ( eq == false ) {
				fprintf( stderr, "error: not equal: t_i = %lu: %s == %s\n",
					t_i,
					d0, d1
				);

			}
			TEST_ASSERT_TRUE( eq );
		}
		t_i++;
	}

}

void test_operators(void) {
	const char* data = "! * / + - == != < > <= >= ??";
	Input in = {
		.type = INT_STRING,
		.string = {
			.s = data,
			.i = 0, .store = 0,
		},
	};

	TokenType ts[] = {
		TOKENTYPE_EXCLAMATION, TOKENTYPE_STAR,
		TOKENTYPE_SLASH, TOKENTYPE_PLUS,
		TOKENTYPE_MINUS, TOKENTYPE_EQUALS2,
		TOKENTYPE_NOT_EQUALS, TOKENTYPE_LESSER,
		TOKENTYPE_GREATER, TOKENTYPE_LESSER_OR_EQUAL,
		TOKENTYPE_GREATER_OR_EQUAL, TOKENTYPE_QUESTIONMARK2,
		TOKENTYPE_EOF
	};
	size_t t_i = 0;

	bool run = true;
	int ret;
	while ( run ) {
		Token token;
		ret = get_token( &in, NULL, &token );
		if ( ret != 0 ) {
			fprintf( stderr, "error: get_token %i\n", ret );
			break;
		}

		TEST_ASSERT_TRUE( token.type == ts[ t_i ] );
		if ( token.type == TOKENTYPE_EOF )
			break;
		t_i++;
	}
}

void test_strings(void) {
	const char* data = ""
		"\"\" "
		"\" \\\" \" "
		"\"Hello, World!\" ";
	Input in = {
		.type = INT_STRING,
		.string = {
			.s = data,
			.i = 0, .store = 0,
		},
	};

	const char* s[] = {
		"",
		" \" ",
		"Hello, World!"
	};
	size_t s_i = 0;

	bool run = true;
	int ret;
	while ( run ) {
		Token token;
		ret = get_token( &in, NULL, &token );
		if ( ret != 0 ) {
			fprintf( stderr, "error: get_token %i\n", ret );
		}
		TEST_ASSERT_FALSE( ret != 0 );
		if ( token.type == TOKENTYPE_EOF )
			break;

		TEST_ASSERT_TRUE( token.type == TOKENTYPE_STRING );
		const char* ts = token.value.str_.data;

		if ( strcmp( ts, s[ s_i ] ) != 0 ) {
			fprintf( stderr, "error: strings[ %lu ] not equal: \"%s\" \"%s\"\n", s_i, s[ s_i ], ts );
		}

		TEST_ASSERT_TRUE(
			strcmp( token.value.str_.data, s[ s_i ] ) == 0
		);

		clear_string( &token.value.str_ );
		s_i++;
	}
}

void test_misc(void) {
	const char* data = "\n ? = -> : ( ) { } , _";
	Input in = {
		.type = INT_STRING,
		.string = {
			.s = data,
			.i = 0, .store = 0,
		},
	};

	TokenType ts[] = {
		TOKENTYPE_NEWLINE, TOKENTYPE_QUESTIONMARK,
		TOKENTYPE_EQUALS, TOKENTYPE_ARROW,
		TOKENTYPE_COLON,
		TOKENTYPE_PAR_L, TOKENTYPE_PAR_R,
		TOKENTYPE_BRACE_L, TOKENTYPE_BRACE_R,
		TOKENTYPE_COMMA,
		TOKENTYPE_UNDERSCORE,
		TOKENTYPE_EOF
	};
	size_t t_i = 0;

	bool run = true;
	int ret;
	while ( run ) {
		Token token;
		ret = get_token( &in, NULL, &token );
		if ( ret != 0 ) {
			fprintf( stderr, "error: get_token %i near char %lu\n", ret, in.string.i );
			break;
		}

		TEST_ASSERT_TRUE( token.type == ts[ t_i ] );
		if ( token.type == TOKENTYPE_EOF )
			break;
		t_i++;
	}
}

void test_expressions(void) {
	const char* data = "a + b * c";
}

void test_simple_program_0(void) {
	const char* data = ""
		"write(\"Hello, World!\")\n"
		"let a : Int? = readInt()\n"
	;
}

int main(void) {
	UNITY_BEGIN();
	RUN_TEST(test_keywords);
	RUN_TEST(test_identifiers);
	RUN_TEST(test_numbers);
	RUN_TEST(test_operators);
	RUN_TEST(test_misc);
	RUN_TEST(test_strings);

	// TODO
	//RUN_TEST(test_expressions);
	//RUN_TEST(test_simple_program_0);
	return UNITY_END();
}
