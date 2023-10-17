#pragma once

#include "symtable.h"
#include <stdbool.h>
#include "string.h"

typedef enum TokenType {
    TOKENTYPE_NEWLINE,
    TOKENTYPE_EOF,
    // operators
    TOKENTYPE_PLUS,
    TOKENTYPE_MINUS,
	
	TOKENTYPE_QUESTIONMARK,
	TOKENTYPE_EXCLAMATION,
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
    TOKENTYPE_KEYWORD
} TokenType;

typedef enum Keyword {
	KEYWORD_FUNC
	// todo
} Keyword;

typedef struct Token {
    TokenType type;
    union {
        int int_;
        double double_;
        String str_;
        size_t id_index;
		Keyword keyword;
    } value;
} Token;

int get_token( FILE*, SymbolTable*, Token* );
