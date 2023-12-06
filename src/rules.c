#include "rules.h"

#include <stddef.h>

#define T( n, _t ) \
	static TNT T_##n = {.is_terminal = true, .terminal = _t}

#define NT( n, _n ) \
	static TNT NT_##n = {.is_terminal = false, .non_terminal = _n}

NT(exp,NT_EXP);
NT(exp_, NT_EXP_);
NT(exp1, NT_EXP1);
NT(exp1_, NT_EXP1_);
NT(exp2,NT_EXP2);
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
T(comma, T_COMMA);

static RuleTable rt;

Rule*
get_rule( NonTerminal nt, Terminal t ) {
	if ( t < 0 || t >= T_END__ )
		return NULL;
	if ( nt < 0 || nt >= NT_END )
		return NULL;

	Rule no_match;
	no_match.valid = false; // syntactic error

	Rule eps;
	eps.valid = true;

	Rule one;
	one.valid = true;
	one.expand_to[0] = &NT_exp1;
	one.expand_to[1] = &NT_exp_;

	Rule two;
	two.valid = true;
	two.expand_to[0] = &T_double_question_mark;
	two.expand_to[1] = &NT_exp;

	Rule three;
	three.valid = true;
	three.expand_to[0] = &NT_exp2;
	three.expand_to[1] = &NT_exp1_;
	three.expand_to[2] = NULL;

	Rule four_equals;
	four_equals.valid = true;
	four_equals.expand_to[0] = &T_equal;
	four_equals.expand_to[1] = &NT_exp1;
	four_equals.expand_to[2] = NULL;

	Rule four_n_equals;
	four_n_equals.valid = true;
	four_n_equals.expand_to[0] = &T_n_equal;
	four_n_equals.expand_to[0] = &NT_exp1;

	Rule four_lt;
	four_lt.valid = true;
	four_lt.expand_to[0] = &T_lt;
	four_lt.expand_to[1] = &NT_exp1;

	Rule four_gt;
	four_gt.valid = true;
	four_gt.expand_to[0] = &T_gt;
	four_gt.expand_to[1] = &NT_exp1;


	Rule four_gte;
	four_gte.valid = true;
	four_gte.expand_to[0] = &T_gte;
	four_gte.expand_to[1] = &NT_exp1;


	Rule four_lte;
	four_lte.valid = true;
	four_lte.expand_to[0] = &T_lte;
	four_lte.expand_to[1] = &NT_exp1;

	Rule five;
	five.valid = true;
	five.expand_to[0] = &NT_exp3;
	five.expand_to[1] = &NT_exp2_;

	Rule six_add;
	six_add.valid = true;
	six_add.expand_to[0] = &T_add;
	six_add.expand_to[1] = &NT_exp2;

	Rule six_sub;
	six_sub.valid = true;
	six_sub.expand_to[0] = &T_sub;
	six_sub.expand_to[1] = &NT_exp2;

	Rule seven;
	seven.valid = true;
	seven.expand_to[0] = &NT_exp4;
	seven.expand_to[1] = &NT_exp3_;

	Rule eight_mult;
	eight_mult.valid = true;
	eight_mult.expand_to[0] = &T_mult;
	eight_mult.expand_to[1] = &NT_exp3;

	Rule eight_div;
	eight_div.valid = true;
	eight_div.expand_to[0] = &T_div;
	eight_div.expand_to[1] = &NT_exp3;

	Rule nine;
	nine.valid = true;
	nine.expand_to[0] = &NT_exp5;
	nine.expand_to[1] = &NT_exp4_;

	Rule ten;
	ten.valid = true;
	ten.expand_to[0] = &NT_exp;

	Rule eleven_exp;
	eleven_exp.valid = true;
	eleven_exp.expand_to[0] = &T_left_par;
	eleven_exp.expand_to[1] = &NT_exp;
	eleven_exp.expand_to[2] = &T_right_par;

	Rule eleven_t_int;
	eleven_t_int.valid = true;
	eleven_t_int.expand_to[0] = &T_t_int;

	Rule eleven_t_string;
	eleven_t_string.valid = true;
	eleven_t_string.expand_to[0] = &T_t_string;

	Rule eleven_t_double;
	eleven_t_double.valid = true;
	eleven_t_double.expand_to[0] = &T_t_double;

	//! todo rule eleven_id_args => id <args>
	//! todo rules for id()
	Rule eleven_id_args;
	eleven_id_args.valid = true;
	eleven_id_args.expand_to[0] = &T_id;
	eleven_id_args.expand_to[1] = &NT_args;

	Rule twelve;
	twelve.valid = true;
	twelve.expand_to[0] = &T_left_par;
	twelve.expand_to[1] = &NT_arg_list;
	twelve.expand_to[2] = &T_right_par;

	Rule fourteen_id;
	fourteen_id.valid = true;
	fourteen_id.expand_to[0] = &NT_e_id;
	fourteen_id.expand_to[1] = &NT_arg_list_n;

	Rule fourteen_else; // int, double, string
	fourteen_else.valid = true;
	fourteen_else.expand_to[0] = &NT_exp;
	fourteen_else.expand_to[1] = &NT_arg_list_n;

	Rule sixteen;
	sixteen.valid = true;
	sixteen.expand_to[0] = &T_id;
	sixteen.expand_to[0] = &NT_exp_id;

	Rule eighteen;
	eighteen.valid = true;
	eighteen.expand_to[0] = &T_comma;
	eighteen.expand_to[1] = &NT_arg_list;

	Rule twentythree;
	eighteen.valid = true;
	eighteen.expand_to[0] = &T_left_par;
	eighteen.expand_to[1] = &NT_arg_list;
	eighteen.expand_to[2] = &T_right_par;

	// tabulka v "../LL-table.csv"
	RuleTable rule_table;
	rule_table.table[NT_EXP][T_INT] = &one;
	rule_table.table[NT_EXP][T_STRING] = &one;
	rule_table.table[NT_EXP][T_DOUBLE] = &one;
	rule_table.table[NT_EXP][T_ID] = &one;
	rule_table.table[NT_EXP][T_COMMA] = &one;

	rule_table.table[NT_EXP_][T_NIL_TEST] = &two;
	rule_table.table[NT_EXP_][T_EQUAL] = &eps;
	rule_table.table[NT_EXP_][T_N_EQUAL] = &eps;
	rule_table.table[NT_EXP_][T_N_EQUAL] = &eps;
	rule_table.table[NT_EXP_][T_GT] = &eps;
	rule_table.table[NT_EXP_][T_LTE] = &eps;
	rule_table.table[NT_EXP_][T_GTE] = &eps;
	rule_table.table[NT_EXP_][T_ADD] = &eps;
	rule_table.table[NT_EXP_][T_SUB] = &eps;
	rule_table.table[NT_EXP_][T_MULT] = &eps;
	rule_table.table[NT_EXP_][T_DIV] = &eps;
	rule_table.table[NT_EXP_][T_NT_EXCLAMATION] = &eps;
	rule_table.table[NT_EXP_][T_PAR_L] = &eps;
	rule_table.table[NT_EXP_][T_INT] = &eps;
	rule_table.table[NT_EXP_][T_STRING] = &eps;
	rule_table.table[NT_EXP_][T_DOUBLE] = &eps;
	rule_table.table[NT_EXP_][T_ID] = &eps;
	rule_table.table[NT_EXP_][T_COMMA] = &eps;
	rule_table.table[NT_EXP_][T_PAR_R] = &eps;
	rule_table.table[NT_EXP_][T_END] = &eps;

	rule_table.table[NT_EXP1][T_INT] = &three;
	rule_table.table[NT_EXP1][T_STRING] = &three;
	rule_table.table[NT_EXP1][T_DOUBLE] = &three;
	rule_table.table[NT_EXP1][T_ID] = &three;
	rule_table.table[NT_EXP1][T_COMMA] = &three;
	rule_table.table[NT_EXP1][T_END] = &eps;

	rule_table.table[NT_EXP1_][T_NIL_TEST] = &eps;
	rule_table.table[NT_EXP1_][T_EQUAL] = &four_equals;
	rule_table.table[NT_EXP1_][T_N_EQUAL] = &four_n_equals;
	rule_table.table[NT_EXP1_][T_N_EQUAL] = &four_lt;
	rule_table.table[NT_EXP1_][T_GT] = &four_gt;
	rule_table.table[NT_EXP1_][T_LTE] = &four_lte;
	rule_table.table[NT_EXP1_][T_GTE] = &four_gte;
	rule_table.table[NT_EXP1_][T_ADD] = &eps;
	rule_table.table[NT_EXP1_][T_SUB] = &eps;
	rule_table.table[NT_EXP1_][T_MULT] = &eps;
	rule_table.table[NT_EXP1_][T_DIV] = &eps;
	rule_table.table[NT_EXP1_][T_NT_EXCLAMATION] = &eps;
	rule_table.table[NT_EXP1_][T_PAR_L] = &eps;
	rule_table.table[NT_EXP1_][T_INT] = &eps;
	rule_table.table[NT_EXP1_][T_STRING] = &eps;
	rule_table.table[NT_EXP1_][T_DOUBLE] = &eps;
	rule_table.table[NT_EXP1_][T_ID] = &eps;
	rule_table.table[NT_EXP1_][T_COMMA] = &eps;
	rule_table.table[NT_EXP1_][T_PAR_R] = &eps;
	rule_table.table[NT_EXP1_][T_END] = &eps;

	rule_table.table[NT_EXP2][T_INT] = &five;
	rule_table.table[NT_EXP2][T_STRING] = &five;
	rule_table.table[NT_EXP2][T_DOUBLE] = &five;
	rule_table.table[NT_EXP2][T_ID] = &five;
	rule_table.table[NT_EXP2][T_COMMA] = &five;
	rule_table.table[NT_EXP2][T_END] = &eps;

	rule_table.table[NT_EXP2_][T_NIL_TEST] = &eps;
	rule_table.table[NT_EXP2_][T_EQUAL] = &eps;
	rule_table.table[NT_EXP2_][T_N_EQUAL] = &eps;
	rule_table.table[NT_EXP2_][T_N_EQUAL] = &eps;
	rule_table.table[NT_EXP2_][T_GT] = &eps;
	rule_table.table[NT_EXP2_][T_LTE] = &eps;
	rule_table.table[NT_EXP2_][T_GTE] = &eps;
	rule_table.table[NT_EXP2_][T_ADD] = &six_add;
	rule_table.table[NT_EXP2_][T_SUB] = &six_sub;
	rule_table.table[NT_EXP2_][T_MULT] = &eps;
	rule_table.table[NT_EXP2_][T_DIV] = &eps;
	rule_table.table[NT_EXP2_][T_NT_EXCLAMATION] = &eps;
	rule_table.table[NT_EXP2_][T_PAR_L] = &eps;
	rule_table.table[NT_EXP2_][T_INT] = &eps;
	rule_table.table[NT_EXP2_][T_STRING] = &eps;
	rule_table.table[NT_EXP2_][T_DOUBLE] = &eps;
	rule_table.table[NT_EXP2_][T_ID] = &eps;
	rule_table.table[NT_EXP2_][T_COMMA] = &eps;
	rule_table.table[NT_EXP2_][T_PAR_R] = &eps;
	rule_table.table[NT_EXP2_][T_END] = &eps;

	rule_table.table[NT_EXP3][T_INT] = &seven;
	rule_table.table[NT_EXP3][T_STRING] = &seven;
	rule_table.table[NT_EXP3][T_DOUBLE] = &seven;
	rule_table.table[NT_EXP3][T_ID] = &seven;
	rule_table.table[NT_EXP3][T_COMMA] = &seven;
	rule_table.table[NT_EXP3][T_END] = &eps;

	rule_table.table[NT_EXP3_][T_NIL_TEST] = &eps;
	rule_table.table[NT_EXP3_][T_EQUAL] = &eps;
	rule_table.table[NT_EXP3_][T_N_EQUAL] = &eps;
	rule_table.table[NT_EXP3_][T_N_EQUAL] = &eps;
	rule_table.table[NT_EXP3_][T_GT] = &eps;
	rule_table.table[NT_EXP3_][T_LTE] = &eps;
	rule_table.table[NT_EXP3_][T_GTE] = &eps;
	rule_table.table[NT_EXP3_][T_ADD] = &eps;
	rule_table.table[NT_EXP3_][T_SUB] = &eps;
	rule_table.table[NT_EXP3_][T_MULT] = &eight_mult;
	rule_table.table[NT_EXP3_][T_DIV] = &eight_div;
	rule_table.table[NT_EXP3_][T_NT_EXCLAMATION] = &eps;
	rule_table.table[NT_EXP3_][T_PAR_L] = &eps;
	rule_table.table[NT_EXP3_][T_INT] = &eps;
	rule_table.table[NT_EXP3_][T_STRING] = &eps;
	rule_table.table[NT_EXP3_][T_DOUBLE] = &eps;
	rule_table.table[NT_EXP3_][T_ID] = &eps;
	rule_table.table[NT_EXP3_][T_COMMA] = &eps;
	rule_table.table[NT_EXP3_][T_PAR_R] = &eps;
	rule_table.table[NT_EXP3_][T_END] = &eps;

	rule_table.table[NT_EXP4][T_INT] = &nine;
	rule_table.table[NT_EXP4][T_STRING] = &nine;
	rule_table.table[NT_EXP4][T_DOUBLE] = &nine;
	rule_table.table[NT_EXP4][T_ID] = &nine;
	rule_table.table[NT_EXP4][T_COMMA] = &nine;
	rule_table.table[NT_EXP4][T_END] = &eps;

	rule_table.table[NT_EXP4_][T_NIL_TEST] = &eps;
	rule_table.table[NT_EXP4_][T_EQUAL] = &eps;
	rule_table.table[NT_EXP4_][T_N_EQUAL] = &eps;
	rule_table.table[NT_EXP4_][T_N_EQUAL] = &eps;
	rule_table.table[NT_EXP4_][T_GT] = &eps;
	rule_table.table[NT_EXP4_][T_LTE] = &eps;
	rule_table.table[NT_EXP4_][T_GTE] = &eps;
	rule_table.table[NT_EXP4_][T_ADD] = &eps;
	rule_table.table[NT_EXP4_][T_SUB] = &eps;
	rule_table.table[NT_EXP4_][T_MULT] = &eps;
	rule_table.table[NT_EXP4_][T_DIV] = &eps;
	rule_table.table[NT_EXP4_][T_NT_EXCLAMATION] = &ten;
	rule_table.table[NT_EXP4_][T_PAR_L] = &eps;
	rule_table.table[NT_EXP4_][T_INT] = &eps;
	rule_table.table[NT_EXP4_][T_STRING] = &eps;
	rule_table.table[NT_EXP4_][T_DOUBLE] = &eps;
	rule_table.table[NT_EXP4_][T_ID] = &eps;
	rule_table.table[NT_EXP4_][T_COMMA] = &eps;
	rule_table.table[NT_EXP4_][T_PAR_R] = &eps;
	rule_table.table[NT_EXP4_][T_END] = &eps;

	rule_table.table[NT_EXP5][T_INT] = &eleven_exp;
	rule_table.table[NT_EXP5][T_STRING] = &eleven_t_int;
	rule_table.table[NT_EXP5][T_DOUBLE] = &eleven_t_string;
	rule_table.table[NT_EXP5][T_ID] = &eleven_t_double;
	rule_table.table[NT_EXP5][T_COMMA] = &eleven_id_args;
	rule_table.table[NT_EXP5][T_END] = &eps;

	rule_table.table[NT_ARGS][T_PAR_L] = &twelve;
	rule_table.table[NT_ARGS][T_END] = &eps;

	rule_table.table[NT_ARGS_LIST][T_PAR_L] = &eleven_id_args; //! check this
	rule_table.table[NT_ARGS_LIST][T_INT] = &eleven_id_args; //! check this
	rule_table.table[NT_ARGS_LIST][T_STRING] = &eleven_id_args; //! check this
	rule_table.table[NT_ARGS_LIST][T_DOUBLE] = &eleven_id_args; //! check this
	rule_table.table[NT_ARGS_LIST][T_ID] = &eleven_id_args; //! check this
	rule_table.table[NT_ARGS_LIST][T_END] = &eps;

	rule_table.table[NT_E_ID][T_ID] = &sixteen;
	rule_table.table[NT_ARG_LIST_N][T_COMMA] = &eighteen;
	rule_table.table[NT_EXP_ID][T_PAR_L] = &twentythree;

	return rt.table[ t ][ nt ];
}
