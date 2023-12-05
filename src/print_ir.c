#include "ir.h"

#include <stdio.h>

void
print_ir_inst( const IR_Inst* i ) {
	switch ( i->type ) {
	case IRT_label:
		printf( "\t%s:\n", i->label->symbol.data );
		break;
	case IRT_goto:
		printf( "\tgoto %s\n", i->label->symbol.data );
		break;
	case IRT_branch_nil:
		printf( "\tbranch_nil\n" );
		break;
	case IRT_branch_false:
		printf( "\tbranch_false\n" );
		break;
	case IRT_call:
		printf( "\tcall " );
		if ( i->fn_call.ret_id != NULL )
			printf( "%s ", i->fn_call.ret_id->symbol.data );
		printf( "%s", i->fn_call.fn_id->symbol.data );
		for ( size_t j = 0; j < i->fn_call.arg_count; j++ ) {
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
		printf( "\tadd %s %s %s\n",
			i->ops[ 0 ]->symbol.data,
			i->ops[ 1 ]->symbol.data,
			i->ops[ 2 ]->symbol.data
		);
		break;
	case IRT_sub:
		printf( "\tsub %s %s %s\n",
			i->ops[ 0 ]->symbol.data,
			i->ops[ 1 ]->symbol.data,
			i->ops[ 2 ]->symbol.data
		);
		break;
	case IRT_mul:
		printf( "\tmul %s %s %s\n",
			i->ops[ 0 ]->symbol.data,
			i->ops[ 1 ]->symbol.data,
			i->ops[ 2 ]->symbol.data
		);
		break;
	case IRT_div:
		printf( "\tdiv %s %s %s\n",
			i->ops[ 0 ]->symbol.data,
			i->ops[ 1 ]->symbol.data,
			i->ops[ 2 ]->symbol.data
		);
		break;
	case IRT_idiv:
		printf( "\tadd %s %s %s\n",
			i->ops[ 0 ]->symbol.data,
			i->ops[ 1 ]->symbol.data,
			i->ops[ 2 ]->symbol.data
		);
		break;
	case IRT_concat:
		printf( "\tconcat %s %s %s\n",
			i->ops[ 0 ]->symbol.data,
			i->ops[ 1 ]->symbol.data,
			i->ops[ 2 ]->symbol.data
		);
		break;
	case IRT_equal:
		printf( "\tequal %s %s %s\n",
			i->ops[ 0 ]->symbol.data,
			i->ops[ 1 ]->symbol.data,
			i->ops[ 2 ]->symbol.data
		);
		break;
	case IRT_nequal:
		printf( "\tnot_equal %s %s %s\n",
			i->ops[ 0 ]->symbol.data,
			i->ops[ 1 ]->symbol.data,
			i->ops[ 2 ]->symbol.data
		);
		break;
	case IRT_lt:
		printf( "\tlt %s %s %s\n",
			i->ops[ 0 ]->symbol.data,
			i->ops[ 1 ]->symbol.data,
			i->ops[ 2 ]->symbol.data
		);
		break;
	case IRT_lte:
		printf( "\tlte %s %s %s\n",
			i->ops[ 0 ]->symbol.data,
			i->ops[ 1 ]->symbol.data,
			i->ops[ 2 ]->symbol.data
		);
		break;
	case IRT_gt:
		printf( "\tgt %s %s %s\n",
			i->ops[ 0 ]->symbol.data,
			i->ops[ 1 ]->symbol.data,
			i->ops[ 2 ]->symbol.data
		);
		break;
	case IRT_gte:
		printf( "\tgte %s %s %s\n",
			i->ops[ 0 ]->symbol.data,
			i->ops[ 1 ]->symbol.data,
			i->ops[ 2 ]->symbol.data
		);
		break;
	case IRT_defvar:
		printf( "\tdefvar %s\n",
			i->id->symbol.data
		);
		break;
	case IRT_asgn:
		printf( "\tassign %s %s\n",
			i->ops[ 0 ]->symbol.data,
			i->ops[ 1 ]->symbol.data
		);
		break;
	case IRT_asgn_int:
		printf( "\tassign_int %s %i\n",
			i->a_i.id->symbol.data,
			i->a_i.v
		);
		break;
	case IRT_asgn_double:
		printf( "\tassign_double %s %f\n",
			i->a_d.id->symbol.data,
			i->a_d.v
		);
		break;
	case IRT_asgn_string:
		printf( "\tassign_string %s \"%s\"\n",
			i->a_s.id->symbol.data,
			i->a_s.v.data
		);
		break;
	case IRT_asgn_nil:
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
