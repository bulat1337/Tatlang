#include <stdio.h>
#include <stdarg.h>
#include <string.h>

#include "backend_secondary.h"

#define IS_KWD(kwd)\
	!strncmp(node->value.var_value, kwd, LEN(kwd))

static size_t label_ct = 0;
static bool   ass_flag = false;

void asmbl(B_tree_node *node, FILE *asm_file)
{
	if(node == NULL)
	{
		return;
	}

	switch(node->type)
	{
		case SMC:
		{
			ASMBL(node->left);
			ASMBL(node->right);

			WRITE_ASM("\n");
			break;
		}
		case SCS:
		{
			ASMBL(node->left);
			ASMBL(node->right);

			WRITE_ASM("\n");
			break;
		}
		case SCE:
		{
			ASMBL(node->left);
			ASMBL(node->right);

			WRITE_ASM("\n");
			break;
		}
		case KWD:
		{
			if(IS_KWD("while"))
			{
				write_while(node, asm_file);
			}
			else if(IS_KWD("if"))
			{
				write_if(node, asm_file);
			}
			else
			{
				LOG("%s: ERROR:\n\tUnknown kwd.\n", __func__);
			}

			WRITE_ASM("\n");
			break;
		}
		case OP:
		{
			write_op(node, asm_file);

			break;
		}
		case VAR:
		{
			write_var(node->value.var_value, asm_file);

			break;
		}
		case NUM:
		{
			write_num(node->value.num_value, asm_file);

			break;
		}
		default:
		{
			LOG("%s: ERROR:\n\tUnknown type: %d\n", __func__, node->type);
			return;
		}

	}
}

void write_num(double num, FILE *asm_file)
{
	WRITE_ASM("push %lf\n", num);
}

void write_var(char *var, FILE *asm_file)
{
	// char *dest = get_dest(var);
	// WRITE_ASM("push %s\n", dest);

	WRITE_ASM("push temp_dest\n");
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

bkd_err_t write_op(B_tree_node *node, FILE *asm_file)
{
	switch(node->value.op_value)
	{
		case ASS:
		{
			ASMBL(node->right);
			// char *dest = get_dest(node->left->value.var_value);
			// WRITE_ASM("pop %s\n", dest);
			WRITE_ASM("pop temp_dest\n");

			break;
		}
		CASE(ADD, "add")
		CASE(SUB, "sub")
		CASE(MUL, "mul")
		CASE(DIV, "div")
		CASE(POW, "pow")
		CASE(LN, "ln")
		CASE(SIN, "sin")
		CASE(COS, "cos")
		CASE(SQRT, "sqrt")
		default:
		{
			LOG("%s: ERROR:\n\tUknown operation.\n", __func__);
			return BKD_UKNOWN_OPERATION;
		}
	}

	return BKD_ALL_GOOD;
}

#undef CASE

void write_while(B_tree_node *node, FILE *asm_file)
{
	WRITE_ASM(":while_%lu\n", label_ct);
	size_t while_id = label_ct;
	label_ct++;

	ASMBL(node->left);

	WRITE_ASM("push 0\n");
	WRITE_ASM("je break_%lu\n", while_id);

	ASMBL(node->right);

	WRITE_ASM("jmp while_%lu\n", while_id);
	WRITE_ASM(":break_%lu\n", while_id);
}

void write_if(B_tree_node *node, FILE *asm_file)
{
	ASMBL(node->left);

	WRITE_ASM("push 0\n");
	WRITE_ASM("je break_%lu\n", label_ct);

	ASMBL(node->right);

	WRITE_ASM(":break_%lu\n", label_ct);
	label_ct++;
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

	// fprintf(log_file, "file: %s func: %s on line : %d\n", file_name, func_name, line);
    vfprintf(log_file, fmt, args);

    va_end(args);
}
