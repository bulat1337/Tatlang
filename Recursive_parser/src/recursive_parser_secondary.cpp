#include <stdio.h>
#include <stdarg.h>

#include "recursive_parser_secondary.h"

static size_t sce_debt = 0;

void rec_write_log(const char *file_name, const char *fmt, ...)
{
    static FILE *log_file = fopen(file_name, "w");

    if (log_file == NULL)
	{
        perror("Error opening log_file");
        return;
    }

    va_list args = NULL;

    va_start(args, fmt);

    vfprintf(log_file, fmt, args);

    va_end(args);
}

B_tree_node *get_scope()
{
	if(CUR_TYPE == OPEN_CBR)
	{
		id++;

		size_t scopes_sce_debt = take_debt();

		PARSE_LOG("There is scope.\n");

		B_tree_node *root = get_all_scopes(true, CLOSE_CBR);
		CHECK_RET(root);

		root = manage_scopes(root);

		if(scopes_sce_debt)
		{
			root = pay_debt_scope(root, scopes_sce_debt);
		}

		return root;
	}
	else
	{
		PARSE_LOG("Getting command.\n");

		B_tree_node *root = get_cmd();
		CHECK_RET(root);

		return root;
	}
}

B_tree_node *get_cmd()
{
	B_tree_node *cmd = NULL;

	size_t cmds_sce_debt = take_debt();

	if(CUR_TYPE == IF || CUR_TYPE == WHILE)
	{
		Node_type cond_type = CUR_TYPE;
		PARSE_LOG("It's 'if' or 'while'.\n");
		id++;

		cmd = get_cond(cond_type);
		CHECK_RET(cmd);

		if(cmds_sce_debt)
		{
			B_tree_node *cmd_parent = pay_debt_cmd(cmd, cmds_sce_debt);

			return cmd_parent;
		}
		else
		{
			return CR_SEMICOLON(cmd, NULL);
		}
	}
	else if(CUR_TYPE == STD_FUNC)
	{
		PARSE_LOG("It's standart function.\n");

		cmd = get_func();
		CHECK_RET(cmd);

		SYNTAX_CHECK(CUR_TYPE == SEMICOLON);

		if(cmds_sce_debt)
		{
			B_tree_node *cmd_parent = pay_debt_cmd(cmd, cmds_sce_debt);

			return cmd_parent;
		}
		else
		{
			return CR_SEMICOLON(cmd, NULL);
		}
	}
	else
	{
		PARSE_LOG("Getting assignment.\n");

		cmd = get_ass();
		CHECK_RET(cmd);

		SYNTAX_CHECK(CUR_TYPE == SEMICOLON);

		if(cmds_sce_debt)
		{
			B_tree_node *cmd_parent = pay_debt_cmd(cmd, cmds_sce_debt);

			return cmd_parent;
		}
		else
		{
			return CR_SEMICOLON(cmd, NULL);
		}

	}
}

B_tree_node *get_func()
{
	Std_func func_type = CUR_STD_FUNC;
	id++;

	SYNTAX_CHECK(CUR_TYPE == OPEN_BR);

	B_tree_node *child = NULL;

	switch(func_type)
	{
		case GETVAR:
		{
			PARSE_LOG("Getting brace var.\n");

			child = get_id();
			CHECK_RET(child);

			break;
		}
		case PUTEXPR:
		{
			PARSE_LOG("Getting brace expression.\n");

			child = get_add();
			CHECK_RET(child);

			break;
		}
		default:
		{
			SYNTAX_ERROR;
		}
	}


	CHECK_RET(child);

	SYNTAX_CHECK(CUR_TYPE == CLOSE_BR);

	return CR_STD_FUNC(func_type, NULL, child);
}

B_tree_node *get_cond(Node_type type)
{

	SYNTAX_CHECK(CUR_TYPE == OPEN_BR);

	PARSE_LOG("Getting brace expression.\n");
	B_tree_node *br_expr = get_add();
	CHECK_RET(br_expr);

	SYNTAX_CHECK(CUR_TYPE == CLOSE_BR);

	B_tree_node *scope = get_scope();
	CHECK_RET(scope);

	return CR_COND(type, br_expr, scope);
}

B_tree_node *get_ass()
{
	B_tree_node *var = get_id();
	CHECK_RET(var);

	SYNTAX_CHECK(CUR_TYPE == OP && CUR_OP == ASS);

	B_tree_node *expr = get_add();
	CHECK_RET(expr);

	return CR_ASS(var, expr);
}

B_tree_node *get_num()
{
	btr_elem_t val = CUR_NUM;

	PARSE_LOG("It's num: %lf\n", val);

	id++;

	return CR_NUM(val, NULL, NULL);
}

B_tree_node *get_add()
{
	B_tree_node *val = get_mul();
	CHECK_RET(val);

	while(	CUR_TYPE == OP &&
			(	CUR_OP == ADD ||
				CUR_OP == SUB	)	)
	{
		PARSE_LOG("It's ADD or SUB.\n");
		Ops op = CUR_OP;

		id++;

		B_tree_node *val_2 = get_mul();
		CHECK_RET(val_2);

		val = CR_OP(op, val, val_2);
	}

	return val;
}

B_tree_node *get_mul()
{
	B_tree_node *val = get_pow();
	CHECK_RET(val);

	while(	CUR_TYPE == OP &&
			(	CUR_OP == MUL ||
				CUR_OP == DIV	)	)
	{
		PARSE_LOG("It's MUL or DIV.\n");
		Ops op = CUR_OP;

		id++;

		B_tree_node *val_2 = get_pow();
		CHECK_RET(val_2);

		val = CR_OP(op, val, val_2);
	}

	return val;
}

B_tree_node *get_par()
{
	if(CUR_TYPE == OPEN_BR)
	{
		id++;
		B_tree_node *val = get_add();
		CHECK_RET(val);

		SYNTAX_CHECK(CUR_TYPE == CLOSE_BR);

		return val;
	}
	else if(CUR_TYPE == NUM)
	{
		B_tree_node *val = get_num();
		CHECK_RET(val);

		return val;
	}
	else if(CUR_TYPE == UNR_OP)
	{
		B_tree_node *val = get_unary();
		CHECK_RET(val);

		return val;
	}
	else
	{
		B_tree_node *val = get_id();
		CHECK_RET(val);

		return val;
	}
}

#define OP_CASE(op, name)					\
	else if(IS_KEYWORD(op_name, name))		\
	{										\
		return CR_UNR_OP(op, NULL, child);	\
	}										\

B_tree_node *get_unary()
{
	PARSE_LOG("%s log:\n", __func__);

	Ops operation = CUR_OP;

	PARSE_LOG("Unary opertaion: %d.\n", operation);

	id++;

	SYNTAX_CHECK(CUR_TYPE == OPEN_BR);

	B_tree_node *child = get_add();
	CHECK_RET(child);

	SYNTAX_CHECK(CUR_TYPE == CLOSE_BR);

	return CR_UNR_OP(operation, NULL, child);
}

B_tree_node *get_id()
{
	PARSE_LOG("%s log:\n", __func__);

	char *var_name = CUR_VAR;

	PARSE_LOG("Variable name: %s.\n", var_name);

	id++;
	return CR_VAR(var_name, NULL, NULL);
}

B_tree_node *get_pow()
{
	B_tree_node *val = get_par();
	CHECK_RET(val);

	while(	(CUR_TYPE == OP) && (CUR_OP == POW)	)
	{
		id++;

		B_tree_node *val_2 = get_par();
		CHECK_RET(val_2);

		val = CR_OP(POW, val, val_2);
	}

	return val;
}

B_tree_node *move_scope_end(B_tree_node *root)
{
	if(root == NULL)
	{
		return root;
	}
	while(root->right != NULL)
	{
		root = root->right;
	}

	return root;
}

size_t take_debt()
{
	size_t scopes_sce_debt = sce_debt;
	sce_debt = 0;

	return scopes_sce_debt;
}

B_tree_node *get_all_scopes(bool manage_ccbrs, Node_type end_type)
{
	PARSE_LOG("Getting scopes.\n");

	B_tree_node *root = get_scope();
	CHECK_RET(root);

	B_tree_node *cur_node = root;

	while(CUR_TYPE != end_type)
	{
		B_tree_node *scope_end = move_scope_end(cur_node);

		scope_end->right = get_scope();
		CHECK_RET(scope_end->right);

		cur_node = scope_end->right;
	}

	if(manage_ccbrs)
	{
		sce_debt++;
		PARSE_LOG("CLOSE_CBR for scope ok.\n");
		id++;
	}

	return root;
}

B_tree_node *manage_scopes(B_tree_node *root)
{
	if(root->type == SCOPE_START)
	{
		root = CR_SCOPE_START(NULL, root);
	}
	else
	{
		root->type = SCOPE_START;
	}

	return root;
}

B_tree_node *pay_debt_scope(B_tree_node *root, size_t scopes_sce_debt)
{
	for(size_t debt_id = 0; debt_id < scopes_sce_debt; debt_id++)
	{
		root = CR_SCOPE_END(NULL, root);
		PARSE_LOG("Scope is paying debt.\n");
	}

	return root;
}

B_tree_node *pay_debt_cmd(B_tree_node *cmd, size_t cmds_sce_debt)
{
	B_tree_node *cmd_parent = CR_SCOPE_END(NULL, NULL);
	B_tree_node *cur_node = cmd_parent;

	for(size_t debt_id = 0; debt_id < cmds_sce_debt - 1; debt_id++)
	{
		cur_node->right = CR_SCOPE_END(NULL, NULL);
		cur_node = cur_node->right;
	}

	cur_node->left = cmd;

	return cmd_parent;
}


#undef SYM_COND
#undef FIRST_SYM_COND
