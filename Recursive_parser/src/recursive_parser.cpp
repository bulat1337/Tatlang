#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#include "recursive_parser.h"
#include "recursive_parser_secondary.h"

#define SYNTAX_CHECK(cond)												\
	if(!(cond))															\
	{																	\
		PARSE_LOG("Syntax ERROR on %d: "#cond" is fasle\n", __LINE__);	\
		PARSE_LOG("%s returning NULL\n", __func__);						\
		return NULL;													\
	}

#define REPORT_ERROR(...)							\
	PARSE_LOG(__VA_ARGS__);							\
	PARSE_LOG("%s returning NULL\n", __func__);		\
	return NULL;

#define SYNTAX_ERROR									\
	REPORT_ERROR("syntax error on: %lu\n", __LINE__)	\
	PARSE_LOG("%s returning NULL\n", __func__);			\
	return NULL;

#define CHECK_RET(ptr)								\
	if(ptr == NULL)									\
	{												\
		PARSE_LOG("%s returning NULL\n", __func__);	\
		return NULL;								\
	}												\

#define DO_IF_TOKEN(token, op)														\
	if(!strncmp(var_name, token, sizeof(token) / sizeof(char))) 					\
	{																				\
		PARSE_LOG("Reserved token: %s\n", var_name);								\
																					\
		if(str[id] == '(')															\
		{																			\
			id++;																	\
			B_tree_node *child = get_add();											\
																					\
			if(str[id] == ')')														\
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

static char *str = NULL;
static size_t id = 0;

struct B_tree_node *parse_expr(const char *expression)
{
	str = skip_spaces(expression, strlen(expression));

	id = 0;

	struct B_tree_node *val = get_add();
	CHECK_RET(val);

	SYNTAX_CHECK(str[id] == '\0');

	return val;
}

struct B_tree_node *get_num()
{
	btr_elem_t val = 0;
	size_t old_id = id;
	bool after_dot = false;
	size_t after_dot_counter = 0;

	while(	('0' <= str[id] && str[id] <= '9') || (str[id] == '.')	)
	{
		if(str[id] == '.')
		{
			after_dot = true;
			id++;
		}
		else
		{
			if(after_dot)
			{
				after_dot_counter++;
			}

			val = val * 10 + (str[id] - '0');

			id++;
		}
	}

	val = val / (pow(10, after_dot_counter));

	SYNTAX_CHECK(id > old_id);

	return create_node(NUM, {.num_value = val}, NULL, NULL).arg.node;
}

struct B_tree_node *get_add()
{
	struct B_tree_node *val = get_mul();
	CHECK_RET(val);

	while(str[id] == '+' || str[id] == '-')
	{
		char op = str[id];

		id++;

		struct B_tree_node *val_2 = get_mul();
		CHECK_RET(val_2);

		switch(op)
		{
			case '+':
			{
				val = create_node(OP, {.op_value = ADD}, val, val_2).arg.node;
				break;
			}
			case '-':
			{
				val = create_node(OP, {.op_value = SUB}, val, val_2).arg.node;
				break;
			}
			default:
			{
				SYNTAX_CHECK(true);
			}
		}
	}

	return val;
}

struct B_tree_node *get_mul()
{
	struct B_tree_node *val = get_pow();
	CHECK_RET(val);

	while(str[id] == '*' || str[id] == '/')
	{
		char op = str[id];

		id++;

		struct B_tree_node *val_2 = get_pow();

		switch(op)
		{
			case '*':
			{
				val = create_node(OP, {.op_value = MUL}, val, val_2).arg.node;
				break;
			}
			case '/':
			{
				val = create_node(OP, {.op_value = DIV}, val, val_2).arg.node;
				break;
			}
			default:
			{
				SYNTAX_CHECK(true)
			}
		}
	}

	return val;
}

struct B_tree_node *get_par()
{
	if(str[id] == '(')
	{
		id++;
		struct B_tree_node *val = get_add();
		CHECK_RET(val);

		SYNTAX_CHECK(str[id] == ')');
		id++;
		return val;
	}
	else if('0' <= str[id] && str[id] <= '9')
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
	PARSE_LOG("%s log:\n", __func__);

	size_t sym_counter = count_symbols(str + id);

	char *var_name = (char *)calloc(sym_counter + 1, sizeof(char));
	if(var_name == NULL)
	{
		REPORT_ERROR("ERROR: allocation error\n");
	}

	memcpy(var_name, str + id, sym_counter);

	var_name[sym_counter] = '\0';

	id += sym_counter;
	PARSE_LOG("sym_counter = %lu\n", sym_counter);

	DO_IF_TOKEN("sin", SIN)
	DO_IF_TOKEN("cos", COS)
	DO_IF_TOKEN("sqrt", SQRT)
	DO_IF_TOKEN("ln", LN)
	else
	{
		PARSE_LOG("var_name: %s\n\n", var_name);

		return create_node(VAR , {.var_value = var_name}, NULL, NULL).arg.node;
	}
}

struct B_tree_node *get_pow()
{
	struct B_tree_node *val = get_par();
	CHECK_RET(val);

	while(str[id] == '^')
	{
		id++;

		struct B_tree_node *val_2 = get_par();

		val = create_node(OP, {.op_value = POW}, val, val_2).arg.node;
	}

	return val;
}
