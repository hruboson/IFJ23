/**
 * Implementace překladače imperativního jazyka IFJ23
 * xsatin03 Ondřej Šatinský
*/

#include "utils.h"

const char*
tokentype_to_string( TokenType t ) {
	switch ( t ) {
	case TOKENTYPE_NEWLINE: return "NEWLINE";
	case TOKENTYPE_EOF: return "EOF";
	case TOKENTYPE_PLUS: return "+";
	case TOKENTYPE_MINUS: return "-";
	case TOKENTYPE_STAR: return "*";
	case TOKENTYPE_SLASH: return "/";
	case TOKENTYPE_QUESTIONMARK2: return "??";
	case TOKENTYPE_EQUALS2: return "==";
	case TOKENTYPE_NOT_EQUALS: return "!=";
	case TOKENTYPE_LESSER: return "<";
	case TOKENTYPE_GREATER: return ">";
	case TOKENTYPE_LESSER_OR_EQUAL: return "<=";
	case TOKENTYPE_GREATER_OR_EQUAL: return ">=";
	case TOKENTYPE_EXCLAMATION: return "!";

	case TOKENTYPE_QUESTIONMARK: return "?";
	case TOKENTYPE_EQUALS: return "=";
	case TOKENTYPE_ARROW: return "->";
	case TOKENTYPE_COLON: return ":";
	case TOKENTYPE_PAR_L: return "(";
	case TOKENTYPE_PAR_R: return ")";
	case TOKENTYPE_BRACE_L: return "{";
	case TOKENTYPE_BRACE_R: return "}";
	case TOKENTYPE_COMMA: return ",";
	case TOKENTYPE_UNDERSCORE: return "_";

	case TOKENTYPE_ID: return "ID";
	case TOKENTYPE_STRING: return "String";
	case TOKENTYPE_INT: return "Int";
	case TOKENTYPE_DOUBLE: return "Double";
	case TOKENTYPE_KEYWORD: return "KEYWORD";

	default: return "DEFAULT";
	}
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
		fprintf( stdout, a[ k ] );
	else
		fprintf( stdout, "unhandeled keyword %i", k );

	return;
}

void
print_token( const Token* t ) {
	switch ( t->type ) {
	case TOKENTYPE_NEWLINE: printf( "NL" ); break;
	case TOKENTYPE_EOF: printf( "EOF" ); break;
	case TOKENTYPE_PLUS: printf( "+" ); break;
	case TOKENTYPE_MINUS: printf( "-" ); break;
	case TOKENTYPE_STAR: printf( "*" ); break;
	case TOKENTYPE_SLASH: printf( "/" ); break;
	case TOKENTYPE_QUESTIONMARK2: printf( "??" ); break;
	case TOKENTYPE_EQUALS2: printf( "==" ); break;
	case TOKENTYPE_NOT_EQUALS: printf( "!=" ); break;
	case TOKENTYPE_LESSER: printf( "<" ); break;
	case TOKENTYPE_GREATER: printf( ">" ); break;
	case TOKENTYPE_LESSER_OR_EQUAL: printf( "<=" ); break;
	case TOKENTYPE_GREATER_OR_EQUAL: printf( ">=" ); break;
	case TOKENTYPE_EXCLAMATION: printf( "!" ); break;
	case TOKENTYPE_QUESTIONMARK: printf( "?" ); break;
	case TOKENTYPE_EQUALS: printf( "=" ); break;
	case TOKENTYPE_ARROW: printf( "->" ); break;
	case TOKENTYPE_COLON: printf( ":" ); break;
	case TOKENTYPE_PAR_L: printf( "(" ); break;
	case TOKENTYPE_PAR_R: printf( ")" ); break;
	case TOKENTYPE_BRACE_L: printf( "{" ); break;
	case TOKENTYPE_BRACE_R: printf( "}" ); break;
	case TOKENTYPE_COMMA: printf( "," ); break;
	case TOKENTYPE_UNDERSCORE: printf( "_" ); break;
	case TOKENTYPE_ID:
		printf( "id( %s )", t->value.id->symbol.data );
		break;
	case TOKENTYPE_STRING:
		printf( "string( \"" );
		print_formatted_string( stdout, &t->value.str_ );
		printf( "\" )" );
		break;
	case TOKENTYPE_INT:
		printf( "int( %i )", t->value.int_ );
		break;
	case TOKENTYPE_DOUBLE:
		printf( "double( %f )", t->value.double_ );
		break;
	case TOKENTYPE_KEYWORD:
		printf( "KEYWORD(" );
		print_keyword( t->value.keyword );
		printf( ")" );
		break;
	default:
		printf( "DEFAULT(%i)", t->type );
	}
}

const char*
terminal_to_string( Terminal t ) {
	switch ( t ) {
	case T_NIL_TEST: return "??";
	case T_EQUAL: return "==";
	case T_N_EQUAL: return "!=";
	case T_LT: return "<";
	case T_GT: return ">";
	case T_LTE: return "<=";
	case T_GTE: return ">=";
	case T_ADD: return "+";
	case T_SUB: return "-";
	case T_MULT: return "*";
	case T_DIV: return "/";
	case T_NT_EXCLAMATION: return "!";
	case T_PAR_L: return "(";
	case T_INT: return "int";
	case T_STRING: return "string";
	case T_DOUBLE: return "double";
	case T_ID: return "id";
	case T_NIL: return "nil";
	case T_COMMA: return ",";
	case T_PAR_R: return ")";
	case T_END: return "$";
	default: return "DEFAULT";
	}
}

const char*
nonterminal_to_string( NonTerminal nt ) {
	switch ( nt ) {
	case NT_EXP: return "<exp>";
	case NT_EXP_: return "<exp'>";
	case NT_EXP1: return "<exp1>";
	case NT_EXP1_: return "<exp1'>";
	case NT_EXP2: return "<exp2>";
	case NT_EXP2_: return "<exp2'>";
	case NT_EXP3: return "<exp3>";
	case NT_EXP3_: return "<exp3'>";
	case NT_EXP4: return "<exp4>";
	case NT_EXP4_: return "<exp4'>";
	case NT_EXP5: return "<exp5>";
	case NT_ARGS: return "<args>";
	case NT_ARGS_LIST: return "<args_list>";
	case NT_E_ID: return "<e_id>";
	case NT_ARG_LIST_N: return "<arg_list_n>";
	case NT_EXP_ID: return "<exp_id>";
	default: return "DEFAULT";
	}
}

const char*
tnt_to_string( const TNT* boom ) {
	if ( boom->is_terminal )
		return terminal_to_string( boom->terminal );
	else
		return nonterminal_to_string( boom->non_terminal );
}

void
print_rule_tree( const Node* n, size_t i ) {
	if ( n == NULL )
		return;

	for ( size_t j = 0; j < i; j++ )
		printf( "  " );

	printf( "%s ", n->isTerminal ? "T" : "NT" );
	if ( n->isTerminal ) {
		print_token( n->val );

		printf( "\n" );

		return;
	} else {
		printf( "%s", nonterminal_to_string( n->nt ) );

		printf( "\n" );
		for ( size_t ii = 0; ii < 3; ii++ )
			print_rule_tree( n->children_nodes[ ii ], i + 1 );

		return;
	}

}


