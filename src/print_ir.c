/**
 * Implementace překladače imperativního jazyka IFJ23
 * xsatin03 Ondřej Šatinský
*/

#include "ir.h"

#include <stdio.h>

#include <assert.h>

void
print_ir_inst( const IR_Inst* i ) {
	switch ( i->type ) {
	case IRT_label:
		assert( i->label );
		printf( "\t%s:\n", i->label->symbol.data );
		break;
	case IRT_goto:
		assert( i->label );
		printf( "\tgoto %s\n", i->label->symbol.data );
		break;
	case IRT_branch_nil:
		assert( i->label );
		printf( "\tbranch_nil %s\n", i->label->symbol.data );
		break;
	case IRT_branch_false:
		assert( i->label );
		printf( "\tbranch_false %s\n", i->label->symbol.data );
		break;
	case IRT_call:
		printf( "\tcall " );
		if ( i->fn_call.ret_id != NULL )
			printf( "%s ", i->fn_call.ret_id->symbol.data );
		assert( i->fn_call.fn_id );
		printf( "%s", i->fn_call.fn_id->symbol.data );
		for ( size_t j = 0; j < i->fn_call.arg_count; j++ ) {
			assert( i->fn_call.args[ j ] );
			printf( " %s", i->fn_call.args[ j ]->symbol.data );
		}
		printf( "\n" );
		break;
	case IRT_return:
		printf( "\treturn" );
		if ( i->id != NULL ) {
			printf( " %s", i->id->symbol.data );
		}
		printf( "\n" );
		break;
	case IRT_add:
		assert( i->ops[ 0 ] );
		assert( i->ops[ 1 ] );
		assert( i->ops[ 2 ] );
		printf( "\tadd %s %s %s\n",
			i->ops[ 0 ]->symbol.data,
			i->ops[ 1 ]->symbol.data,
			i->ops[ 2 ]->symbol.data
		);
		break;
	case IRT_sub:
		assert( i->ops[ 0 ] );
		assert( i->ops[ 1 ] );
		assert( i->ops[ 2 ] );
		printf( "\tsub %s %s %s\n",
			i->ops[ 0 ]->symbol.data,
			i->ops[ 1 ]->symbol.data,
			i->ops[ 2 ]->symbol.data
		);
		break;
	case IRT_mul:
		assert( i->ops[ 0 ] );
		assert( i->ops[ 1 ] );
		assert( i->ops[ 2 ] );
		printf( "\tmul %s %s %s\n",
			i->ops[ 0 ]->symbol.data,
			i->ops[ 1 ]->symbol.data,
			i->ops[ 2 ]->symbol.data
		);
		break;
	case IRT_div:
		assert( i->ops[ 0 ] );
		assert( i->ops[ 1 ] );
		assert( i->ops[ 2 ] );
		printf( "\tdiv %s %s %s\n",
			i->ops[ 0 ]->symbol.data,
			i->ops[ 1 ]->symbol.data,
			i->ops[ 2 ]->symbol.data
		);
		break;
	case IRT_idiv:
		assert( i->ops[ 0 ] );
		assert( i->ops[ 1 ] );
		assert( i->ops[ 2 ] );
		printf( "\tadd %s %s %s\n",
			i->ops[ 0 ]->symbol.data,
			i->ops[ 1 ]->symbol.data,
			i->ops[ 2 ]->symbol.data
		);
		break;
	case IRT_concat:
		assert( i->ops[ 0 ] );
		assert( i->ops[ 1 ] );
		assert( i->ops[ 2 ] );
		printf( "\tconcat %s %s %s\n",
			i->ops[ 0 ]->symbol.data,
			i->ops[ 1 ]->symbol.data,
			i->ops[ 2 ]->symbol.data
		);
		break;
	case IRT_equal:
		assert( i->ops[ 0 ] );
		assert( i->ops[ 1 ] );
		assert( i->ops[ 2 ] );
		printf( "\tequal %s %s %s\n",
			i->ops[ 0 ]->symbol.data,
			i->ops[ 1 ]->symbol.data,
			i->ops[ 2 ]->symbol.data
		);
		break;
	case IRT_nequal:
		assert( i->ops[ 0 ] );
		assert( i->ops[ 1 ] );
		assert( i->ops[ 2 ] );
		printf( "\tnot_equal %s %s %s\n",
			i->ops[ 0 ]->symbol.data,
			i->ops[ 1 ]->symbol.data,
			i->ops[ 2 ]->symbol.data
		);
		break;
	case IRT_lt:
		assert( i->ops[ 0 ] );
		assert( i->ops[ 1 ] );
		assert( i->ops[ 2 ] );
		printf( "\tlt %s %s %s\n",
			i->ops[ 0 ]->symbol.data,
			i->ops[ 1 ]->symbol.data,
			i->ops[ 2 ]->symbol.data
		);
		break;
	case IRT_lte:
		assert( i->ops[ 0 ] );
		assert( i->ops[ 1 ] );
		assert( i->ops[ 2 ] );
		printf( "\tlte %s %s %s\n",
			i->ops[ 0 ]->symbol.data,
			i->ops[ 1 ]->symbol.data,
			i->ops[ 2 ]->symbol.data
		);
		break;
	case IRT_gt:
		assert( i->ops[ 0 ] );
		assert( i->ops[ 1 ] );
		assert( i->ops[ 2 ] );
		printf( "\tgt %s %s %s\n",
			i->ops[ 0 ]->symbol.data,
			i->ops[ 1 ]->symbol.data,
			i->ops[ 2 ]->symbol.data
		);
		break;
	case IRT_gte:
		assert( i->ops[ 0 ] );
		assert( i->ops[ 1 ] );
		assert( i->ops[ 2 ] );
		printf( "\tgte %s %s %s\n",
			i->ops[ 0 ]->symbol.data,
			i->ops[ 1 ]->symbol.data,
			i->ops[ 2 ]->symbol.data
		);
		break;
	case IRT_defvar:
		assert( i->ops[ 0 ] );
		assert( i->ops[ 1 ] );
		assert( i->ops[ 2 ] );
		printf( "\tdefvar %s\n",
			i->id->symbol.data
		);
		break;
	case IRT_asgn:
		assert( i->ops[ 0 ] );
		assert( i->ops[ 1 ] );
		assert( i->ops[ 2 ] );
		printf( "\tassign %s %s\n",
			i->ops[ 0 ]->symbol.data,
			i->ops[ 1 ]->symbol.data
		);
		break;
	case IRT_asgn_int:
		assert( i->ops[ 0 ] );
		assert( i->ops[ 1 ] );
		assert( i->ops[ 2 ] );
		printf( "\tassign_int %s %i\n",
			i->a_i.id->symbol.data,
			i->a_i.v
		);
		break;
	case IRT_asgn_double:
		assert( i->ops[ 0 ] );
		assert( i->ops[ 1 ] );
		assert( i->ops[ 2 ] );
		printf( "\tassign_double %s %f\n",
			i->a_d.id->symbol.data,
			i->a_d.v
		);
		break;
	case IRT_asgn_string:
		assert( i->ops[ 0 ] );
		assert( i->ops[ 1 ] );
		assert( i->ops[ 2 ] );
		printf( "\tassign_string %s \"%s\"\n",
			i->a_s.id->symbol.data,
			i->a_s.v.data
		);
		break;
	case IRT_asgn_nil:
		assert( i->ops[ 0 ] );
		assert( i->ops[ 1 ] );
		assert( i->ops[ 2 ] );
		printf( "\tassign_nil %s\n",
			i->id->symbol.data
		);
		break;
	default:
		printf( "\tDEFAULT %i\n", i->type );
	}

}

void
print_ir_body( const IR_Body* b ) {
	for ( size_t i = 0; i < b->count; i++ ) {
		print_ir_inst( b->inst + i );
	}
}

void
print_ir( FILE* f, const IR* ir ) {
	// print IR
	printf( "main:\n" );
	print_ir_body( &ir->main );

	for ( size_t i = 0; i < ir->func_count; i++ ) {
		printf( "func %s(", ir->funcs[ i ].id->symbol.data );
		// TODO: args
		printf( ")\n" );
		print_ir_body( &ir->funcs[ i ].body );
	}
}
