#ifndef RECURSIVE_PARSER_SECONDARY_H
#define RECURSIVE_PARSER_SECONDARY_H

#include "recursive_parser.h"

#define CUR_TYPE\
	tokens->data[id].type

#define CUR_OP\
	tokens->data[id].value.op_value

#define CUR_NUM\
	tokens->data[id].value.num_value

#define CUR_VAR\
	tokens->data[id].value.var_value

#define CUR_STD_FUNC\
	tokens->data[id].value.func

#define CR_SEMICOLON(left_child, right_child)\
	create_node(SEMICOLON, {.num_value = 0}, left_child, right_child).arg.node;

#define CR_MAIN(left_child, right_child)\
	create_node(MAIN, {.num_value = 0}, left_child, right_child).arg.node;

#define CR_COMMA(left_child, right_child)\
	create_node(COMMA, {.num_value = 0}, left_child, right_child).arg.node;

#define CR_RETURN(left_child, right_child)\
	create_node(RETURN, {.num_value = 0}, left_child, right_child).arg.node;

#define CR_SCOPE_END(left_child, right_child)\
	create_node(SCOPE_END, {.num_value = 0}, left_child, right_child).arg.node;

#define CR_COND(type, left_child, right_child)\
	create_node(type, {.num_value = 0}, left_child, right_child).arg.node;

#define CR_KEYWORD(val, left_child, right_child)\
	create_node(KEYWORD, {.var_value = val}, left_child, right_child).arg.node;

#define CR_STD_FUNC(val, left_child, right_child)\
	create_node(STD_FUNC, {.func = val}, left_child, right_child).arg.node;

#define CR_ASS(left_child, right_child)\
	create_node(OP, {.op_value = ASS}, left_child, right_child).arg.node;

#define CR_OP(op, left_child, right_child)\
	create_node(OP, {.op_value = op}, left_child, right_child).arg.node

#define CR_UNR_OP(op, left_child, right_child)\
	create_node(UNR_OP, {.op_value = op}, left_child, right_child).arg.node

#define CR_NUM(val, left_child, right_child)\
	create_node(NUM, {.num_value = val}, left_child, right_child).arg.node;

#define CR_FUNC_DECL(val, left_child, right_child)\
	create_node(FUNC_DECL, val, left_child, right_child).arg.node;

#define CR_CMD_FUNC(val, left_child, right_child)\
	create_node(CMD_FUNC, val, left_child, right_child).arg.node;

#define CR_FUNC(val, left_child, right_child)\
	create_node(FUNC, val, left_child, right_child).arg.node;

#define CR_VAR(val, left_child, right_child)\
	create_node(VAR, {.var_value = val}, left_child, right_child).arg.node;

#define CR_SCOPE_START(left_child, right_child)\
	create_node(SCOPE_START, {.num_value = 0}, left_child, right_child).arg.node;

#define SYNTAX_CHECK(cond)													\
	if(!(cond))																\
	{																		\
		REPORT_ERROR("Syntax ERROR on %d: "#cond" is fasle\n", __LINE__);	\
	}																		\
	else																	\
	{																		\
		PARSE_LOG(#cond" - OK.\n");											\
		id++;																\
	}

#define REPORT_ERROR(...)							\
	PARSE_LOG(__VA_ARGS__);							\
	PARSE_LOG("%s returning NULL\n", __func__);		\
	return NULL;

#define SYNTAX_ERROR								\
	REPORT_ERROR("syntax error on: %d\n", __LINE__)

#define CHECK_RET(ptr)								\
	if(ptr == NULL)									\
	{												\
		REPORT_ERROR(#ptr" is NULL.\n");			\
	}												\

#define PARSE_LOG(...)\
	rec_write_log("parse_log.txt", __VA_ARGS__);

#define PAY_CMD_DEBT;												\
	if(cmds_sce_debt)												\
	{																\
		B_tree_node *cmd_parent = pay_debt_cmd(cmd, cmds_sce_debt);	\
																	\
		return cmd_parent;											\
	}																\
	else															\
	{																\
		return CR_SEMICOLON(cmd, NULL);								\
	}

void         rec_write_log  (const char *file_name, const char *fmt, ...);

B_tree_node *get_general    (Tokens *passed_tokens);

B_tree_node *get_cmd        ();

B_tree_node *get_std_func   ();

B_tree_node *get_cond       (Node_type type);

B_tree_node *get_ass        ();

B_tree_node *get_num        ();

B_tree_node *get_add        ();

B_tree_node *get_mul        ();

B_tree_node *get_par        ();

B_tree_node *get_id         ();

B_tree_node *get_pow        ();

B_tree_node *get_scope      ();

B_tree_node *get_unary      ();

B_tree_node *get_func       (bool cmd_func);

B_tree_node *get_func_decl  ();

B_tree_node *get_return     ();

B_tree_node *get_main       ();

B_tree_node *move_scope_end (B_tree_node *root);

size_t       take_debt      ();

B_tree_node *get_all_scopes (bool manage_ccbrs, Node_type end_type);

B_tree_node *manage_scopes  (B_tree_node *root);

B_tree_node *pay_debt_scope (B_tree_node *root, size_t scopes_sce_debt);

B_tree_node *pay_debt_cmd   (B_tree_node *cmd, size_t cmds_sce_debt);



#endif
