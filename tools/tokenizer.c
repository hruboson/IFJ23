#include "scanner.h"

#include <assert.h>

#define arraysize( x ) ( sizeof( x ) / sizeof( x[0] ) )

void
print_keyword( Keyword k ) {

	const char* a[] = {
		[KEYWORD_DOUBLE] = "Double",
		[KEYWORD_ELSE] = "else",
		[KEYWORD_FUNC] = "func",
		[KEYWORD_IF] = "if",
		[KEYWORD_INT] = "Int",
		[KEYWORD_LET] = "let",
		[KEYWORD_NIL] = "nil",
		[KEYWORD_RETURN] = "return",
		[KEYWORD_STRING] = "String",
		[KEYWORD_VAR] = "var",
		[KEYWORD_WHILE] = "while",
	};

	if ( k <= arraysize( a ) && a[ k ] != NULL )
		fprintf( stderr, a[ k ] );
	else
		fprintf( stderr, "unhandeled keyword %i", k );

	return;
}

void
print_formatted_string( FILE* f, const String* s ) {

	for ( size_t i = 0; i < s->length; i++ ) {
		char c = s->data[ i ];
		if ( c == '\"' ) {
			fputs( "\\\"", f );
		} else if ( c > 31 ) {
			fputc( c, f );
		} else {
			fprintf( f, "\\u{%x}", c );
		}
	}

}

void
print_token( Token* t ) {

	const char* non_special[] = {
		[TOKENTYPE_NEWLINE] = "NEWLINE",
		[TOKENTYPE_EOF] = "EOF",

		/* operators */
		[TOKENTYPE_PLUS] = "+",
		[TOKENTYPE_MINUS] = "-",
		[TOKENTYPE_STAR] = "*",
		[TOKENTYPE_SLASH] = "/",
		[TOKENTYPE_QUESTIONMARK2] = "??",
		[TOKENTYPE_EQUALS2] = "==",
		[TOKENTYPE_NOT_EQUALS] = "!=",
		[TOKENTYPE_LESSER] = "<",
		[TOKENTYPE_GREATER] = ">",
		[TOKENTYPE_LESSER_OR_EQUAL] = ">=",
		[TOKENTYPE_GREATER_OR_EQUAL] = "<=",
		[TOKENTYPE_EXCLAMATION] = "!",

		[TOKENTYPE_QUESTIONMARK] = "?",
		[TOKENTYPE_EQUALS] = "=",
		[TOKENTYPE_ARROW] = "->",
		[TOKENTYPE_COLON] = ":",
		[TOKENTYPE_PAR_L] = "(",
		[TOKENTYPE_PAR_R] = ")",
		[TOKENTYPE_BRACE_L] = "{",
		[TOKENTYPE_BRACE_R] = "}",
		[TOKENTYPE_COMMA] = ",",
		[TOKENTYPE_UNDERSCORE] = "_",
	};

	if ( t->type >= TOKENTYPE_NEWLINE && t->type <= TOKENTYPE_UNDERSCORE ) {
		fprintf( stderr, "%s", non_special[ t->type ] );
		if ( t->type == TOKENTYPE_NEWLINE || t->type == TOKENTYPE_EOF )
			fprintf( stderr, "\n" );
		else
			fprintf( stderr, " " );
		return;
	}

	switch ( t->type ) {
	case TOKENTYPE_ID:
		fprintf( stderr, "$" );
		assert( t->value.id );
		assert( t->value.id->symbol.data );
		fputs( t->value.id->symbol.data, stderr );
		break;
	case TOKENTYPE_STRING:
		fprintf( stderr, "\"" );
		print_formatted_string( stderr, &t->value.str_ );
		fprintf( stderr, "\"" );
		break;
	case TOKENTYPE_INT:
		fprintf( stderr, "%i", t->value.int_ );
		break;
	case TOKENTYPE_DOUBLE:
		fprintf( stderr, "%f", t->value.double_ );
		break;
	case TOKENTYPE_KEYWORD:
		print_keyword( t->value.keyword );
		break;
	default:
		fprintf( stderr, "unhandeled token type %i", t->type );
	}

	fprintf( stderr, " " );

}

int
main() {
	Input in = {
		.type = INT_FILE,
		.file.f = stdin,
	};

	SymbolTable symtab;
	init_symboltable( &symtab );

	bool run = true;
	int ret;
	while ( run ) {
		Token token;
		ret = get_token( &in, &symtab, &token );
		if ( ret != 0 ) {
			fprintf( stderr, "error: get_token %i\n", ret );
			break;
		}

		print_token( &token );
		if ( token.type == TOKENTYPE_EOF )
			break;

		if ( token.type == TOKENTYPE_STRING )
			clear_string( &token.value.str_ );
	}

	clear_symboltable( &symtab );

}
