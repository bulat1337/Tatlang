#ifndef RECURSIVE_PARSER_SECONDARY_H
#define RECURSIVE_PARSER_SECONDARY_H

#include "recursive_parser.h"

extern Tokens *tokens;
extern size_t id;

#define CUR_TYPE\
	tokens->data[id].type

#define CUR_OP\
	tokens->data[id].value.op_value

#define CUR_NUM\
	tokens->data[id].value.num_value

#define CR_SMC(left_child, right_child)\
	create_node(SMC, {.num_value = 0}, left_child, right_child).arg.node;

#define CR_KWD(val, left_child, right_child)\
	create_node(KWD, {.var_value = val}, left_child, right_child).arg.node;

#define CR_ASS(left_child, right_child)\
	create_node(OP, {.op_value = ASS}, left_child, right_child).arg.node;

#define CR_OP(op, left_child, right_child)\
	create_node(OP, {.op_value = op}, left_child, right_child).arg.node

#define CR_NUM(val, left_child, right_child)\
	create_node(NUM, {.num_value = val}, left_child, right_child).arg.node;

#define CR_VAR(val, left_child, right_child)\
	create_node(VAR, {.var_value = val}, left_child, right_child).arg.node;

#define CR_SCS(left_child, right_child)\
	create_node(SCS, {.num_value = 0}, left_child, right_child).arg.node;

#define CR_SCE\
	create_node(SCE, {.num_value = 0}, NULL, NULL).arg.node;

#define IS_KWD(var, check_kwd)\
	!strncmp(var, check_kwd, LEN(check_kwd))

#define SYNTAX_CHECK(cond)												\
	if(!(cond))															\
	{																	\
		printf("Syntax ERROR on %d: "#cond" is fasle\n", __LINE__);		\
		printf("%s returning NULL\n", __func__);						\
		return NULL;													\
	}

#define REPORT_ERROR(...)							\
	printf(__VA_ARGS__);							\
	printf("%s returning NULL\n", __func__);		\
	return NULL;

#define SYNTAX_ERROR									\
	REPORT_ERROR("syntax error on: %d\n", __LINE__)		\
	printf("%s returning NULL\n", __func__);			\
	return NULL;

#define CHECK_RET(ptr)								\
	if(ptr == NULL)									\
	{												\
		printf("%s returning NULL\n", __func__);	\
		return NULL;								\
	}												\

#define DO_IF_KWD(kwd, op)															\
	if(!strncmp(var_name, kwd, sizeof(kwd) / sizeof(char))) 						\
	{																				\
		printf("It's kwd: %s\n", var_name);											\
																					\
		if(CUR_TYPE == OBR)															\
		{																			\
			id++;																	\
			B_tree_node *child = get_add();											\
			CHECK_RET(child);														\
																					\
			if(CUR_TYPE == CBR)														\
			{																		\
				id++;																\
				return CR_OP(op, NULL, child);										\
			}																		\
			else																	\
			{																		\
				SYNTAX_ERROR;														\
			}																		\
		}																			\
		else																		\
		{																			\
			SYNTAX_ERROR;															\
		}																			\
	}

#define PARSE_LOG(...)\
	rec_write_log("parse_log.txt", __VA_ARGS__);

void         rec_write_log       (const char *file_name, const char *fmt, ...);

B_tree_node *get_cmd             ();

B_tree_node *get_cond            ();

B_tree_node *get_ass             ();

B_tree_node *get_num             ();

B_tree_node *get_add             ();

B_tree_node *get_mul             ();

B_tree_node *get_par             ();

B_tree_node *get_id              ();

B_tree_node *get_pow             ();

B_tree_node *get_scope           ();

B_tree_node *get_scope_end           (B_tree_node *root);


#endif
