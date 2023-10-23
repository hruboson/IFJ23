#include "scanner.h"

#include <string.h>

void string_append( String* s, const char* c ) {
	append_string( s, c );
}

void string_append_c( String* s, char c ) {
	append_string_c( s, c );
}

void clear_string( String* s ) {
	free_string( s );
}

typedef enum State {
	STATE_START,
	STATE_SLASH,
	STATE_COMMENT_LINE,
	STATE_COMMENT_BLOCK,
	STATE_IDENTIF,
	STATE_GREATER,
	STATE_LESSER,
	STATE_EQUALS,
	STATE_QUESTION,
	STATE_EXCLAMATION,
	STATE_NUMBER,
	STATE_NUMBER_DOT,
	STATE_NUMBER_DECIMAL,
	STATE_NUMBER_E,
	STATE_NUMBER_EXPONENT,
	STATE_STRING,
	STATE_STRING_EMPTY,
	STATE_STRING_3,
	STATE_STRING_3_BODY,
} State;

/*
 * returns true when string is a keyword, false when it is not
 */
bool is_keyword( String* id, Keyword* k ) {

	if ( strcmp( id->data, "nil" ) == 0 ) {
		*k = KEYWORD_NIL;
		return true;
	}

	return false;
}

int get_token( FILE* f, SymbolTable* symtab, Token* token ) {

	State state = STATE_START;
	int c, ret;

	int whole = 0;
	double decimal = 0.0;
	size_t decimal_position = 1;
	int exponent = 0;
	char exponent_sign = 0; // 0 +, 1 -

	String string;

	while ( true ) {
		c = getc( f );
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
				c = ungetc( c, f );
				if ( c == EOF )
					return 99;
				break;
			case '0' ... '9':
				state = STATE_NUMBER;
				c = ungetc( c, f );
				if ( c == EOF )
					return 99;
				break;
			case '"':
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
				token->type = TOKENTYPE_MINUS;
				return 0;
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
			default:
				return 1;
			}
			break;
		case STATE_QUESTION:
			if ( c == '?' ) {
				token->type = TOKENTYPE_QUESTIONMARK2;
				return 0;
			}

			if ( c != EOF ) {
				c = ungetc( c, f );
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
				c = ungetc( c, f );
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
				c = ungetc( c, f );
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
				c = ungetc( c, f );
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
				c = ungetc( c, f );
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
					c = ungetc( c, f );
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
					c = ungetc( c, f );
					if ( c == EOF )
						return 99;
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
				ret = symboltable_insert( symtab, &string, &token->value.id_index );
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
					c = ungetc( c, f );
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
			default:
				return 1;
			}
			break;
		case STATE_NUMBER_E:
			switch ( c ) {
			case '0' ... '9':
				state = STATE_NUMBER_EXPONENT;
				c = ungetc( c, f );
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
					c = ungetc( c, f );
					if ( c == EOF )
						return 99;
				}

				token->type = TOKENTYPE_DOUBLE;
				token->value.double_ = ( whole + decimal ); // TODO: exponent
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

			}
			break;
		case STATE_STRING_EMPTY:
			switch ( c ) {
			case '"':
				state = STATE_STRING_3;
				break;
			default:
				if ( c != EOF ) {
					c = ungetc( c, f );
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
				state = STATE_STRING_3_BODY;
				break;
			default:
				return 1;
			}
			break;
		default: // unhandeled state
#ifdef NDEBUG
			fprintf( stderr, "scanner: error: unhandeled state: %i\n\t%s:%lu\n", state, __FILE__, __LINE__ );
#endif
			return 99;
		}
	}

	return 0;
}
