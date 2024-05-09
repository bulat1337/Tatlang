#include <stdio.h>
#include <stdarg.h>
#include <string.h>

#include "backend_secondary.h"

#define CUR_LVL\
	nm_tbl_mngr->cur_lvl

#define CUR_TABLE\
	nm_tbl_mngr->name_tables[CUR_LVL]

#define CHECK_ERROR										\
	if(error_code != BKD_ALL_GOOD)						\
	{													\
		LOG("%s: ERROR: %d\n", __func__, error_code);	\
		return BKD_ALL_GOOD;							\
	}

#define IS_FUNC(kwd)\
	!strncmp(node->value.var_value, kwd, MAX_TOKEN_SIZE)

static size_t label_ct = 0;

bkd_err_t asmbl(B_tree_node *node, FILE *asm_file, Nm_tbl_mngr *nm_tbl_mngr)
{
	bkd_err_t error_code = BKD_ALL_GOOD;

	if(node == NULL)
	{
		return error_code;
	}

	switch(node->type)
	{
		case SCOPE_START:
		{
			CALL(upgrade_n_table(nm_tbl_mngr));
			goto asmbl_children;
		}
		case SCOPE_END:
		{
			CALL(downgrade_n_table(nm_tbl_mngr));
			goto asmbl_children;
		}
		case SEMICOLON:
		{
			asmbl_children:

			ASMBL(node->left);
			ASMBL(node->right);

			break;
		}
		case WHILE:
		{
			CALL(write_while(node, asm_file, nm_tbl_mngr));

			break;
		}
		case IF:
		{
			CALL(write_if(node, asm_file, nm_tbl_mngr));

			break;
		}
		case STD_FUNC:
		{
			switch(node->value.func)
			{
				case GETVAR:
				{
					write_getvar(node, asm_file, nm_tbl_mngr);
					break;
				}
				case PUTEXPR:
				{
					write_putexpr(node, asm_file, nm_tbl_mngr);
					break;
				}
				default:
				{
					LOG("%s: ERROR:\n\tUnknown func.\n", __func__);

					return BKD_UKNOWN_FUNC;
				}
			}

			break;
		}
		case FUNC:
		case CMD_FUNC:
		{
			CALL(write_func(node, asm_file, nm_tbl_mngr));

			break;
		}
		case FUNC_DECL:
		{
			CALL(write_func_decl(node, asm_file));

			break;
		}
		case RETURN:
		{
			CALL(write_return(node, asm_file, nm_tbl_mngr));

			break;
		}
		case MAIN:
		{
			CALL(write_main(node, asm_file));

			break;
		}
		case OP:
		{
			CALL(write_op(node, asm_file, nm_tbl_mngr));

			break;
		}
		case UNR_OP:
		{
			CALL(write_op(node, asm_file, nm_tbl_mngr));

			break;
		}
		case VAR:
		{
			CALL(write_var(node->value.var_value, asm_file, nm_tbl_mngr));

			break;
		}
		case NUM:
		{
			write_num(node->value.num_value, asm_file);

			break;
		}
		case ABOVE:
		case BELOW:
		case ABOVE_EQUAL:
		case BELOW_EQUAL:
		case EQUAL:
		case NOT_EQUAL:
		{
			write_cond_expr(node, asm_file, nm_tbl_mngr);

			break;
		}
		case DECLARE:
		case COMMA:
		case KEYWORD:
		case OPEN_BR:
		case CLOSE_BR:
		case OPEN_CBR:
		case CLOSE_CBR:
		case END:
		{
			LOG("%s: ERROR:\n\tInvalid node in the language tree: %d.\n", __func__, node->type);

			return BKD_INVALID_NODE;
		}
		default:
		{
			LOG("%s: ERROR:\n\tUnknown type: %d\n", __func__, node->type);

			return BKD_UNKNWON_TYPE;
		}
	}

	return error_code;
}

bkd_err_t write_cond_expr(B_tree_node *node, FILE *asm_file, Nm_tbl_mngr *nm_tbl_mngr)
{
	bkd_err_t error_code = BKD_ALL_GOOD;
	const char *cond = get_cond_type(node->type);
	if(cond == NULL)
	{
		return BKD_INVALID_COND_EXPR;
	}

	WRITE_ASM("push 0\n");

	ASMBL(node->right);
	ASMBL(node->left);

	WRITE_ASM("%s break_%lu\n", cond, label_ct);
	WRITE_ASM("push 1\n");
	WRITE_ASM("add\n");
	WRITE_ASM(":break_%lu\n", label_ct++);
	WRITE_ASM("push 0\n");
	WRITE_ASM("add\n");

	return error_code;
}

const char *get_cond_type(Node_type type)
{
	switch(type)
	{
		case ABOVE:
		{
			return "jbe";
		}
		case BELOW:
		{
			return "jae";
		}
		case ABOVE_EQUAL:
		{
			return "jb";
		}
		case BELOW_EQUAL:
		{
			return "ja";
		}
		case EQUAL:
		{
			return "jne";
		}
		case NOT_EQUAL:
		{
			return "jne";
		}
		default:
		{
			LOG("%s: ERROR:\n\tInvalid cond expr.\n", __func__);
			return NULL;
		}
	}
}

bkd_err_t write_func(B_tree_node *node, FILE *asm_file, Nm_tbl_mngr *nm_tbl_mngr)
{
	bkd_err_t error_code = BKD_ALL_GOOD;

	CALL(push_all(nm_tbl_mngr, asm_file));

	B_tree_node *cur_node = node->left;


	size_t arg_counter = 0;

	while(cur_node != NULL)
	{
		ASMBL(cur_node->left);

		arg_counter++;

		cur_node = cur_node->right;
	}

	for(size_t arg_id = arg_counter; arg_id >= 1; arg_id--)
	{
		char *loc = get_loc_in_order(arg_id, &error_code);
		WRITE_ASM("pop %s\n", loc);
	}

	WRITE_ASM("call %ls\n", node->value.var_value);

	// pops all exept return register (wich is rax)
	CALL(pop_all(nm_tbl_mngr, asm_file));

	if(CUR_LVL == 0 && CUR_TABLE.size == 0)
	{
		if(node->type == FUNC)
		{
			WRITE_ASM("push rax\n");
		}
	}
	else
	{
		CALL(save_ret_reg(asm_file, node->type, nm_tbl_mngr));
	}


	return error_code;
}

bkd_err_t save_ret_reg(FILE *asm_file, Node_type type, Nm_tbl_mngr *nm_tbl_mngr)
{
	bkd_err_t error_code = BKD_ALL_GOOD;

	if(type == FUNC)
	{
		char *exch_reg = get_loc(L"_temp_exch", nm_tbl_mngr, true, &error_code);
		if(error_code != BKD_ALL_GOOD)
		{
			return error_code;
		}

		WRITE_ASM("pop %s\n", exch_reg);
		WRITE_ASM("push rax\n");
		WRITE_ASM("push %s\n", exch_reg);
		WRITE_ASM("pop rax\n");

		//deinit exch
	}
	else
	{
		WRITE_ASM("pop rax\n");
	}

	return error_code;
}



bkd_err_t write_getvar(B_tree_node *node, FILE *asm_file, Nm_tbl_mngr *nm_tbl_mngr)
{
	bkd_err_t error_code = BKD_ALL_GOOD;

	WRITE_ASM("in\n");

	char *loc = get_loc(node->right->value.var_value, nm_tbl_mngr, true, &error_code);

	if(error_code != BKD_ALL_GOOD)
	{
		LOG("%s: ERROR:\n\tget_loc error: %d.\n", __func__, error_code);

		return error_code;
	}

	WRITE_ASM("pop %s\n", loc);

	return error_code;
}

bkd_err_t write_putexpr(B_tree_node *node, FILE *asm_file, Nm_tbl_mngr *nm_tbl_mngr)
{
	bkd_err_t error_code = BKD_ALL_GOOD;

	ASMBL(node->right);
	WRITE_ASM("out\n");

	return error_code;
}

void write_num(double num, FILE *asm_file)
{
	WRITE_ASM("push %lf\n", num);
}

bkd_err_t write_var(wchar_t *var, FILE *asm_file, Nm_tbl_mngr *nm_tbl_mngr)
{
	bkd_err_t error_code = BKD_ALL_GOOD;

	char *loc = get_loc(var, nm_tbl_mngr, false, &error_code);

	if(error_code != BKD_ALL_GOOD)
	{
		LOG("%s: ERROR:\n\tUnknown var.\n", __func__);
		return error_code;
	}

	WRITE_ASM("push %s\n", loc);

	return error_code;
}

#define CASE(op, name)			\
	case op:					\
	{							\
		ASMBL(node->left);		\
		ASMBL(node->right);		\
		WRITE_ASM(name"\n");	\
								\
		break;					\
	}							\

bkd_err_t write_op(B_tree_node *node, FILE *asm_file, Nm_tbl_mngr *nm_tbl_mngr)
{
	bkd_err_t error_code = BKD_ALL_GOOD;

	switch(node->value.op_value)
	{
		case ASS:
		{
			ASMBL(node->right);

			char *loc = get_loc(node->left->value.var_value, nm_tbl_mngr, true, &error_code);

			if(error_code != BKD_ALL_GOOD)
			{
				LOG("%s: ERROR:\n\tget_loc error: %d.\n", __func__, error_code);

				return error_code;
			}


			WRITE_ASM("pop %s\n", loc);
			// WRITE_ASM("pop [temp_loc for %s]\n", node->left->value.var_value);

			break;
		}
		CASE(ADD,  "add")
		CASE(SUB,  "sub")
		CASE(MUL,  "mul")
		CASE(DIV,  "div")
		CASE(POW,  "pow")
		CASE(LN,   "ln")
		CASE(SIN,  "sin")
		CASE(COS,  "cos")
		CASE(SQRT, "sqrt")
		case DO_NOTHING:
		{
			LOG("%s: ERROR:\n\tInvalid operation: %d\n", __func__, node->value.op_value);
			return BKD_UNKNOWN_OPERATION;
		}
		default:
		{
			LOG("%s: ERROR:\n\tUknown operation.\n", __func__);
			return BKD_UNKNOWN_OPERATION;
		}
	}

	return error_code;
}

#undef CASE

bkd_err_t write_while(B_tree_node *node, FILE *asm_file, Nm_tbl_mngr *nm_tbl_mngr)
{
	bkd_err_t error_code = BKD_ALL_GOOD;

	WRITE_ASM(":while_%lu\n", label_ct);
	size_t while_id = label_ct;
	label_ct++;

	ASMBL(node->left);

	WRITE_ASM("push 0\n");
	WRITE_ASM("je break_%lu\n", while_id);

	ASMBL(node->right);

	WRITE_ASM("jmp while_%lu\n", while_id);
	WRITE_ASM(":break_%lu\n", while_id);

	return error_code;
}

bkd_err_t write_main(B_tree_node *node, FILE *asm_file)
{
	bkd_err_t error_code = BKD_ALL_GOOD;

	Nm_tbl_mngr nm_tbl_mngr = {};
	init_name_tables(&nm_tbl_mngr);
	nm_tbl_mngr.in_func_start = true;

	WRITE_ASM(":main\n")

	asmbl(node->right, asm_file, &nm_tbl_mngr);

	return error_code;
}

bkd_err_t write_if(B_tree_node *node, FILE *asm_file, Nm_tbl_mngr *nm_tbl_mngr)
{
	bkd_err_t error_code = BKD_ALL_GOOD;

	ASMBL(node->left);

	WRITE_ASM("push 0\n");
	WRITE_ASM("je break_%lu\n", label_ct);

	ASMBL(node->right);

	WRITE_ASM(":break_%lu\n", label_ct);
	label_ct++;

	return error_code;
}



void bkd_write_log(const char *file_name, const char *fmt, ...)
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

bkd_err_t init_name_tables(Nm_tbl_mngr *nm_tbl_mngr)
{
	CUR_LVL = 0;

	CALLOC(nm_tbl_mngr->name_tables, CUR_LVL + 1, Name_table);

	CUR_TABLE.capacity = ST_CELLS_AMOUNT;
	CUR_TABLE.size     = 0;
	CALLOC(CUR_TABLE.cells, ST_CELLS_AMOUNT, Table_cell);

	LOG("Manager inited.\n");

	return BKD_ALL_GOOD;
}

bkd_err_t upgrade_n_table(Nm_tbl_mngr *nm_tbl_mngr)
{
	LOG("Upgrade in progress.\n");

	bkd_err_t error_code = BKD_ALL_GOOD;

	if(nm_tbl_mngr->in_func_start == true)
	{
		LOG("Manager in func start. Not upgrating.\n");

		nm_tbl_mngr->in_func_start = false;
		return error_code;
	}

	CUR_LVL++;

	REALLOC(nm_tbl_mngr->name_tables, CUR_LVL + 1, Name_table);

	CUR_TABLE.capacity = ST_CELLS_AMOUNT;
	CUR_TABLE.size     = 0;

	CALLOC((CUR_TABLE).cells, ST_CELLS_AMOUNT, Table_cell);

	LOG("Successful upgrade.\n");

	return error_code;
}

bkd_err_t downgrade_n_table(Nm_tbl_mngr *nm_tbl_mngr)
{
	LOG("Downgrade in progress.\n");
	bkd_err_t error_code = BKD_ALL_GOOD;

	if(CUR_LVL == 0)
	{
		LOG("Trying to downgrade 0 lvl manager. Destroying it.\n");
		CALL(dtor_name_tables(nm_tbl_mngr));

		return error_code;
	}

	CUR_TABLE.capacity = 0;
	CUR_TABLE.size     = 0;
	free(CUR_TABLE.cells);
	CUR_TABLE.cells = NULL;

	CUR_LVL--;

	REALLOC(nm_tbl_mngr->name_tables, CUR_LVL + 1, Name_table);

	LOG("Successful downgrade.\n");

	return error_code;
}

bkd_err_t dtor_name_tables(Nm_tbl_mngr *nm_tbl_mngr)
{
	while(CUR_LVL)
	{
		free(CUR_TABLE.cells);

		CUR_TABLE.cells = NULL;
		CUR_TABLE.capacity  = 0;
		CUR_TABLE.size      = 0;

		CUR_LVL--;
	}

	free(nm_tbl_mngr->name_tables);
	nm_tbl_mngr->name_tables = NULL;

	nm_tbl_mngr = NULL;

	LOG("Manager dtored.\n");

	return BKD_ALL_GOOD;
}

#define IS_VAR(table_name)\
	!wcsncmp(table_name, var, MAX_TOKEN_SIZE)

#define NAME_TABLE\
	nm_tbl_mngr->name_tables[lvl_id]

#undef ALLOCATION_CHECK

#define ALLOCATION_CHECK(ptr)					\
	if(ptr == NULL)								\
	{											\
		LOG("Unable to allocate"#ptr".\n");		\
		*error_code = BKD_UNABLE_TO_ALLOCATE;	\
		return NULL;							\
	}

char *get_loc(wchar_t *var, Nm_tbl_mngr *nm_tbl_mngr, bool init_flag, bkd_err_t *error_code)
{
	size_t overall_size = 0;

	for(long lvl_id = (long)CUR_LVL; lvl_id >= 0; lvl_id--)
	{
		overall_size += NAME_TABLE.size;

		for(size_t cell_id = 0; cell_id < NAME_TABLE.size; cell_id++)
		{
			if(IS_VAR(NAME_TABLE.cells[cell_id].name))
			{
				char *loc = get_init_var(&(NAME_TABLE.cells[cell_id]), error_code);

				return loc;
			}
		}
	}


	if(init_flag)
	{
		char *loc = init_var(var, &(CUR_TABLE), error_code, overall_size);

		return loc;
	}
	else
	{
		return NULL;
	}
}

char *get_loc_in_order(size_t arg_counter, bkd_err_t *error_code)
{
	char *loc = NULL;
	CALLOC(loc, LOC_SIZE, char);

	if(arg_counter >= AMOUNT_OF_REGS)
	{
		snprintf(loc, LOC_SIZE - 1, "[%lu]", arg_counter - AMOUNT_OF_REGS);
	}
	else
	{
		snprintf(loc, LOC_SIZE - 1, "r%cx", (char)arg_counter + 'a');
	}

	return loc;
}

char *get_init_var(Table_cell *cell, bkd_err_t *error_code)
{
	char *loc = NULL;
	CALLOC(loc, LOC_SIZE, char);

	switch(cell->type)
	{
		case REG:
		{
			snprintf(loc, LOC_SIZE - 1, "r%cx", cell->loc.reg_id + 'a');

			return loc;
		}
		case RAM:
		{
			snprintf(loc, LOC_SIZE - 1, "[%lu]", cell->loc.RAM_address);

			return loc;
		}
		default:
		{
			free(loc);
			*error_code = BKD_UNKNOWN_VAR;
			return NULL;
		}
	}
}

char *init_var(wchar_t *var, struct Name_table *cur_table,
			   bkd_err_t *error_code, size_t overall_size)
{
	char *loc = NULL;
	CALLOC(loc, LOC_SIZE, char);

	if(cur_table->size >= cur_table->capacity)
	{
		cur_table->capacity *= REALLOC_COEFF;
		REALLOC(cur_table->cells, cur_table->capacity, Table_cell)
		LOG("Cells reallocated.\n");
	}

	cur_table->cells[cur_table->size].name = var;

	if(overall_size >= AMOUNT_OF_REGS)
	{
		cur_table->cells[cur_table->size].type            = RAM;
		cur_table->cells[cur_table->size].loc.RAM_address = overall_size - AMOUNT_OF_REGS;

		snprintf(loc, LOC_SIZE - 1, "[%lu]",
					cur_table->cells[cur_table->size].loc.RAM_address);
	}
	else
	{
		cur_table->cells[cur_table->size].type       = REG;
		cur_table->cells[cur_table->size].loc.reg_id = (unsigned char)overall_size;

		snprintf(loc, LOC_SIZE - 1, "r%cx",
					cur_table->cells[cur_table->size].loc.reg_id + 'a');
	}

	cur_table->size++;


	return loc;
}

bkd_err_t push_all(Nm_tbl_mngr *nm_tbl_mngr, FILE *asm_file)
{
	bkd_err_t error_code = BKD_ALL_GOOD;

	for(long lvl_id = (long)CUR_LVL; lvl_id >= 0; lvl_id--)
	{
		for(size_t cell_id = 0; cell_id < NAME_TABLE.size; cell_id++)
		{
			char *loc = get_init_var(&(NAME_TABLE.cells[cell_id]), &error_code);

			WRITE_ASM("push %s\n", loc);
		}
	}

	return error_code;
}

bkd_err_t pop_all(Nm_tbl_mngr *nm_tbl_mngr, FILE *asm_file)
{
	bkd_err_t error_code = BKD_ALL_GOOD;

	for(long lvl_id = 0; lvl_id <= (long)CUR_LVL; lvl_id++)
	{
		for(long cell_id = (long)NAME_TABLE.size - 1; cell_id >= 0; cell_id--)
		{
			char *loc = get_init_var(&(NAME_TABLE.cells[cell_id]), &error_code);

			if(!(cell_id == 0 && lvl_id == 0))
			{
				WRITE_ASM("pop %s\n", loc);
			}
		}
	}

	return  error_code;
}

#undef NAME_TABLE
#undef IS_VAR

#undef CUR_TABLE
#undef CUR_LVL


#define CUR_LVL\
	nm_tbl_mngr.cur_lvl

#define CUR_TABLE\
	nm_tbl_mngr.name_tables[CUR_LVL]


bkd_err_t write_func_decl(B_tree_node *node, FILE *asm_file)
{
	bkd_err_t error_code = BKD_ALL_GOOD;


	Nm_tbl_mngr nm_tbl_mngr = {};
	init_name_tables(&nm_tbl_mngr);
	nm_tbl_mngr.in_func_start = true;

	B_tree_node *cur_node = node->left;

	init_var(L"_ret_var", &(CUR_TABLE), &error_code, 0);
	size_t arg_counter = 1;


	while(cur_node != NULL)
	{
		init_var(cur_node->left->value.var_value, &(CUR_TABLE), &error_code, arg_counter);
		arg_counter++;

		cur_node = cur_node->right;
	}

	WRITE_ASM(":%ls\n", node->value.var_value);

	asmbl(node->right, asm_file, &nm_tbl_mngr);

	WRITE_ASM("ret\n");

	return error_code;
}

bkd_err_t write_return(B_tree_node *node, FILE *asm_file, Nm_tbl_mngr *nm_tbl_mngr)
{
	bkd_err_t error_code = BKD_ALL_GOOD;

	ASMBL(node->right);

	WRITE_ASM("pop rax\n");

	WRITE_ASM("ret\n");

	return error_code;
}
