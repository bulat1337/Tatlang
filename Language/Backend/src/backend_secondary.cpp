#include <stdio.h>
#include <stdarg.h>
#include <string.h>

#include "backend_secondary.h"

#define CHECK_ERROR										\
	if(error_code != BKD_ALL_GOOD)						\
	{													\
		LOG("%s: ERROR: %d\n", __func__, error_code);	\
		return BKD_ALL_GOOD;							\
	}

#define IS_FUNC(kwd)\
	!strncmp(node->value.var_value, kwd, LEN(kwd))

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

bkd_err_t write_var(char *var, FILE *asm_file, Nm_tbl_mngr *nm_tbl_mngr)
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
			LOG("%s: ERROR:\n\tInvalid operation.\n", __func__);
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

#define CUR_LVL\
	nm_tbl_mngr->cur_lvl

#define CUR_TABLE\
	nm_tbl_mngr->name_tables[CUR_LVL]

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

	CUR_LVL++;

	REALLOC(nm_tbl_mngr->name_tables, CUR_LVL + 1, Name_table);

	CUR_TABLE.capacity = ST_CELLS_AMOUNT;
	CUR_TABLE.size     = 0;

	CALLOC((CUR_TABLE).cells, ST_CELLS_AMOUNT, Table_cell);

	LOG("Successful upgrade.\n");

	return BKD_ALL_GOOD;
}

bkd_err_t downgrade_n_table(Nm_tbl_mngr *nm_tbl_mngr)
{
	LOG("Downgrade in progress.\n");

	CUR_TABLE.capacity = 0;
	CUR_TABLE.size     = 0;
	free(CUR_TABLE.cells);
	CUR_TABLE.cells = NULL;

	CUR_LVL--;

	REALLOC(nm_tbl_mngr->name_tables, CUR_LVL + 1, Name_table);

	LOG("Successful downgrade.\n");

	return BKD_ALL_GOOD;
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
	!strncmp(table_name, var, strlen(var))

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

char *get_loc(char *var, Nm_tbl_mngr *nm_tbl_mngr, bool init_flag, bkd_err_t *error_code)
{
	char *loc = NULL;
	CALLOC(loc, LOC_SIZE, char);

	size_t overall_size = 0;

	for(long lvl_id = (long)CUR_LVL; lvl_id >= 0; lvl_id--)
	{
		overall_size += NAME_TABLE.size;

		for(size_t cell_id = 0; cell_id < NAME_TABLE.size; cell_id++)
		{
			if(IS_VAR(NAME_TABLE.cells[cell_id].name))
			{

				switch(NAME_TABLE.cells[cell_id].type)
				{
					case REG:
					{
						snprintf(loc, LOC_SIZE - 1, "r%cx",
								 NAME_TABLE.cells[cell_id].loc.reg_id + 'a');

						return loc;
					}
					case RAM:
					{
						snprintf(loc, LOC_SIZE - 1, "[%lu]",
								 NAME_TABLE.cells[cell_id].loc.RAM_address);

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
		}
	}

	if(init_flag)
	{
		if(CUR_TABLE.size >= CUR_TABLE.capacity)
		{
			CUR_TABLE.capacity *= REALLOC_COEFF;
			REALLOC(CUR_TABLE.cells, CUR_TABLE.capacity, Table_cell)
			LOG("Cells reallocated.\n");
		}

		CUR_TABLE.cells[CUR_TABLE.size].name            = var;

		if(overall_size >= AMOUNT_OF_REGS)
		{
			CUR_TABLE.cells[CUR_TABLE.size].type            = RAM;
			CUR_TABLE.cells[CUR_TABLE.size].loc.RAM_address = overall_size - AMOUNT_OF_REGS;

			snprintf(loc, LOC_SIZE - 1, "[%lu]",
						CUR_TABLE.cells[CUR_TABLE.size].loc.RAM_address);
		}
		else
		{
			CUR_TABLE.cells[CUR_TABLE.size].type       = REG;
			CUR_TABLE.cells[CUR_TABLE.size].loc.reg_id = (unsigned char)overall_size;

			snprintf(loc, LOC_SIZE - 1, "r%cx",
						CUR_TABLE.cells[CUR_TABLE.size].loc.reg_id + 'a');
		}

		CUR_TABLE.size++;


		return loc;

	}
	else
	{
		free(loc);
		return NULL;
	}
}

#undef NAME_TABLE
#undef IS_VAR
