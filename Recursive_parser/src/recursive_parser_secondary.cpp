#include <stdio.h>
#include <stdarg.h>

#include "recursive_parser_secondary.h"

static size_t sce_debt = 0;

#define FIRST_SYM_COND									\
	('a' <= *(cur_str) && *(cur_str) <= 'z') ||			\
	('A' <= *(cur_str) && *(cur_str) <= 'Z') ||			\
	(*(cur_str) == '_') ||								\
	(*(cur_str) == '$')

#define SYM_COND																\
	('a' <= *(cur_str + sym_counter) && *(cur_str + sym_counter) <= 'z') ||		\
	('A' <= *(cur_str + sym_counter) && *(cur_str + sym_counter) <= 'Z') ||		\
	('0' <= *(cur_str + sym_counter) && *(cur_str + sym_counter) <= '9') ||		\
	(*(cur_str + sym_counter) == '_') ||										\
	(*(cur_str + sym_counter) == '$')

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
	if(CUR_TYPE == OCBR)
	{
		id++;

		size_t scopes_sce_debt = take_debt();

		PARSE_LOG("There is scope.\n");

		B_tree_node *root = get_all_scopes(true, CCBR);

		root = manage_scs(root);

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
			return CR_SMC(cmd, NULL);
		}
	}
	else if(CUR_TYPE == FUNC)
	{
		PARSE_LOG("It's function.\n");

		cmd = get_func();
		CHECK_RET(cmd);


		if(CUR_TYPE == SMC)
		{
			id++;
			PARSE_LOG("SMC ok.\n");


			if(cmds_sce_debt)
			{
				B_tree_node *cmd_parent = pay_debt_cmd(cmd, cmds_sce_debt);

				return cmd_parent;
			}
			else
			{
				return CR_SMC(cmd, NULL);
			}
		}
		else
		{
			SYNTAX_ERROR;
		}
	}
	else
	{
		PARSE_LOG("Getting assignment.\n");

		cmd = get_ass();
		CHECK_RET(cmd);

		if(CUR_TYPE == SMC)
		{
			id++;
			PARSE_LOG("SMC ok.\n");


			if(cmds_sce_debt)
			{
				B_tree_node *cmd_parent = pay_debt_cmd(cmd, cmds_sce_debt);

				return cmd_parent;
			}
			else
			{
				return CR_SMC(cmd, NULL);
			}
		}
		else
		{
			SYNTAX_ERROR;
		}
	}
}

B_tree_node *get_func()
{
	char *func_id = CUR_VAR;
	id++;

	if(CUR_TYPE == OBR)
	{
		PARSE_LOG("OBR ok.\n");
		id++;
	}
	else
	{
		SYNTAX_ERROR;
	}

	B_tree_node *child = NULL;

	if(IS_KWD(func_id, "getvar"))
	{
		PARSE_LOG("Getting brace var.\n");
		child = get_id();
	}
	else if(IS_KWD(func_id, "putexpr"))
	{
		PARSE_LOG("Getting brace expression.\n");
		child = get_add();
	}
	else
	{
		printf("%s.\n", func_id);
		SYNTAX_ERROR;
	}


	CHECK_RET(child);

	if(CUR_TYPE == CBR)
	{
		PARSE_LOG("CBR ok\n");
		id++;

		return CR_FUNC(func_id, NULL, child);
	}
	else
	{
		SYNTAX_ERROR;
	}
}

B_tree_node *get_cond(Node_type type)
{
	if(CUR_TYPE == OBR)
	{
		PARSE_LOG("OBR ok.\n");
		id++;

		PARSE_LOG("Getting brace expression.\n");
		B_tree_node *br_expr = get_add();
		CHECK_RET(br_expr);

		if(CUR_TYPE == CBR)
		{
			PARSE_LOG("CBR ok\n");
			id++;

			B_tree_node *scope = get_scope();

			return CR_COND(type, br_expr, scope);
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

B_tree_node *get_ass()
{
	B_tree_node *var = get_id();
	CHECK_RET(var);

	if(CUR_TYPE == OP && CUR_OP == ASS)
	{
		id++;
	}
	else
	{
		SYNTAX_ERROR;
	}

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
	if(CUR_TYPE == OBR)
	{
		id++;
		B_tree_node *val = get_add();
		CHECK_RET(val);

		SYNTAX_CHECK(CUR_TYPE == CBR);
		id++;

		return val;
	}
	else if(CUR_TYPE == NUM)
	{
		B_tree_node *val = get_num();
		CHECK_RET(val);

		return val;
	}
	else if(CUR_TYPE == UN_OP)
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
	else if(IS_KWD(op_name, name))			\
	{										\
		return CR_UN_OP(op, NULL, child);	\
	}										\

B_tree_node *get_unary()
{
	PARSE_LOG("%s log:\n", __func__);

	char *op_name = CUR_VAR;

	PARSE_LOG("Unary opertaion name: %s.\n", op_name);

	id++;

	if(CUR_TYPE == OBR)
	{
		PARSE_LOG("OBR ok\n");

		id++;
		B_tree_node *child = get_add();
		CHECK_RET(child);

		if(CUR_TYPE == CBR)
		{
			PARSE_LOG("CBR ok\n");

			id++;
			if(false)
			{
				;
			}
			OP_CASE(SIN, "sin")
			OP_CASE(COS, "cos")
			OP_CASE(LN,  "ln")
			OP_CASE(SQRT, "sqrt")
			else
			{
				REPORT_ERROR("Uknown operation.\n");
			}

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

		cur_node = scope_end->right;
	}

	if(manage_ccbrs)
	{
		sce_debt++;
		PARSE_LOG("CCBR for scope ok.\n");
		id++;
	}

	return root;
}

B_tree_node *manage_scs(B_tree_node *root)
{
	if(root->type == SCS)
	{
		root = CR_SCS(NULL, root);
	}
	else
	{
		root->type = SCS;
	}

	return root;
}

B_tree_node *pay_debt_scope(B_tree_node *root, size_t scopes_sce_debt)
{
	for(size_t debt_id = 0; debt_id < scopes_sce_debt; debt_id++)
	{
		root = CR_SCE(NULL, root);
		PARSE_LOG("Scope is paying debt.\n");
	}

	return root;
}

B_tree_node *pay_debt_cmd(B_tree_node *cmd, size_t cmds_sce_debt)
{
	B_tree_node *cmd_parent = CR_SCE(NULL, NULL);
	B_tree_node *cur_node = cmd_parent;

	for(size_t debt_id = 0; debt_id < cmds_sce_debt - 1; debt_id++)
	{
		cur_node->right = CR_SCE(NULL, NULL);
		cur_node = cur_node->right;
	}

	cur_node->left = cmd;

	return cmd_parent;
}


#undef SYM_COND
#undef FIRST_SYM_COND
