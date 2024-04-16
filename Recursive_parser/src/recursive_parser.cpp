#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#include "recursive_parser.h"
#include "recursive_parser_secondary.h"

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
	REPORT_ERROR("syntax error on: %lu\n", __LINE__)	\
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
		printf("It's kwd: %s\n", var_name);										\
																					\
		if(tokens->data[id].type == OBR)											\
		{																			\
			id++;																	\
			B_tree_node *child = get_add();											\
																					\
			if(tokens->data[id].type == CBR)										\
			{																		\
				id++;																\
				return create_node(OP , {.op_value = op}, NULL, child).arg.node;	\
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

static Tokens *tokens = NULL;
static size_t id = 0;

struct B_tree_node *parse_tokens(Tokens *tkns)
{
	tokens = tkns;

	id = 0;

	struct B_tree_node *val = get_add();
	CHECK_RET(val);

	SYNTAX_CHECK(tokens->data[id].type == END);

	return val;
}

struct B_tree_node *get_num()
{
	btr_elem_t val = tokens->data[id].value.num_value;

	printf("It's num: %lf\n", val);

	id++;

	return create_node(NUM, {.num_value = val}, NULL, NULL).arg.node;
}

struct B_tree_node *get_add()
{
	struct B_tree_node *val = get_mul();
	CHECK_RET(val);

	while(	tokens->data[id].type == OP &&
			(	tokens->data[id].value.op_value == ADD ||
				tokens->data[id].value.op_value == SUB	)	)
	{
		Ops op = tokens->data[id].value.op_value;

		id++;

		struct B_tree_node *val_2 = get_mul();
		CHECK_RET(val_2);

		val = create_node(OP, {.op_value = op}, val, val_2).arg.node;
	}

	return val;
}

struct B_tree_node *get_mul()
{
	struct B_tree_node *val = get_pow();
	CHECK_RET(val);

	while(	tokens->data[id].type == OP &&
			(	tokens->data[id].value.op_value == MUL ||
				tokens->data[id].value.op_value == DIV	)	)
	{
		Ops op = tokens->data[id].value.op_value;

		id++;

		struct B_tree_node *val_2 = get_pow();

		val = create_node(OP, {.op_value = op}, val, val_2).arg.node;
	}

	return val;
}

struct B_tree_node *get_par()
{
	if(tokens->data[id].type == OBR)
	{
		id++;
		struct B_tree_node *val = get_add();
		CHECK_RET(val);

		SYNTAX_CHECK(tokens->data[id].type == CBR);
		id++;
		return val;
	}
	else if(tokens->data[id].type == NUM)
	{
		return get_num();
	}
	else
	{
		return get_id();
	}
}

struct B_tree_node *get_id()
{
	printf("%s log:\n", __func__);

	char *var_name = tokens->data[id].value.var_value;

	printf("name: %s\n", var_name);



	if(tokens->data[id].type == KWD)
	{
		printf("It's KWD.\n");
		id++;

		if(tokens->data[id].type == OBR)
		{
			printf("OBR ok\n");

			id++;
			B_tree_node *child = get_add();

			if(tokens->data[id].type == CBR)
			{
				printf("CBR ok\n");

				id++;
				return create_node(KWD , {.var_value = var_name}, NULL, child).arg.node;
			}
			else
			{
				SYNTAX_ERROR;
			}
		}
		else
		{
			SYNTAX_ERROR;
		}
	}
	else
	{
		id++;
		return create_node(VAR , {.var_value = var_name}, NULL, NULL).arg.node;
	}


}

struct B_tree_node *get_pow()
{
	struct B_tree_node *val = get_par();
	CHECK_RET(val);

	while(	(tokens->data[id].type == OP) || (tokens->data[id].value.op_value == POW)	)
	{
		id++;

		struct B_tree_node *val_2 = get_par();

		val = create_node(OP, {.op_value = POW}, val, val_2).arg.node;
	}

	return val;
}
