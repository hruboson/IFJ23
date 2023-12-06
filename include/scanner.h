/**
 * Implementace překladače imperativního jazyka IFJ23
 * xhrubo01 Ondřej Hruboš
 * xsatin03 Ondřej Šatinský
 * xjestr04 Radek Jestřabík
 * xborek12 Dominik Borek
*/

#pragma once

#include "symtable.h"
#include "string.h"

#include <stdbool.h>
#include <stdio.h>

typedef enum TokenType {
	TOKENTYPE_NEWLINE,
	TOKENTYPE_EOF,

	// operators
	TOKENTYPE_PLUS,
	TOKENTYPE_MINUS,
	TOKENTYPE_STAR,
	TOKENTYPE_SLASH,
	TOKENTYPE_QUESTIONMARK2,
	TOKENTYPE_EQUALS2,
	TOKENTYPE_NOT_EQUALS,
	TOKENTYPE_LESSER,
	TOKENTYPE_GREATER,
	TOKENTYPE_LESSER_OR_EQUAL,
	TOKENTYPE_GREATER_OR_EQUAL,
	TOKENTYPE_EXCLAMATION,

	TOKENTYPE_QUESTIONMARK,
	TOKENTYPE_EQUALS,
	TOKENTYPE_ARROW,
	TOKENTYPE_COLON,
	TOKENTYPE_PAR_L,
	TOKENTYPE_PAR_R,
	TOKENTYPE_BRACE_L,
	TOKENTYPE_BRACE_R,
	TOKENTYPE_COMMA,
	TOKENTYPE_UNDERSCORE,

	TOKENTYPE_ID,
	TOKENTYPE_STRING,
	TOKENTYPE_INT,
	TOKENTYPE_DOUBLE,
	TOKENTYPE_KEYWORD
} TokenType;

typedef enum Keyword {
	KEYWORD_DOUBLE,
	KEYWORD_ELSE,
	KEYWORD_FUNC,
	KEYWORD_IF,
	KEYWORD_INT,
	KEYWORD_LET,
	KEYWORD_NIL,
	KEYWORD_RETURN,
	KEYWORD_STRING,
	KEYWORD_VAR,
	KEYWORD_WHILE,
} Keyword;

typedef struct Token {
	TokenType type;
	union {
		int int_;
		double double_;
		String str_;
		SymbolRecord* id;
		Keyword keyword;
	} value;
} Token;

typedef enum InputType {
	INT_FILE,
	INT_STRING,
} InputType;

typedef struct Input {
	InputType type;
	union {
		struct {
			FILE* f;
		} file;
		struct {
			const char* s;
			size_t i; // index ve stringu
			char store; // pri in_ungetc se tam ulozi znak
		} string;
	};
} Input;

int in_getc( Input* );
int in_ungetc( Input*, char );

int get_token( Input*, SymbolTable*, Token* );
