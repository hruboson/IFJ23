/**
 * Implementace překladače imperativního jazyka IFJ23
 * xsatin03 Ondřej Šatinský
*/

#include "scanner.h"

#include <string.h>
#include <assert.h>
#include <ctype.h>
#include <stdlib.h>

#define arraysize( x ) ( sizeof( x ) / sizeof( x[ 0 ] ) )

int
in_getc( Input* i ) {
	if ( i->type == INT_FILE )
		return getc( i->file.f );
	if ( i->type == INT_STRING ) {
		if ( i->string.store != 0 ) {
			char o = i->string.store;
			i->string.store = 0;
			return o;
		}

		char o = i->string.s[ i->string.i ];

		if ( o == 0 )
			return EOF;

		i->string.i++;

		return o;
	}

	fprintf( stderr, "scanner error: unexpected input type: %i\n", i->type );
	exit( 99 );
}

int
in_ungetc( Input* i, char c ) {
	if ( i->type == INT_FILE )
		return ungetc( c, i->file.f );
	if ( i->type == INT_STRING ) {
		i->string.store = c;
		return c;
	}

	fprintf( stderr, "scanner error: unexpected input type: %i\n", i->type );
	exit( 99 );
}


typedef enum State {
	STATE_START,
	STATE_SLASH,
	STATE_COMMENT_LINE,
	STATE_COMMENT_BLOCK,
	STATE_COMMENT_BLOCK_SLASH,
	STATE_COMMENT_BLOCK_STAR,
	STATE_IDENTIF,
	STATE_GREATER,
	STATE_LESSER,
	STATE_EQUALS,
	STATE_QUESTION,
	STATE_EXCLAMATION,
	STATE_MINUS,
	STATE_NUMBER,
	STATE_NUMBER_DOT,
	STATE_NUMBER_DECIMAL,
	STATE_NUMBER_E,
	STATE_NUMBER_EXPONENT,
	STATE_STRING,
	STATE_STRING_EMPTY,
	STATE_STRING_BODY,
	STATE_STRING_BODY_SLASH,
	STATE_STRING_BODY_U,
	STATE_STRING_BODY_U_B,
	STATE_STRING_3,
	STATE_STRING_3_BODY,
	STATE_STRING_3_BODY_NEWLINE,
	STATE_STRING_3_BODY_NEWLINE_0,
	STATE_STRING_3_BODY_QUOTE,
	STATE_STRING_3_BODY_QUOTE2,
	STATE_STRING_3_BODY_INV_QUOTE,
	STATE_STRING_3_BODY_INV_QUOTE2,
} State;

/*
 * returns true when string is a keyword, false when it is not
 */
bool
is_keyword( String* id, Keyword* k ) {

	struct a {
		const char* id;
		Keyword key;
	};

	/* list of all keywords */
	struct a a[] = {
		{ "Double", KEYWORD_DOUBLE },
		{ "else",   KEYWORD_ELSE },
		{ "func",   KEYWORD_FUNC },
		{ "if",     KEYWORD_IF },
		{ "Int",    KEYWORD_INT },
		{ "let",    KEYWORD_LET },
		{ "nil",    KEYWORD_NIL },
		{ "return", KEYWORD_RETURN },
		{ "String", KEYWORD_STRING },
		{ "var",    KEYWORD_VAR },
		{ "while",  KEYWORD_WHILE },
	};

	// linear search for now
	//   suggestion: compare lengths, hash map
	for ( size_t i = 0; i < arraysize( a ); i++ ) {
		if ( strcmp( a[ i ].id, id->data ) == 0 ) {
			*k = a[ i ].key;
			return true;
		}
	}

	return false;
}

int
get_token( Input* in, SymbolTable* symtab, Token* token ) {

	State state = STATE_START;
	int c, ret;

	int whole = 0;
	char hex_count = 0;
	size_t nested_comment_count = 0;
	double decimal = 0.0;
	size_t decimal_position = 1;
	size_t exponent = 0;
	char exponent_sign = 0; // 0 +, 1 -

	String string;

	while ( true ) {
		c = in_getc( in );
		switch ( state ) {
		case STATE_START:
			switch ( c ) {
			case EOF:
				token->type = TOKENTYPE_EOF;
				return 0;
			case '\n':
				token->type = TOKENTYPE_NEWLINE;
				return 0;
			case ' ': case '\t':
				break;
			case 'a' ... 'z':
			case 'A' ... 'Z':
			case '_':
				state = STATE_IDENTIF;
				init_string( &string );
				c = in_ungetc( in, c );
				if ( c == EOF )
					return 99;
				break;
			case '0' ... '9':
				state = STATE_NUMBER;
				c = in_ungetc( in, c );
				if ( c == EOF )
					return 99;
				break;
			case '"':
				init_string( &string );
				state = STATE_STRING;
				break;
			/* operators */
			case '!':
				state = STATE_EXCLAMATION;
				break;
			case '*':
				token->type = TOKENTYPE_STAR;
				return 0;
			case '/':
				state = STATE_SLASH;
				break;
			case '+':
				token->type = TOKENTYPE_PLUS;
				return 0;
			case '-':
				state = STATE_MINUS;
				break;
			case '=':
				state = STATE_EQUALS;
				break;
			case '<':
				state = STATE_LESSER;
				break;
			case '>':
				state = STATE_GREATER;
				break;
			case '?':
				state = STATE_QUESTION;
				break;
			case ',':
				token->type = TOKENTYPE_COMMA;
				return 0;
			case ':':
				token->type = TOKENTYPE_COLON;
				return 0;
			case '(':
				token->type = TOKENTYPE_PAR_L;
				return 0;
			case ')':
				token->type = TOKENTYPE_PAR_R;
				return 0;
			case '{':
				token->type = TOKENTYPE_BRACE_L;
				return 0;
			case '}':
				token->type = TOKENTYPE_BRACE_R;
				return 0;
			default:
				return 1;
			}
			break;
		case STATE_MINUS:
			if ( c == '>' ) {
				token->type = TOKENTYPE_ARROW;
				return 0;
			}

			if ( c != EOF ) {
				c = in_ungetc( in, c );
				if ( c == EOF )
					return 99;
			}

			token->type = TOKENTYPE_MINUS;
			return 0;
		case STATE_QUESTION:
			if ( c == '?' ) {
				token->type = TOKENTYPE_QUESTIONMARK2;
				return 0;
			}

			if ( c != EOF ) {
				c = in_ungetc( in, c );
				if ( c == EOF )
					return 99;
			}

			token->type = TOKENTYPE_QUESTIONMARK;
			return 0;
		case STATE_GREATER:
			if ( c == '=' ) {
				token->type = TOKENTYPE_GREATER_OR_EQUAL;
				return 0;
			}

			if ( c != EOF ) {
				c = in_ungetc( in, c );
				if ( c == EOF )
					return 99;
			}

			token->type = TOKENTYPE_GREATER;
			return 0;
		case STATE_LESSER:
			if ( c == '=' ) {
				token->type = TOKENTYPE_LESSER_OR_EQUAL;
				return 0;
			}

			if ( c != EOF ) {
				c = in_ungetc( in, c );
				if ( c == EOF )
					return 99;
			}

			token->type = TOKENTYPE_LESSER;
			return 0;
		case STATE_EQUALS:
			if ( c == '=' ) {
				token->type = TOKENTYPE_EQUALS2;
				return 0;
			}

			if ( c != EOF ) {
				c = in_ungetc( in, c );
				if ( c == EOF )
					return 99;
			}

			token->type = TOKENTYPE_EQUALS;
			return 0;
		case STATE_EXCLAMATION:
			if ( c == '=' ) {
				token->type = TOKENTYPE_NOT_EQUALS;
				return 0;
			}


			if ( c != EOF ) {
				c = in_ungetc( in, c );
				if ( c == EOF )
					return 99;
			}

			token->type = TOKENTYPE_EXCLAMATION;
			return 0;
		case STATE_SLASH:
			switch ( c ) {
			case '/':
				state = STATE_COMMENT_LINE;
				break;
			case '*':
				state = STATE_COMMENT_BLOCK;
				break;
			default:
				if ( c != EOF ) {
					c = in_ungetc( in, c );
					if ( c == EOF )
						return 99;
				}

				token->type = TOKENTYPE_SLASH;
				return 0;
			}
			break;
		case STATE_IDENTIF:
			switch ( c ) {
			case 'a' ... 'z':
			case 'A' ... 'Z':
			case '0' ... '9':
			case '_':
				string_append_c( &string, c );
				break;
			default:
				if ( c != EOF ) {
					c = in_ungetc( in, c );
					if ( c == EOF )
						return 99;
				}

				/* check for underscore */
				if ( string.length == 1 && string.data[ 0 ] == '_' ) {
					clear_string( &string );
					token->type = TOKENTYPE_UNDERSCORE;
					return 0;
				}

				/* check if identifier is a keyword */
				ret = is_keyword( &string, &token->value.keyword );
				if ( ret ) {
					clear_string( &string );
					token->type = TOKENTYPE_KEYWORD;
					return 0;
				}

				/* insert identifier into the symbol table */
				token->type = TOKENTYPE_ID;
				ret = symboltable_insert( symtab, &string, &token->value.id );
				if ( ret )
					return ret;

				return 0;
			}
			break;
		case STATE_NUMBER:
			switch ( c ) {
			case '0' ... '9':
				whole = whole * 10 + c - '0';
				break;
			case '.':
				state = STATE_NUMBER_DOT;
				break;
			case 'e': case 'E':
				state = STATE_NUMBER_E;
				break;
			default:
				if ( c != EOF ) {
					c = in_ungetc( in, c );
					if ( c == EOF )
						return 99;
				}

				token->type = TOKENTYPE_INT;
				token->value.int_ = whole;
				return 0;
			}
			break;
		case STATE_NUMBER_DOT:
			switch ( c ) {
			case '0' ... '9':
				decimal_position *= 10;
				decimal += ( c - '0' ) / (double)decimal_position;
				break;
			case 'E':
				state = STATE_NUMBER_E;
				break;
			default:
				if ( c != EOF ) {
					c = in_ungetc( in, c );
					if ( c == EOF )
						return 99;
				}

				token->type = TOKENTYPE_DOUBLE;
				token->value.double_ = whole + decimal;
				return 0;
			}
			break;
		case STATE_NUMBER_E:
			switch ( c ) {
			case '0' ... '9':
				state = STATE_NUMBER_EXPONENT;
				c = in_ungetc( in, c );
				if ( c == EOF )
					return 99;
				break;
			case '+':
				state = STATE_NUMBER_EXPONENT;
				exponent_sign = 0;
				break;
			case '-':
				state = STATE_NUMBER_EXPONENT;
				exponent_sign = 1;
				break;
			default:
				return 1;
			}
			break;
		case STATE_NUMBER_EXPONENT:
			switch ( c ) {
			case '0' ... '9':
				exponent = exponent * 10 + c - '0';
				break;
			default:
				if ( c != EOF ) {
					c = in_ungetc( in, c );
					if ( c == EOF )
						return 99;
				}

				token->type = TOKENTYPE_DOUBLE;
				token->value.double_ = ( whole + decimal );
				if ( exponent_sign == 1 ) {
					for ( size_t i = 0; i < exponent; i++ ) {
						token->value.double_ /= 10;
					}
				} else {
					for ( size_t i = 0; i < exponent; i++ ) {
						token->value.double_ *= 10;
					}
				}
				return 0;
			}
			break;
		case STATE_STRING:
			switch ( c ) {
			case EOF:
				return 1;
			case '"':
				state = STATE_STRING_EMPTY;
				break;
			case '\n':
				return 1;
			default:
				if ( c != EOF ) {
					c = in_ungetc( in, c );
					if ( c == EOF )
						return 99;
				}

				state = STATE_STRING_BODY;
				break;
			}
			break;
		case STATE_STRING_BODY:
			if ( c == '"' ) {
				token->type = TOKENTYPE_STRING;
				token->value.str_ = string;
				return 0;
			} else if ( c == '\\' ) {
				state = STATE_STRING_BODY_SLASH;
				break;
			} else if ( c > 31 ) {
				string_append_c( &string, ( char )c );
			} else {
				return 1;
			}
			break;
		case STATE_STRING_BODY_SLASH:
			switch ( c ) {
			case '0':
				state = STATE_STRING_BODY;
				string_append_c( &string, 0 );
				break;
			case '\\':
				state = STATE_STRING_BODY;
				string_append_c( &string, '\\' );
				break;
			case 't':
				state = STATE_STRING_BODY;
				string_append_c( &string, '\t' );
				break;
			case 'n':
				state = STATE_STRING_BODY;
				string_append_c( &string, '\n' );
				break;
			case 'r':
				state = STATE_STRING_BODY;
				string_append_c( &string, '\r' );
				break;
			case '"':
				state = STATE_STRING_BODY;
				string_append_c( &string, '"' );
				break;
			case '\'':
				state = STATE_STRING_BODY;
				string_append_c( &string, '\'' );
				break;
			case 'u':
				state = STATE_STRING_BODY_U;
				break;
			default:
				return 1;
			}
			break;
		case STATE_STRING_BODY_U:
			if ( c == '{' ) {
				state = STATE_STRING_BODY_U_B;
				hex_count = 0;
				whole = 0;
				break;
			}
			return 1;
		case STATE_STRING_BODY_U_B:
			if ( isxdigit( c ) ) {

				// support up to 6 hex digits
				if ( hex_count >= 6 )
					return 1;

				whole = whole * 16;
				if ( c >= '0' && c <= '9' )
					whole += c - '0';
				else if ( c >= 'a' && c <= 'f' )
					whole += c - 'a' + 10;
				else
					whole += c - 'A' + 10;

				hex_count++;

				break;
			}

			if ( c == '}' ) {
				if ( hex_count == 0 )
					return 1;

				/* add hex value to string */
				if ( hex_count > 0 ) {
					string_append_c( &string, whole & 0xff );
					whole >>= 8;
					hex_count -= 2;
				}

				state = STATE_STRING_BODY;
				break;
			}

			return 1;
		case STATE_STRING_EMPTY:
			switch ( c ) {
			case '"':
				state = STATE_STRING_3;
				break;
			default:
				if ( c != EOF ) {
					c = in_ungetc( in, c );
					if ( c == EOF )
						return 99;
				}

				token->type = TOKENTYPE_STRING;
				token->value.str_ = string;
				return 0;
			}
			break;
		case STATE_STRING_3:
			switch ( c ) {
			case '\n':
				state = STATE_STRING_3_BODY_NEWLINE_0;
				break;
			default:
				return 1;
			}
			break;
		case STATE_STRING_3_BODY:

			if ( c == EOF ) {
				return 1;
			}

			if ( c == '"' ) {
				state = STATE_STRING_3_BODY_INV_QUOTE;
				break;
			}

			if ( c == '\n' ) {
				state = STATE_STRING_3_BODY_NEWLINE;
				break;
			}

			string_append_c( &string, c );

			break;
		case STATE_STRING_3_BODY_INV_QUOTE:
			if ( c == '"' ) {
				state = STATE_STRING_3_BODY_INV_QUOTE2;
				break;
			}

			string_append_c( &string, '"' );

			if ( c != EOF ) {
				c = in_ungetc( in, c );
				if ( c == EOF )
					return 99;
			}

			state = STATE_STRING_3_BODY;
			break;
		case STATE_STRING_3_BODY_INV_QUOTE2:
			if ( c == '"' ) {
				return 1;
			}

			string_append_c( &string, '"' );
			string_append_c( &string, '"' );

			if ( c != EOF ) {
				c = in_ungetc( in, c );
				if ( c == EOF )
					return 99;
			}

			state = STATE_STRING_3_BODY;
			break;
		case STATE_STRING_3_BODY_NEWLINE_0:
			if ( c == EOF )
				return 1;

			if ( c == '"' ) {
				state = STATE_STRING_3_BODY_QUOTE;
				break;
			}

			if ( c == '\n' )
				string_append_c( &string, '\n' );

			c = in_ungetc( in, c );
			if ( c == EOF )
				return 99;

			state = STATE_STRING_3_BODY;
			break;
		case STATE_STRING_3_BODY_NEWLINE:

			if ( c == EOF )
				return 1;

			if ( c == '"' ) {
				state = STATE_STRING_3_BODY_QUOTE;
				break;
			}

			c = in_ungetc( in, c );
			if ( c == EOF )
				return 99;

			state = STATE_STRING_3_BODY;
			string_append_c( &string, '\n' );

			break;
		case STATE_STRING_3_BODY_QUOTE:
			if ( c == EOF )
				return 1;

			if ( c == '"' ) {
				state = STATE_STRING_3_BODY_QUOTE2;
				break;
			}

			c = in_ungetc( in, c );
			if ( c == EOF )
				return 99;

			state = STATE_STRING_3_BODY;
			string_append( &string, "\n\"" );

			break;
		case STATE_STRING_3_BODY_QUOTE2:
			if ( c == EOF )
				return 1;

			if ( c == '"' ) {
				token->type = TOKENTYPE_STRING;
				token->value.str_ = string;
				return 0;
			}

			c = in_ungetc( in, c );
			if ( c == EOF )
				return 99;

			state = STATE_STRING_3_BODY;
			string_append( &string, "\n\"\"" );

			break;
		case STATE_COMMENT_LINE:
			switch ( c ) {
			case EOF:
				token->type = TOKENTYPE_EOF;
				return 0;
			case '\n':
				token->type = TOKENTYPE_NEWLINE;
				return 0;
			default:
				break;
				/* nothing */
			}
			break;
		case STATE_COMMENT_BLOCK:
			switch ( c ) {
			case EOF:
				// not ended comment
				return 1;
			case '/':
				state = STATE_COMMENT_BLOCK_SLASH;
				break;
			case '*':
				state = STATE_COMMENT_BLOCK_STAR;
				break;
			default:
				break;
				/* nothing */
			}
			break;
		case STATE_COMMENT_BLOCK_SLASH:
			switch ( c ) {
			case '*':
				state = STATE_COMMENT_BLOCK;
				nested_comment_count++;
				break;
			case '/':
				break;
			default:
				state = STATE_COMMENT_BLOCK;
			}
			break;
		case STATE_COMMENT_BLOCK_STAR:
			switch ( c ) {
			case '/':
				if ( nested_comment_count == 0 ) {
					state = STATE_START;
					break;
				}
				nested_comment_count--;
				break;
			case '*':
				break;
			default:
				state = STATE_COMMENT_BLOCK;
			}
			break;
		default: // unhandeled/invalid state
#ifndef NDEBUG
			fprintf( stderr, "scanner: error: unhandeled state: %i\n\t%s:%u\n", state, __FILE__, __LINE__ );
#endif
			return 99;
		}
	}

	return 0;
}
