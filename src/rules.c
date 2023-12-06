#include "rules.h"

#include <stddef.h>

#define T( n, _t ) \
	static TNT T_##n = {.is_terminal = true, .terminal = _t}

#define NT( n, _n ) \
	static TNT NT_##n = {.is_terminal = false, .non_terminal = _n}

NT(exp,NT_EXP);

TNT exp_ = { .is_terminal = false, .non_terminal = NT_EXP_ };
TNT exp1 = { .is_terminal = false, .non_terminal = NT_EXP1 };
TNT exp1_ = { .is_terminal = true, .non_terminal = NT_EXP2 };
NT(exp2,NT_EXP2);
TNT exp2_ = { .is_terminal = false, .non_terminal = NT_EXP2_ };
TNT exp3 = { .is_terminal = false, .non_terminal = NT_EXP3 };
TNT exp3_ = { .is_terminal = false, .non_terminal = NT_EXP3_ };
TNT exp4 = { .is_terminal = false, .non_terminal = NT_EXP4 };
TNT exp4_ = { .is_terminal = false, .non_terminal = NT_EXP4_ };
TNT exp5 = { .is_terminal = false, .non_terminal = NT_EXP5 };
TNT double_question_mark = { .is_terminal = true, .terminal = T_NIL_TEST };
TNT equal = { .is_terminal = true, .terminal = T_EQUAL };
TNT n_equal = { .is_terminal = true, .terminal = T_N_EQUAL };
TNT lt = { .is_terminal = true, .terminal = T_LT };
TNT gt = { .is_terminal = true, .terminal = T_GT };
TNT lte = { .is_terminal = true, .terminal = T_LTE };
TNT gte = { .is_terminal = true, .terminal = T_GTE };
TNT add = { .is_terminal = true, .terminal = T_ADD };
TNT sub = { .is_terminal = true, .terminal = T_SUB };
TNT mult = { .is_terminal = true, .terminal = T_MULT };
TNT div = { .is_terminal = true, .terminal = T_DIV };
TNT exc = { .is_terminal = true, .terminal = T_NT_EXCLAMATION };
TNT left_par = { .is_terminal = true, .terminal = T_PAR_L };
TNT right_par = { .is_terminal = true, .terminal = T_PAR_R };
TNT t_int = { .is_terminal = true, .terminal = T_INT };
TNT t_string = { .is_terminal = true, .terminal = T_STRING };
TNT t_double = { .is_terminal = true, .terminal = T_DOUBLE };
TNT id = { .is_terminal = true, .terminal = T_ID };
TNT args = { .is_terminal = false, .non_terminal = NT_ARGS };
TNT arg_list = { .is_terminal = false, .non_terminal = NT_ARGS_LIST };
TNT arg_list_n = { .is_terminal = false, .non_terminal = NT_ARG_LIST_N };
TNT e_id = { .is_terminal = false, .non_terminal = NT_E_ID };
TNT exp_id = { .is_terminal = false, .non_terminal = NT_EXP_ID };
TNT comma = { .is_terminal = true, .terminal = T_COMMA };

static RuleTable rt;

Rule*
get_rule( NonTerminal nt, Terminal t ) {
	if ( t < 0 || t >= T_END__ )
		return NULL;
	if ( nt < 0 || nt >= NT_END )
		return NULL;
#if 0
	Rule no_match;
	no_match.valid = false; // syntactic error

	Rule eps;
	eps.valid = true;

	Rule one;
	one.valid = true;
	one.expand_to[0] = &exp1;
	one.expand_to[1] = &exp_;

	Rule two;
	two.valid = true;
	two.expand_to[0] = &double_question_mark;
	two.expand_to[1] = &exp;

	Rule three;
	three.valid = true;
	three.expand_to[0] = &exp2;
	three.expand_to[1] = &exp1_;
	three.expand_to[2] = NULL;

	Rule four_equals;
	four_equals.valid = true;
	four_equals.expand_to[0] = &equal;
	four_equals.expand_to[1] = &exp1;
	four_equals.expand_to[2] = NULL;

	Rule four_n_equals;
	four_n_equals.valid = true;
	four_n_equals.expand_to[0] = &n_equal;
	four_n_equals.expand_to[0] = &exp1;

	Rule four_lt;
	four_lt.valid = true;
	four_lt.expand_to[0] = &lt;
	four_lt.expand_to[1] = &exp1;

	Rule four_gt;
	four_gt.valid = true;
	four_gt.expand_to[0] = &gt;
	four_gt.expand_to[1] = &exp1;


	Rule four_gte;
	four_gte.valid = true;
	four_gte.expand_to[0] = &gte;
	four_gte.expand_to[1] = &exp1;


	Rule four_lte;
	four_lte.valid = true;
	four_lte.expand_to[0] = &lte;
	four_lte.expand_to[1] = &exp1;

	Rule five;
	five.valid = true;
	five.expand_to[0] = &exp3;
	five.expand_to[1] = &exp2_;

	Rule six_add;
	six_add.valid = true;
	six_add.expand_to[0] = &add;
	six_add.expand_to[1] = &exp2;

	Rule six_sub;
	six_sub.valid = true;
	six_sub.expand_to[0] = &sub;
	six_sub.expand_to[1] = &exp2;

	Rule seven;
	seven.valid = true;
	seven.expand_to[0] = &exp4;
	seven.expand_to[1] = &exp3_;

	Rule eight_mult;
	eight_mult.valid = true;
	eight_mult.expand_to[0] = &mult;
	eight_mult.expand_to[1] = &exp3;

	Rule eight_div;
	eight_div.valid = true;
	eight_div.expand_to[0] = &div;
	eight_div.expand_to[1] = &exp3;

	Rule nine;
	nine.valid = true;
	nine.expand_to[0] = &exp5;
	nine.expand_to[1] = &exp4_;

	Rule ten;
	ten.valid = true;
	ten.expand_to[0] = &exc;

	Rule eleven_exp;
	eleven_exp.valid = true;
	eleven_exp.expand_to[0] = &left_par;
	eleven_exp.expand_to[1] = &exp;
	eleven_exp.expand_to[2] = &right_par;

	Rule eleven_t_int;
	eleven_t_int.valid = true;
	eleven_t_int.expand_to[0] = &t_int;

	Rule eleven_t_string;
	eleven_t_string.valid = true;
	eleven_t_string.expand_to[0] = &t_string;

	Rule eleven_t_double;
	eleven_t_double.valid = true;
	eleven_t_double.expand_to[0] = &t_double;

	//! todo rule eleven_id_args => id <args>
	//! todo rules for id()
	Rule eleven_id_args;
	eleven_id_args.valid = true;
	eleven_id_args.expand_to[0] = &id;
	eleven_id_args.expand_to[1] = &args;

	Rule twelve;
	twelve.valid = true;
	twelve.expand_to[0] = &left_par;
	twelve.expand_to[1] = &arg_list;
	twelve.expand_to[2] = &right_par;

	Rule fourteen_id;
	fourteen_id.valid = true;
	fourteen_id.expand_to[0] = &e_id;
	fourteen_id.expand_to[1] = &arg_list_n;

	Rule fourteen_else; // int, double, string
	fourteen_else.valid = true;
	fourteen_else.expand_to[0] = &exp;
	fourteen_else.expand_to[1] = &arg_list_n;

	Rule sixteen;
	sixteen.valid = true;
	sixteen.expand_to[0] = &id;
	sixteen.expand_to[0] = &exp_id;

	Rule eighteen;
	eighteen.valid = true;
	eighteen.expand_to[0] = &comma;
	eighteen.expand_to[1] = &arg_list;

	Rule twentythree;
	eighteen.valid = true;
	eighteen.expand_to[0] = &left_par;
	eighteen.expand_to[1] = &arg_list;
	eighteen.expand_to[2] = &right_par;

	// tabulka v "../LL-table.csv"
	RuleTable rule_table;
	rule_table.table[0][13] = &one;
	rule_table.table[0][14] = &one;
	rule_table.table[0][15] = &one;
	rule_table.table[0][16] = &one;
	rule_table.table[0][17] = &one;

	rule_table.table[1][0] = &two;
	rule_table.table[1][1] = &eps;
	rule_table.table[1][2] = &eps;
	rule_table.table[1][3] = &eps;
	rule_table.table[1][4] = &eps;
	rule_table.table[1][5] = &eps;
	rule_table.table[1][6] = &eps;
	rule_table.table[1][7] = &eps;
	rule_table.table[1][8] = &eps;
	rule_table.table[1][9] = &eps;
	rule_table.table[1][10] = &eps;
	rule_table.table[1][11] = &eps;
	rule_table.table[1][12] = &eps;
	rule_table.table[1][13] = &eps;
	rule_table.table[1][14] = &eps;
	rule_table.table[1][15] = &eps;
	rule_table.table[1][16] = &eps;
	rule_table.table[1][17] = &eps;
	rule_table.table[1][18] = &eps;
	rule_table.table[1][T_END] = &eps;

	rule_table.table[2][13] = &three;
	rule_table.table[2][14] = &three;
	rule_table.table[2][15] = &three;
	rule_table.table[2][16] = &three;
	rule_table.table[2][17] = &three;
	rule_table.table[2][T_END] = &eps;

	rule_table.table[3][0] = &eps;
	rule_table.table[3][1] = &four_equals;
	rule_table.table[3][2] = &four_n_equals;
	rule_table.table[3][3] = &four_lt;
	rule_table.table[3][4] = &four_gt;
	rule_table.table[3][5] = &four_lte;
	rule_table.table[3][6] = &four_gte;
	rule_table.table[3][7] = &eps;
	rule_table.table[3][8] = &eps;
	rule_table.table[3][9] = &eps;
	rule_table.table[3][10] = &eps;
	rule_table.table[3][11] = &eps;
	rule_table.table[3][12] = &eps;
	rule_table.table[3][13] = &eps;
	rule_table.table[3][14] = &eps;
	rule_table.table[3][15] = &eps;
	rule_table.table[3][16] = &eps;
	rule_table.table[3][17] = &eps;
	rule_table.table[3][18] = &eps;
	rule_table.table[3][T_END] = &eps;

	rule_table.table[4][13] = &five;
	rule_table.table[4][14] = &five;
	rule_table.table[4][15] = &five;
	rule_table.table[4][16] = &five;
	rule_table.table[4][17] = &five;
	rule_table.table[4][T_END] = &eps;

	rule_table.table[5][0] = &eps;
	rule_table.table[5][1] = &eps;
	rule_table.table[5][2] = &eps;
	rule_table.table[5][3] = &eps;
	rule_table.table[5][4] = &eps;
	rule_table.table[5][5] = &eps;
	rule_table.table[5][6] = &eps;
	rule_table.table[5][7] = &six_add;
	rule_table.table[5][8] = &six_sub;
	rule_table.table[5][9] = &eps;
	rule_table.table[5][10] = &eps;
	rule_table.table[5][11] = &eps;
	rule_table.table[5][12] = &eps;
	rule_table.table[5][13] = &eps;
	rule_table.table[5][14] = &eps;
	rule_table.table[5][15] = &eps;
	rule_table.table[5][16] = &eps;
	rule_table.table[5][17] = &eps;
	rule_table.table[5][18] = &eps;
	rule_table.table[5][T_END] = &eps;

	rule_table.table[6][13] = &seven;
	rule_table.table[6][14] = &seven;
	rule_table.table[6][15] = &seven;
	rule_table.table[6][16] = &seven;
	rule_table.table[6][17] = &seven;
	rule_table.table[6][T_END] = &eps;

	rule_table.table[7][0] = &eps;
	rule_table.table[7][1] = &eps;
	rule_table.table[7][2] = &eps;
	rule_table.table[7][3] = &eps;
	rule_table.table[7][4] = &eps;
	rule_table.table[7][5] = &eps;
	rule_table.table[7][6] = &eps;
	rule_table.table[7][7] = &eps;
	rule_table.table[7][8] = &eps;
	rule_table.table[7][9] = &eight_mult;
	rule_table.table[7][10] = &eight_div;
	rule_table.table[7][11] = &eps;
	rule_table.table[7][12] = &eps;
	rule_table.table[7][13] = &eps;
	rule_table.table[7][14] = &eps;
	rule_table.table[7][15] = &eps;
	rule_table.table[7][16] = &eps;
	rule_table.table[7][17] = &eps;
	rule_table.table[7][18] = &eps;
	rule_table.table[7][T_END] = &eps;

	rule_table.table[8][13] = &nine;
	rule_table.table[8][14] = &nine;
	rule_table.table[8][15] = &nine;
	rule_table.table[8][16] = &nine;
	rule_table.table[8][17] = &nine;
	rule_table.table[8][T_END] = &eps;

	rule_table.table[9][0] = &eps;
	rule_table.table[9][1] = &eps;
	rule_table.table[9][2] = &eps;
	rule_table.table[9][3] = &eps;
	rule_table.table[9][4] = &eps;
	rule_table.table[9][5] = &eps;
	rule_table.table[9][6] = &eps;
	rule_table.table[9][7] = &eps;
	rule_table.table[9][8] = &eps;
	rule_table.table[9][9] = &eps;
	rule_table.table[9][10] = &eps;
	rule_table.table[9][11] = &ten;
	rule_table.table[9][12] = &eps;
	rule_table.table[9][13] = &eps;
	rule_table.table[9][14] = &eps;
	rule_table.table[9][15] = &eps;
	rule_table.table[9][16] = &eps;
	rule_table.table[9][17] = &eps;
	rule_table.table[9][18] = &eps;
	rule_table.table[9][T_END] = &eps;

	rule_table.table[10][13] = &eleven_exp;
	rule_table.table[10][14] = &eleven_t_int;
	rule_table.table[10][15] = &eleven_t_string;
	rule_table.table[10][16] = &eleven_t_double;
	rule_table.table[10][17] = &eleven_id_args;
	rule_table.table[10][T_END] = &eps;

	rule_table.table[11][12] = &twelve;
	rule_table.table[11][T_END] = &eps;

	rule_table.table[12][12] = &eleven_id_args; //! check this
	rule_table.table[12][13] = &eleven_id_args; //! check this
	rule_table.table[12][14] = &eleven_id_args; //! check this
	rule_table.table[12][15] = &eleven_id_args; //! check this
	rule_table.table[12][16] = &eleven_id_args; //! check this
	rule_table.table[12][T_END] = &eps;

	rule_table.table[13][16] = &sixteen;
	rule_table.table[14][17] = &eighteen;
	rule_table.table[15][12] = &twentythree;
#endif

	return rt.table[ t ][ nt ];
}
