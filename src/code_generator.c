#include "code_generator.h"

#include <stdlib.h>

/* output:
 *
 * <main>
 * goto end
 * <functions>
 * end:
 */

enum frame {
	f_g,
	f_l,
	f_t,
};

void
append_frame( String* c, enum frame f ) {
	switch ( f ) {
	case f_g:
		string_append( c, "GF@" );
		break;
	case f_l:
		string_append( c, "LF@" );
		break;
	case f_t:
		string_append( c, "TF@" );
		break;
	default:
		exit( 99 );
	}
}

void
append_var( String* c, enum frame f, SymbolRecord* id ) {
	append_frame( c, f );
	string_append( c, id->symbol.data );
}

void
append_var_str( String* c, enum frame f, const String* id ) {
	append_frame( c, f );
	string_append( c, id->data );
}

void
append_var_cstr( String* c, enum frame f, const char* id ) {
	append_frame( c, f );
	string_append( c, id );
}

void
append_label( String* c, SymbolRecord* l ) {
	string_append( c, l->symbol.data );
}

void
i_defvar( String* c, enum frame f, SymbolRecord* id ) {
	string_append( c, "DEFVAR " );
	append_var( c, f, id );
	string_append( c, "\n" );
}

void
i_defvar_str( String* c, enum frame f, const String* id ) {
	string_append( c, "DEFVAR " );
	append_var_str( c, f, id );
	string_append( c, "\n" );
}

void
i_defvar_cstr( String* c, enum frame f, const char* id ) {
	string_append( c, "DEFVAR " );
	append_var_cstr( c, f, id );
	string_append( c, "\n" );
}

void
i_asgn_int( String* c, enum frame f, SymbolRecord* id, int v ) {
	string_append( c, "MOVE " );
	append_var( c, f, id );

	string_append( c, " int@" );

	char b[ 64 ];
	snprintf( b, sizeof( b ), "%i", v );
	string_append( c, b );

	string_append( c, "\n" );
}

void
i_asgn_double( String* c, enum frame f, SymbolRecord* id, double v ) {
	string_append( c, "MOVE " );

	append_var( c, f, id );

	string_append( c, " float@" );

	char b[ 64 ];
	snprintf( b, sizeof( b ), "%a", v );
	string_append( c, b );

	string_append( c, "\n" );
}

void
i_asgn_string( String* c, enum frame f, SymbolRecord* id, const String* v ) {
	string_append( c, "MOVE " );
	append_var( c, f, id );

	string_append( c, " string@" );
	for ( size_t i = 0; i < v->length; i++ ) {
		char c_ = v->data[ i ];

		if ( c_ <= 32 || c_ == 35 || c_ == 92 ) {
			string_append_c( c, '\\' );
			char b[ 4 ];
			snprintf( b, sizeof( b ), "%03u", c_ );
			string_append( c, b );
		} else
			string_append_c( c, c_ );
	}

	string_append( c, "\n" );
}

void
i_asgn_nil( String* c, enum frame f, SymbolRecord* id ) {
	string_append( c, "MOVE " );
	append_var( c, f, id );

	string_append( c, " nil@nil\n" );
}

void
i_move( String* code, enum frame f_dst, SymbolRecord* dst, enum frame f_src, SymbolRecord* src ) {
	string_append( code, "MOVE " );
	append_var( code, f_dst, dst );
	string_append( code, " " );
	append_var( code, f_src, src );
	string_append( code, "\n" );
}

void
i_move_id_str( String* code, enum frame f_dst, SymbolRecord* dst, enum frame f_src, String* src ) {
	string_append( code, "MOVE " );
	append_var( code, f_dst, dst );
	string_append( code, " " );
	append_var_str( code, f_src, src );
	string_append( code, "\n" );
}

void
i_move_id_cstr( String* code, enum frame f_dst, SymbolRecord* dst, enum frame f_src, const char* src ) {
	string_append( code, "MOVE " );
	append_var( code, f_dst, dst );
	string_append( code, " " );
	append_var_cstr( code, f_src, src );
	string_append( code, "\n" );
}

void
append_ir_inst( String* code, const IR_Inst* i, enum frame f, const IR_Func* fn ) {
	char b0[ 1024 ];
	char b1[ 1024 ];
	switch ( i->type ) {
	case IRT_label:
		string_append( code, "LABEL " );
		append_label( code, i->label );
		string_append( code, "\n" );
		break;
	case IRT_goto:
		string_append( code, "JUMP " );
		append_label( code, i->label );
		string_append( code, "\n" );
		break;
	case IRT_branch_nil:
		string_append( code, "JUMPIFEQ " );
		append_label( code, i->ops[ 0 ] );
		string_append( code, " " );
		string_append( code, "nil@nil" );
		string_append( code, " " );
		append_var( code, f, i->ops[ 1 ] );
		string_append( code, "\n" );
		break;
	case IRT_branch_false:
		string_append( code, "JUMPIFEQ " );
		append_label( code, i->ops[ 0 ] );
		string_append( code, " " );
		string_append( code, "bool@false" );
		string_append( code, " " );
		append_var( code, f, i->ops[ 1 ] );
		string_append( code, "\n" );
		break;
	case IRT_call:
		string_append( code, "CREATEFRAME\n" );
		for ( size_t a_i = 0; a_i < i->fn_call.arg_count; a_i++ ) {
			// defvar
			snprintf( b0, sizeof( b0 ), "%%p%lu", a_i );
			i_defvar_cstr( code, f_t, b0 );

			// move
			string_append( code, "MOVE " );
			// dest
			append_var_cstr( code, f_t, b0 );
			string_append( code, " " );
			// src
			append_var( code, f, i->fn_call.args[ a_i ] );
			string_append( code, "\n" );

		}

		// call
		string_append( code, "CALL " );
		append_label( code, i->fn_call.fn_id );
		string_append( code, "\n" );

		// save result
		if ( i->fn_call.ret_id != NULL ) {
			string_append( code, "MOVE " );
			// dest
			append_var( code, f, i->fn_call.ret_id );
			string_append( code, " " );
			// src
			append_var_cstr( code, f_t, "%ret" );
			string_append( code, "\n" );
		}

		break;
	case IRT_return:
		if ( i->id != NULL ) {
			// move %n -> %ret
			string_append( code, "MOVE " );
			// dest
			append_var_cstr( code, f, "%ret" );
			string_append( code, " " );
			// src
			append_var( code, f, i->id );
			string_append( code, "\n" );
		}

		// goto fn_end
		string_append( code, "JUMP " );
		append_label( code, fn->id );
		string_append( code, "%end\n" );
		break;
	case IRT_add:
		i_defvar( code, f, i->ops[ 2 ] );
		string_append( code, "ADD " );
		append_var( code, f, i->ops[ 0 ] );
		string_append( code, " " );
		append_var( code, f, i->ops[ 1 ] );
		string_append( code, "\n" );
		break;
	case IRT_sub:
		i_defvar( code, f, i->ops[ 2 ] );
		string_append( code, "SUB " );
		append_var( code, f, i->ops[ 0 ] );
		string_append( code, " " );
		append_var( code, f, i->ops[ 1 ] );
		string_append( code, "\n" );
		break;
	case IRT_mul:
		i_defvar( code, f, i->ops[ 2 ] );
		string_append( code, "MUL " );
		append_var( code, f, i->ops[ 0 ] );
		string_append( code, " " );
		append_var( code, f, i->ops[ 1 ] );
		string_append( code, "\n" );
		break;
	case IRT_div:
		i_defvar( code, f, i->ops[ 2 ] );
		string_append( code, "DIR " );
		append_var( code, f, i->ops[ 0 ] );
		string_append( code, " " );
		append_var( code, f, i->ops[ 1 ] );
		string_append( code, "\n" );
		break;
	case IRT_idiv:
		i_defvar( code, f, i->ops[ 2 ] );
		string_append( code, "IDIV " );
		append_var( code, f, i->ops[ 0 ] );
		string_append( code, " " );
		append_var( code, f, i->ops[ 1 ] );
		string_append( code, "\n" );
		break;
	case IRT_concat:
		i_defvar( code, f, i->ops[ 2 ] );
		string_append( code, "CONCAT " );
		append_var( code, f, i->ops[ 0 ] );
		string_append( code, " " );
		append_var( code, f, i->ops[ 1 ] );
		string_append( code, "\n" );
		break;
	case IRT_equal:
		i_defvar( code, f, i->ops[ 2 ] );
		string_append( code, "EQ " );
		append_var( code, f, i->ops[ 0 ] );
		string_append( code, " " );
		append_var( code, f, i->ops[ 1 ] );
		string_append( code, "\n" );
		break;
	case IRT_nequal:
		snprintf( b0, sizeof( b0 ), "%s_n", i->ops[ 2 ]->symbol.data );
		i_defvar_cstr( code, f, b0 );
		string_append( code, "EQ " );
		append_var( code, f, i->ops[ 0 ] );
		string_append( code, " " );
		append_var( code, f, i->ops[ 1 ] );
		string_append( code, "\n" );

		i_defvar( code, f, i->ops[ 2 ] );
		string_append( code, "NOT " );
		append_var( code, f, i->ops[ 2 ] );
		string_append( code, " " );
		append_var_cstr( code, f, b0 );
		string_append( code, "\n" );
		break;
	case IRT_lt:
		i_defvar( code, f, i->ops[ 2 ] );
		string_append( code, "LT " );
		append_var( code, f, i->ops[ 0 ] );
		string_append( code, " " );
		append_var( code, f, i->ops[ 1 ] );
		string_append( code, "\n" );
		break;
	case IRT_lte:
		snprintf( b0, sizeof( b0 ), "%s_lt", i->ops[ 2 ]->symbol.data );
		i_defvar_cstr( code, f, b0 );
		string_append( code, "LT " );
		append_var( code, f, i->ops[ 0 ] );
		string_append( code, " " );
		append_var( code, f, i->ops[ 1 ] );
		string_append( code, "\n" );

		snprintf( b1, sizeof( b1 ), "%s_eq", i->ops[ 2 ]->symbol.data );
		i_defvar_cstr( code, f, b1 );
		string_append( code, "EQ " );
		append_var( code, f, i->ops[ 0 ] );
		string_append( code, " " );
		append_var( code, f, i->ops[ 1 ] );
		string_append( code, "\n" );

		i_defvar( code, f, i->ops[ 2 ] );
		string_append( code, "OR " );
		append_var( code, f, i->ops[ 2 ] );
		string_append( code, " " );
		append_var_cstr( code, f, b0 );
		string_append( code, " " );
		append_var_cstr( code, f, b1 );
		string_append( code, "\n" );
		break;
	case IRT_gt:
		i_defvar( code, f, i->ops[ 2 ] );
		string_append( code, "GT " );
		append_var( code, f, i->ops[ 0 ] );
		string_append( code, " " );
		append_var( code, f, i->ops[ 1 ] );
		string_append( code, "\n" );
		break;
	case IRT_gte:
		snprintf( b0, sizeof( b0 ), "%s_gt", i->ops[ 2 ]->symbol.data );
		i_defvar_cstr( code, f, b0 );
		string_append( code, "GT " );
		append_var( code, f, i->ops[ 0 ] );
		string_append( code, " " );
		append_var( code, f, i->ops[ 1 ] );
		string_append( code, "\n" );

		snprintf( b1, sizeof( b1 ), "%s_eq", i->ops[ 2 ]->symbol.data );
		i_defvar_cstr( code, f, b1 );
		string_append( code, "EQ " );
		append_var( code, f, i->ops[ 0 ] );
		string_append( code, " " );
		append_var( code, f, i->ops[ 1 ] );
		string_append( code, "\n" );

		i_defvar( code, f, i->ops[ 2 ] );
		string_append( code, "OR " );
		append_var( code, f, i->ops[ 2 ] );
		string_append( code, " " );
		append_var_cstr( code, f, b0 );
		string_append( code, " " );
		append_var_cstr( code, f, b1 );
		string_append( code, "\n" );
		break;

	case IRT_defvar:
		i_defvar( code, f, i->id );
		break;
	case IRT_asgn:
		i_move( code, f, i->ops[ 0 ], f, i->ops[ 1 ] );
		break;
	case IRT_asgn_int:
		i_defvar( code, f, i->a_i.id );
		i_asgn_int( code, f, i->a_i.id, i->a_i.v );
		break;
	case IRT_asgn_double:
		i_defvar( code, f, i->a_d.id );
		i_asgn_double( code, f, i->a_d.id, i->a_d.v );
		break;
	case IRT_asgn_string:
		i_defvar( code, f, i->a_s.id );
		i_asgn_string( code, f, i->a_s.id, &i->a_s.v );
		break;
	case IRT_asgn_nil:
		i_defvar( code, f, i->id );
		i_asgn_nil( code, f, i->a_i.id );
		break;
	}
}

void
append_ir_fn_init( String* c, const IR_Func* f ) {
	// PUSHFRAME
	string_append( c, "PUSHFRAME\n" );
	// MOVE %p_n -> args
	for ( size_t i = 0; i < f->param_count; i++ ) {
		i_defvar( c, f_l, f->params[ i ] );
		char b[ 64 ];
		snprintf( b, sizeof( b ), "%%p%lu", i );
		i_move_id_cstr( c, f_l, f->params[ i ], f_l, b );
	}
}

void
append_ir_fn_end( String* c, const IR_Func* f ) {
	// label fn_name%end
	string_append( c, "LABEL " );
	append_label( c, f->id );
	string_append( c, "%end\n" );
	// POPFRAME
	string_append( c, "POPFRAME\n" );
}

int
generate_code( const IR* ir, String* code ) {

	string_append( code, ".IFJcode23\n" );

	for ( size_t i = 0; i < ir->main.count; i++ ) {
		const IR_Inst* inst = ir->main.inst + i;
		append_ir_inst( code, inst, f_g, NULL );
	}

	string_append( code, "JUMP END\n" );

	for ( size_t i = 0; i < ir->func_count; i++ ) {
		const IR_Func* f = ir->funcs + i;

		// fn init
		append_ir_fn_init( code, f );

		// fn body
		for ( size_t j = 0; j < f->body.count; j++ ) {
			const IR_Inst* inst = f->body.inst + i;
			append_ir_inst( code, inst, f_l, f );
		}

		// fn end
		append_ir_fn_end( code, f );
	}


	string_append( code, "LABEL END\n" );


	return 0;
}
