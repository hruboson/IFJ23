#include "rules.h"

#include <stddef.h>

#define T( n, _t ) \
	TNT T_##n = {.is_terminal = true, .terminal = _t}

#define NT( n, _n ) \
	TNT NT_##n = {.is_terminal = false, .non_terminal = _n}

NT(exp, NT_EXP);
NT(exp_, NT_EXP_);
NT(exp1, NT_EXP1);
NT(exp1_, NT_EXP1_);
NT(exp2, NT_EXP2);
NT(exp2_, NT_EXP2_);
NT(exp3, NT_EXP3);
NT(exp3_, NT_EXP3_);
NT(exp4, NT_EXP4);
NT(exp4_, NT_EXP4_);
NT(exp5, NT_EXP5);
NT(args, NT_ARGS);
NT(arg_list, NT_ARGS_LIST);
NT(arg_list_n, NT_ARG_LIST_N);
NT(e_id, NT_E_ID);
NT(exp_id, NT_EXP_ID);

T(double_question_mark, T_NIL_TEST);
T(equal, T_EQUAL);
T(n_equal, T_N_EQUAL);
T(lt, T_LT);
T(gt, T_GT);
T(lte, T_LTE);
T(gte, T_GTE);
T(add, T_ADD);
T(sub, T_SUB);
T(div, T_DIV);
T(mult, T_MULT);
T(exc, T_NT_EXCLAMATION);
T(left_par, T_PAR_L);
T(right_par, T_PAR_R);
T(t_int, T_INT);
T(t_string, T_STRING);
T(t_double, T_DOUBLE);
T(id, T_ID);
T(nil, T_NIL);
T(comma, T_COMMA);
T(end, T_END);
T(colon, T_COLON);

#define RULE(n) static Rule R_##n = { .expand_to =
#define ENDRULE }

RULE(one) { &NT_exp1, & NT_exp_ } ENDRULE;
RULE(two) { &T_double_question_mark, & NT_exp } ENDRULE;
RULE(three) { &NT_exp2, & NT_exp1_ } ENDRULE;
RULE(four_equals) { &T_equal, & NT_exp1 } ENDRULE;
RULE(four_n_equals) { &T_n_equal, & NT_exp1 } ENDRULE;
RULE(four_lt) { &T_lt, & NT_exp1 } ENDRULE;
RULE(four_gt) { &T_gt, & NT_exp1 } ENDRULE;
RULE(four_lte) { &T_lte, & NT_exp1 } ENDRULE;
RULE(four_gte) { &T_gte, & NT_exp1 } ENDRULE;
RULE(five) { &NT_exp3, & NT_exp2_ } ENDRULE;
RULE(six_add) { &T_add, & NT_exp2 } ENDRULE;
RULE(six_sub) { &T_sub, & NT_exp2 } ENDRULE;
RULE(seven) { &NT_exp4, & NT_exp3_ } ENDRULE;
RULE(eight_mult) { &T_mult, & NT_exp3 } ENDRULE;
RULE(eight_div) { &T_div, & NT_exp3 } ENDRULE;
RULE(nine) { &NT_exp5, & NT_exp4_ } ENDRULE;
RULE(ten) { &T_exc } ENDRULE;
RULE(eleven_exp) { &T_left_par, & NT_exp, & T_right_par } ENDRULE;
RULE(eleven_t_int) { &T_t_int } ENDRULE;
RULE(eleven_t_string) { &T_t_string } ENDRULE;
RULE(eleven_t_double) { &T_t_double } ENDRULE;
RULE(eleven_nil) { &T_nil } ENDRULE;
RULE(eleven_id_args) { &T_id, & NT_args } ENDRULE;
RULE(twelve) { &T_left_par, & NT_arg_list, & T_right_par } ENDRULE;
RULE(fourteen_id) { &NT_e_id, & NT_arg_list_n } ENDRULE;
RULE(fourteen_else) { &NT_exp, & NT_arg_list_n } ENDRULE;
RULE(sixteen) { &T_id, & NT_exp_id } ENDRULE;
RULE(eighteen) { &T_comma, & NT_arg_list } ENDRULE;
RULE(twenty) { &NT_exp4_, & NT_exp3_, & NT_exp2_, & NT_exp1_, & NT_exp_ } ENDRULE;
RULE(twentythree) { &T_left_par, & NT_arg_list, & T_right_par } ENDRULE;
RULE(twentyfour) { &T_colon, & NT_exp } ENDRULE;
RULE(eps) {} ENDRULE;

//TEMP
static Rule eps;


// tabulka v "../LL-table.csv"
static RuleTable rt = {
	.table = {
		[NT_EXP] = {
			[T_INT] = &R_one,
			[T_STRING] = &R_one,
			[T_DOUBLE] = &R_one,
			[T_ID] = &R_one,
			[T_NIL] = &R_one,
			[T_COMMA] = &R_one,
			[T_PAR_L] = &R_one
		},
		[NT_EXP_] = {
			[T_NIL_TEST] = &R_two,
			[T_EQUAL] = &R_eps,
			[T_N_EQUAL] = &R_eps,
			[T_GT] = &R_eps,
			[T_GTE] = &R_eps,
			[T_ADD] = &R_eps,
			[T_SUB] = &R_eps,
			[T_MULT] = &R_eps,
			[T_DIV] = &R_eps,
			[T_NT_EXCLAMATION] = &R_eps,
			[T_PAR_L] = &R_eps,
			[T_INT] = &R_eps,
			[T_STRING] = &R_eps,
			[T_DOUBLE] = &R_eps,
			[T_ID] = &R_eps,
			[T_COMMA] = &R_eps,
			[T_PAR_R] = &R_eps,
			[T_END] = &R_eps
		},
		[NT_EXP1] = {
			[T_INT] = &R_three,
			[T_STRING] = &R_three,
			[T_DOUBLE] = &R_three,
			[T_ID] = &R_three,
			[T_NIL] = &R_three,
			[T_COMMA] = &R_three,
			[T_END] = &R_eps,
			[T_PAR_L] = &R_three
		},
		[NT_EXP1_] = {
			[T_NIL_TEST] = &R_eps,
			[T_EQUAL] = &R_four_equals,
			[T_N_EQUAL] = &R_four_n_equals,
			[T_N_EQUAL] = &R_four_lt,
			[T_GT] = &R_four_gt,
			[T_LTE] = &R_four_lte,
			[T_GTE] = &R_four_gte,
			[T_ADD] = &R_eps,
			[T_SUB] = &R_eps,
			[T_MULT] = &R_eps,
			[T_DIV] = &R_eps,
			[T_NT_EXCLAMATION] = &R_eps,
			[T_PAR_L] = &R_eps,
			[T_INT] = &R_eps,
			[T_STRING] = &R_eps,
			[T_DOUBLE] = &R_eps,
			[T_ID] = &R_eps,
			[T_COMMA] = &R_eps,
			[T_PAR_R] = &R_eps,
			[T_END] = &R_eps
		},
		[NT_EXP2] = {
			[T_PAR_L] = &R_five,
			[T_INT] = &R_five,
			[T_STRING] = &R_five,
			[T_DOUBLE] = &R_five,
			[T_ID] = &R_five,
			[T_NIL] = &R_five,
			[T_COMMA] = &R_five,
			[T_END] = &R_eps
		},
		[NT_EXP2_] = {
			[T_NIL_TEST] = &R_eps,
			[T_EQUAL] = &R_eps,
			[T_N_EQUAL] = &R_eps,
			[T_N_EQUAL] = &R_eps,
			[T_GT] = &R_eps,
			[T_LTE] = &R_eps,
			[T_GTE] = &R_eps,
			[T_ADD] = &R_six_add,
			[T_SUB] = &R_six_sub,
			[T_MULT] = &R_eps,
			[T_DIV] = &R_eps,
			[T_NT_EXCLAMATION] = &R_eps,
			[T_PAR_L] = &R_eps,
			[T_INT] = &R_eps,
			[T_STRING] = &R_eps,
			[T_DOUBLE] = &R_eps,
			[T_ID] = &R_eps,
			[T_COMMA] = &R_eps,
			[T_PAR_R] = &R_eps,
			[T_END] = &R_eps
		},
		[NT_EXP3] = {
			[T_PAR_L] = &R_seven,
			[T_INT] = &R_seven,
			[T_STRING] = &R_seven,
			[T_DOUBLE] = &R_seven,
			[T_ID] = &R_seven,
			[T_NIL] = &R_seven,
			[T_COMMA] = &R_seven,
			[T_END] = &R_eps
		},
		[NT_EXP3_] = {
			[T_NIL_TEST] = &R_eps,
			[T_EQUAL] = &R_eps,
			[T_N_EQUAL] = &R_eps,
			[T_N_EQUAL] = &R_eps,
			[T_GT] = &R_eps,
			[T_LTE] = &R_eps,
			[T_GTE] = &R_eps,
			[T_ADD] = &R_eps,
			[T_SUB] = &R_eps,
			[T_MULT] = &R_eight_mult,
			[T_DIV] = &R_eight_div,
			[T_NT_EXCLAMATION] = &R_eps,
			[T_PAR_L] = &R_eps,
			[T_INT] = &R_eps,
			[T_STRING] = &R_eps,
			[T_DOUBLE] = &R_eps,
			[T_ID] = &R_eps,
			[T_COMMA] = &R_eps,
			[T_PAR_R] = &R_eps,
			[T_END] = &R_eps
		},
		[NT_EXP4] = {
			[T_PAR_L] = &R_nine,
			[T_INT] = &R_nine,
			[T_STRING] = &R_nine,
			[T_DOUBLE] = &R_nine,
			[T_ID] = &R_nine,
			[T_NIL] = &R_nine,
			[T_COMMA] = &R_nine,
			[T_END] = &R_eps
		},
		[NT_EXP4_] = {
			[T_NIL_TEST] = &R_eps,
			[T_EQUAL] = &R_eps,
			[T_N_EQUAL] = &R_eps,
			[T_N_EQUAL] = &R_eps,
			[T_GT] = &R_eps,
			[T_LTE] = &R_eps,
			[T_GTE] = &R_eps,
			[T_ADD] = &R_eps,
			[T_SUB] = &R_eps,
			[T_MULT] = &R_eps,
			[T_DIV] = &R_eps,
			[T_NT_EXCLAMATION] = &R_ten,
			[T_PAR_L] = &R_eps,
			[T_INT] = &R_eps,
			[T_STRING] = &R_eps,
			[T_DOUBLE] = &R_eps,
			[T_ID] = &R_eps,
			[T_COMMA] = &R_eps,
			[T_PAR_R] = &R_eps,
			[T_END] = &R_eps
		},
		[NT_EXP5] = {
			[T_PAR_L] = &R_eleven_exp,
			[T_INT] = &R_eleven_t_int,
			[T_STRING] = &R_eleven_t_string,
			[T_DOUBLE] = &R_eleven_t_double,
			[T_NIL] = &R_eleven_nil,
			[T_ID] = &R_eleven_id_args,
			[T_END] = &R_eps,
			[T_PAR_R] = &R_eps,
		},
		[NT_ARGS] = {
			[T_NIL_TEST] = &R_eps,
			[T_EQUAL] = &R_eps,
			[T_N_EQUAL] = &R_eps,
			[T_LT] = &R_eps,
			[T_GT] = &R_eps,
			[T_LTE] = &R_eps,
			[T_GTE] = &R_eps,
			[T_ADD] = &R_eps,
			[T_SUB] = &R_eps,
			[T_MULT] = &R_eps,
			[T_DIV] = &R_eps,
			[T_NT_EXCLAMATION] = &R_eps,
			[T_INT] = &R_eps,
			[T_STRING] = &R_eps,
			[T_DOUBLE] = &R_eps,
			[T_ID] = &R_eps,
			[T_COMMA] = &R_eps,
			[T_PAR_L] = &R_twelve,
			[T_PAR_R] = &R_eps,
			[T_END] = &R_eps
		},
		[NT_ARGS_LIST] = {
			[T_NIL_TEST] = &R_fourteen_else,
			[T_EQUAL] = &R_fourteen_else,
			[T_N_EQUAL] = &R_fourteen_else,
			[T_LT] = &R_eps,
			[T_GT] = &R_fourteen_else,
			[T_LTE] = &R_fourteen_else,
			[T_GTE] = &R_fourteen_else,
			[T_ADD] = &R_fourteen_else,
			[T_SUB] = &R_fourteen_else,
			[T_MULT] = &R_fourteen_else,
			[T_DIV] = &R_fourteen_else,
			[T_NT_EXCLAMATION] = &R_fourteen_else,
			[T_INT] = &R_fourteen_else,
			[T_STRING] = &R_fourteen_else,
			[T_DOUBLE] = &R_fourteen_else,
			[T_COMMA] = &R_fourteen_else, // ?
			[T_ID] = &R_fourteen_id,
			[T_PAR_R] = &R_eps,
			[T_PAR_L] = &R_fourteen_else
		},
		[NT_E_ID] = {
			[T_NIL_TEST] = &R_eps,
			[T_EQUAL] = &R_eps,
			[T_N_EQUAL] = &R_eps,
			[T_LT] = &R_eps,
			[T_GT] = &R_eps,
			[T_LTE] = &R_eps,
			[T_GTE] = &R_eps,
			[T_ADD] = &R_eps,
			[T_SUB] = &R_eps,
			[T_MULT] = &R_eps,
			[T_DIV] = &R_eps,
			[T_NT_EXCLAMATION] = &R_eps,
			[T_PAR_L] = &R_eps,
			[T_INT] = &R_eps,
			[T_STRING] = &R_eps,
			[T_DOUBLE] = &R_eps,
			[T_ID] = &R_eps,
			[T_COMMA] = &R_eps,
			[T_PAR_R] = &R_eps,
			[T_COLON] = &R_twentyfour,
			[T_ID] = &R_sixteen,
			[T_END] = &R_eps,
		},
		[NT_ARG_LIST_N] = {
			[T_NIL_TEST] = &R_eps,
			[T_EQUAL] = &R_eps,
			[T_N_EQUAL] = &R_eps,
			[T_LT] = &R_eps,
			[T_GT] = &R_eps,
			[T_LTE] = &R_eps,
			[T_GTE] = &R_eps,
			[T_ADD] = &R_eps,
			[T_SUB] = &R_eps,
			[T_MULT] = &R_eps,
			[T_DIV] = &R_eps,
			[T_NT_EXCLAMATION] = &R_eps,
			[T_PAR_L] = &R_eps,
			[T_INT] = &R_eps,
			[T_STRING] = &R_eps,
			[T_DOUBLE] = &R_eps,
			[T_ID] = &R_eps,
			[T_PAR_R] = &R_eps,
			[T_END] = &R_eps,
			[T_ID] = &R_eps,
			[T_COMMA] = &R_eighteen
		},
		[NT_EXP_ID] = {
			[T_NIL_TEST] = &R_twenty,
			[T_EQUAL] = &R_eps,
			[T_N_EQUAL] = &R_eps,
			[T_LT] = &R_eps,
			[T_GT] = &R_eps,
			[T_LTE] = &R_eps,
			[T_GTE] = &R_eps,
			[T_ADD] = &R_twenty,
			[T_SUB] = &R_twenty,
			[T_MULT] = &R_twenty,
			[T_DIV] = &R_twenty,
			[T_NT_EXCLAMATION] = &R_twenty,
			[T_PAR_L] = &R_eps,
			[T_INT] = &R_eps,
			[T_STRING] = &R_eps,
			[T_DOUBLE] = &R_eps,
			[T_ID] = &R_twenty, // ?
			[T_PAR_R] = &R_eps,
			[T_END] = &R_eps,
			[T_PAR_L] = &R_twentythree,
			[T_COMMA] = &R_eps,
		}
	}
};

Rule*
get_rule(NonTerminal nt, Terminal t) {
	if (t < 0 || t >= T_END__)
		return NULL;
	if (nt < 0 || nt >= NT_END)
		return NULL;

	return rt.table[nt][t];
}
