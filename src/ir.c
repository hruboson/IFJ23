#include "ir.h"

#include <stdlib.h>
#include <assert.h>

#ifdef __GNUC__
#define FALLTHROUGH __attribute__ ((fallthrough))
#else
#define FALLTHROUGH
#endif

void
init_ir_body( IR_Body* b ) {
	b->inst = NULL;
	b->count = b->cap = 0;

	b->cv_i = 0;
	b->cl_i = 0;
}

void
init_ir( IR* ir ) {
	init_ir_body( &ir->main );

	ir->funcs = NULL;
	ir->func_count = 0;
	ir->func_cap = 0;

	init_symboltable( &ir->symtab );
}

void
clear_ir( IR* ir ) {
	// TODO: free main, funcs
	clear_symboltable( &ir->symtab );
}

void
ir_append( IR_Body* b, const IR_Inst* i ) {

	if ( b->count + 1 > b->cap ) {
		if ( b->cap == 0 )
			b->cap = 128;

		b->cap *= 2;

		b->inst = realloc( b->inst, sizeof( *b->inst ) * b->cap );
		if ( b->inst == NULL )
			exit( 99 );
	}

	b->inst[ b->count ] = *i;
	b->count++;
}

void
ir_append_fn( IR* ir, const IR_Func* fn ) {

	if ( ir->func_count + 1 > ir->func_cap ) {
		if ( ir->func_cap == 0 )
			ir->func_cap = 8;

		ir->func_cap *= 2;

		ir->funcs = realloc( ir->funcs, sizeof( *ir->funcs ) * ir->func_cap );
		if ( ir->funcs == NULL )
			exit( 99 );
	}

	ir->funcs[ ir->func_count ] = *fn;
	ir->func_count++;
}

SymbolRecord*
new_cv( IR* ir, IR_Body* b ) {
	char buf[ 64 ];
	snprintf( buf, 64, "%%%lu", b->cv_i );
	b->cv_i++;

	SymbolRecord* out;

	String s;
	init_string( &s );
	string_append( &s, buf );

	symboltable_insert( &ir->symtab, &s, &out );

	return out;
}

SymbolRecord*
new_label( IR* ir, IR_Body* b ) {
	char buf[ 64 ];
	snprintf( buf, 64, "L%lu", b->cl_i );
	b->cl_i++;

	SymbolRecord* out;

	String s;
	init_string( &s );
	string_append( &s, buf );

	symboltable_insert( &ir->symtab, &s, &out );

	return out;
}

void
insert_label( IR_Body* body, SymbolRecord* l ) {
	IR_Inst i;
	i.type = IRT_label;
	i.id = l;
	ir_append( body, &i );
	return;
}

SymbolRecord*
convert_exp( IR* ir, IR_Body* body, Expression* exp ) {
	if ( exp == NULL )
		return NULL;

	IR_Inst i;
	bool bi = false;
	String s;
	SymbolRecord *exp1, *res, *l1, *l2;
	SymbolRecord* id_out = NULL;
	switch ( exp->type ) {
	case ET_INT:
		i.type = IRT_asgn_int;
		i.a_i.v = exp->int_;
		i.a_i.id = new_cv( ir, body );
		ir_append( body, &i );
		return i.a_i.id;
	case ET_DOUBLE:
		i.type = IRT_asgn_double;
		i.a_d.v = exp->double_;
		i.a_d.id = new_cv( ir, body );
		ir_append( body, &i );
		return i.a_d.id;
	case ET_STRING:
		i.type = IRT_asgn_string;
		i.a_s.v = string_copy( &exp->str_ );
		i.a_s.id = new_cv( ir, body );
		ir_append( body, &i );
		return i.a_s.id;
	case ET_ID:
		s = string_copy( &exp->id->symbol );
		symboltable_insert( &ir->symtab, &s, &i.id );
		return i.id;
	case ET_NIL:
		i.type = IRT_asgn_nil;
		i.id = new_cv( ir, body );
		ir_append( body, &i );
		return i.id;
	case ET_ADD:
		if ( bi == false ) {
			assert( exp->ops[ 0 ] != NULL );
			assert( exp->ops[ 1 ] != NULL );
			VarType vt0 = exp->ops[ 0 ]->data_type.type;
			VarType vt1 = exp->ops[ 1 ]->data_type.type;
			if ( vt0 != vt1  ) {
				exit( 99 );
			}

			if ( vt0 == VARTYPE_STRING )
				i.type = IRT_concat;
			else if ( vt0 == VARTYPE_INT || vt0 == VARTYPE_DOUBLE )
				i.type = IRT_add;
			else
				exit( 99 );
			bi = true;
		}
		FALLTHROUGH;
	case ET_SUB:
		if ( bi == false ) {
			assert( exp->ops[ 0 ] != NULL );
			assert( exp->ops[ 1 ] != NULL );
			VarType vt0 = exp->ops[ 0 ]->data_type.type;
			VarType vt1 = exp->ops[ 1 ]->data_type.type;
			if ( vt0 != vt1  ) {
				exit( 99 );
			}

			if ( vt0 != VARTYPE_INT && vt0 != VARTYPE_DOUBLE ) {
				exit( 99 );
			}
			i.type = IRT_sub; bi = true;
		}
		FALLTHROUGH;
	case ET_MULT:
		if ( bi == false ) {
			assert( exp->ops[ 0 ] != NULL );
			assert( exp->ops[ 1 ] != NULL );
			VarType vt0 = exp->ops[ 0 ]->data_type.type;
			VarType vt1 = exp->ops[ 1 ]->data_type.type;
			if ( vt0 != vt1  ) {
				exit( 99 );
			}

			if ( vt0 != VARTYPE_INT && vt0 != VARTYPE_DOUBLE ) {
				exit( 99 );
			}
			i.type = IRT_mul; bi = true;
		}
		FALLTHROUGH;
	case ET_DIV:
		if ( bi == false ) {
			assert( exp->ops[ 0 ] != NULL );
			assert( exp->ops[ 1 ] != NULL );
			VarType vt0 = exp->ops[ 0 ]->data_type.type;
			VarType vt1 = exp->ops[ 1 ]->data_type.type;
			if ( vt0 != vt1 ) {
				fprintf( stderr, "error: ir caught semantic error\n" );
				exit( 99 );
			}

			if ( vt0 == VARTYPE_INT )
				i.type = IRT_idiv;
			if ( vt0 == VARTYPE_DOUBLE )
				i.type = IRT_div;
			else {
				fprintf( stderr, "error: ir caught semantic error\n" );
				exit( 99 );
			}

			bi = true;
		}

		i.ops[ 0 ] = convert_exp( ir, body, exp->ops[ 0 ] );
		i.ops[ 1 ] = convert_exp( ir, body, exp->ops[ 1 ] );
		i.ops[ 2 ] = new_cv( ir, body );
		ir_append( body, &i );
		return i.ops[ 2 ];
	case ET_EQUAL:
		if ( bi == false ) { i.type = IRT_equal; bi = true; }
		__attribute__ ((fallthrough));
	case ET_N_EQUAL:
		if ( bi == false ) { i.type = IRT_nequal; bi = true; }
		__attribute__ ((fallthrough));
	case ET_LT:
		if ( bi == false ) { i.type = IRT_lt; bi = true; }
		__attribute__ ((fallthrough));
	case ET_LTE:
		if ( bi == false ) { i.type = IRT_lte; bi = true; }
		__attribute__ ((fallthrough));
	case ET_GT:
		if ( bi == false ) { i.type = IRT_gt; bi = true; }
		__attribute__ ((fallthrough));
	case ET_GTE:
		if ( bi == false ) { i.type = IRT_gte; bi = true; }

		assert( exp->ops[ 0 ] != NULL );
		assert( exp->ops[ 1 ] != NULL );

		i.ops[0] = convert_exp( ir, body, exp->ops[ 0 ] );
		i.ops[1] = convert_exp( ir, body, exp->ops[ 1 ] );
		i.ops[2] = new_cv( ir, body );
		ir_append( body, &i );
		return i.ops[2];
	case ET_FUNC:
		// built-in write
		if ( strcmp( "write", exp->fn_call.id->symbol.data ) == 0 ) {
			for ( size_t j = 0; j < exp->fn_call.arg_count; j++ ) {
				i.type = IRT_call;
				s = string_copy( &exp->fn_call.id->symbol );
				symboltable_insert( &ir->symtab, &s, &i.fn_call.fn_id );
				i.fn_call.ret_id = NULL;
				i.fn_call.arg_count = 1;
				i.fn_call.args = malloc( sizeof( i.fn_call.args[ 0 ] ) );
				if ( i.fn_call.args == NULL )
					exit( 99 );

				i.fn_call.args[ 0 ] = convert_exp( ir, body, exp->fn_call.args[ j ].exp );

				ir_append( body, &i );
			}

			return NULL;
		}

		// other function calls
		i.type = IRT_call;
		i.fn_call.ret_id = NULL;

		s = string_copy( &exp->fn_call.id->symbol );
		symboltable_insert( &ir->symtab, &s, &i.fn_call.fn_id );

		i.fn_call.arg_count = exp->fn_call.arg_count;
		if ( i.fn_call.arg_count > 0 ) {
			i.fn_call.args = malloc( sizeof( i.fn_call.args[ 0 ] ) * i.fn_call.arg_count );
			if ( i.fn_call.args == NULL )
				exit( 99 );

			for ( size_t a_i = 0; a_i < i.fn_call.arg_count; a_i++ ) {
				i.fn_call.args[ a_i ] = convert_exp( ir, body, exp->fn_call.args[ a_i ].exp );
			}

		} else
			i.fn_call.args = NULL;

		// save return value
		if ( exp->data_type.type != VARTYPE_VOID ) {
			id_out = new_cv( ir, body );
			i.fn_call.ret_id = id_out;
		}

		ir_append( body, &i );
		return id_out;
	case ET_EXCLAMATION:
		s = string_copy( &exp->id->symbol );
		symboltable_insert( &ir->symtab, &s, &i.id );
		return i.id;
	case ET_NIL_TEST:
		// <exp1> ?? <exp2>

		// if <exp1> == nil:
		// 	return <exp2>
		// return <exp1>

		// %n = <exp1>
		// if %n == nil: goto Ln
		// ; %n != nil
		// %(n+1) = %n
		// goto L(n+1)
		// .Ln:
		// %(n+1) = <exp2>
		// .L(n+1):

		exp1 = res = l1 = l2 = NULL;

		i.type = IRT_branch_nil;
		i.ops[ 0 ] = exp1 = convert_exp( ir, body, exp->ops[ 0 ] );
		i.ops[ 1 ] = l1 = new_label( ir, body );
		ir_append( body, &i );

		i.type = IRT_asgn;
		i.ops[ 0 ] = res = new_cv( ir, body );
		i.ops[ 1 ] = exp1;
		ir_append( body, &i );

		i.type = IRT_goto;
		i.id = l2 = new_label( ir, body );
		ir_append( body, &i );

		insert_label( body, l1 );

		i.type = IRT_asgn;
		i.ops[ 0 ] = res;
		i.ops[ 1 ] = convert_exp( ir, body, exp->ops[ 1 ] );
		ir_append( body, &i );

		insert_label( body, l2 );

		return res;
	default:
		exit( 99 );
	}
}

void
convert_st( IR* ir, IR_Body* body, const Statement* st ) {
	IR_Inst inst;
	String s;
	SymbolRecord *l_skip, *l_start, *t0, *l_else;
	SymbolRecord *id;
	IR_Func fn;
	for ( const Statement* i = st; i; i = i->next ) {
		switch ( i->type ) {
		case ST_VAR:
			s = string_copy( &i->var.id->symbol );
			symboltable_insert( &ir->symtab, &s, &id );

			inst.type = IRT_defvar;
			inst.id = id;
			ir_append( body, &inst );

			// skip if no values is assigned
			if ( i->var.exp == NULL )
				continue;

			inst.type = IRT_asgn;
			inst.ops[ 0 ] = id;

			// exp
			inst.ops[ 1 ] = convert_exp( ir, body, i->var.exp );

			ir_append( body, &inst );

			break;
		case ST_IF:
			// if <exp> { <body> } else { <body2> }
			//
			// t0 = exp
			// branch t0 == false: else
			// <body1>
			// goto skip
			// else:
			// <body2>
			// skip:


			t0 = convert_exp( ir, body, i->if_.exp );

			inst.type = IRT_branch_false;
			inst.ops[ 0 ] = t0;
			inst.ops[ 1 ] = l_else = new_label( ir, body );
			ir_append( body, &inst );

			convert_st( ir, body, i->if_.body );

			inst.type = IRT_goto;
			inst.id = l_skip = new_label( ir, body );
			ir_append( body, &inst );

			insert_label( body, l_else );

			convert_st( ir, body, i->if_.else_ );

			insert_label( body, l_skip );

			break;
		case ST_ASSIGN:
			inst.type = IRT_asgn;

			s = string_copy( &i->assign.id->symbol );

			symboltable_insert( &ir->symtab, &s, inst.ops + 0 );

			// exp
			inst.ops[ 1 ] = convert_exp( ir, body, i->assign.exp );

			ir_append( body, &inst );
			break;
		case ST_WHILE:
			// while <exp> { <body> }

			// l_start:
			// t0 = <exp>
			// branch t0 == false: l_skip
			// <body>
			// goto l_start
			// l_skip:

			l_start = new_label( ir, body );
			insert_label( body, l_start );

			t0 = convert_exp( ir, body, i->while_.exp );

			inst.type = IRT_branch_false;
			inst.ops[ 0 ] = t0;
			inst.ops[ 1 ] = l_skip = new_label( ir, body );
			ir_append( body, &inst );

			convert_st( ir, body, i->while_.body );

			inst.type = IRT_goto;
			inst.id = l_start;
			ir_append( body, &inst );

			insert_label( body, l_skip );

			break;

		case ST_RETURN:
			inst.type = IRT_return;

			// exp
			inst.id = convert_exp( ir, body, i->return_.exp );

			ir_append( body, &inst );
			break;

		case ST_EXP:
			convert_exp( ir, body, i->exp.exp );
			break;
		case ST_FUNC:
			s = string_copy( &i->func.id->symbol );
			symboltable_insert( &ir->symtab, &s, &fn.id );

			//args
			fn.param_count = i->func.param_count;
			if ( fn.param_count > 0 ) {
				fn.params = malloc( fn.param_count * sizeof ( fn.params[ 0 ] ) );
				if ( fn.params == NULL )
					exit( 99 );

				for ( size_t pi = 0; pi < fn.param_count; pi++ ) {
					s = string_copy( &i->func.parameters[ pi ].intern_id->symbol );
					symboltable_insert( &ir->symtab, &s, &fn.params[ pi ] );
				}
			} else
				fn.params = NULL;

			init_ir_body( &fn.body );
			convert_st( ir, &fn.body, i->func.body );

			ir_append_fn( ir, &fn );

			break;
		default:
			fprintf( stderr,
				"error: unexpected state %i\n\t%s:%i\n",
				i->type, __FILE__, __LINE__
			);
			exit( 99 );
		}
	}

}

int
convert_to_ir( const AST* ast, IR* ir ) {
	convert_st( ir, &ir->main, ast->statement );
	return 0;
}
