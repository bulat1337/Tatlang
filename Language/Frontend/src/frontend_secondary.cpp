#include <stdio.h>
#include <stdarg.h>
#include <string.h>

#include "frontend_secondary.h"
#include "utils.h"

#define CHECK_ERROR\
	if(error_code != FRD_ALL_GOOD)\
		return error_code;

#define FILE_PTR_CHECK(ptr)							\
	if(ptr == NULL)									\
	{												\
		fprintf(stderr, "Unable to open "#ptr"\n");	\
		return FRD_UNABLE_TO_OPEN_FILE;				\
	}

#define ALLOCATION_CHECK(ptr)								\
	if(ptr == NULL)											\
	{														\
		LOG("ERROR:\n\tUnable to allocate "#ptr".\n");		\
															\
		return FRD_UNABLE_TO_ALLOCATE;						\
	}

void write_log(const char *file_name, const char *fmt, ...)
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

char *skip_nums(char *symbs)
{
	while(	(*(symbs) >= '0' && *(symbs) <= '9') ||
			 *(symbs) == '.')
	{
		symbs++;
	}

	return symbs;
}

bool is_number(char sym)
{
	if(sym >= '0' && sym <= '9')
	{
		return true;
	}
	else
	{
		return false;
	}
}

bool is_op(char sym)
{
	if(	sym == '+' ||
		sym == '-' ||
		sym == '*' ||
		sym == '/' ||
		sym == '=' ||
		sym == '^')
	{
		return true;
	}
	else
	{
		return false;
	}
}

bool is_blank(char sym)
{
	if(	sym == ' '  ||
		sym == '\n' ||
		sym == '\t')
	{
		return true;
	}
	else
	{
		return false;
	}
}

char *skip_comment(char *symbs, size_t left_amount)
{
	size_t skip_ct = 0;

	while(*symbs != '\n' && skip_ct < left_amount)
	{
		symbs++;
		skip_ct++;
	}

	symbs++;

	return symbs;
}

frd_err_t init_tokens(Tokens *tokens)
{
	CALLOC(tokens->data, STARTER_TOKENS_AMOUNT, B_tree_node);
	tokens->size = 0;
	tokens->capacity = STARTER_TOKENS_AMOUNT;

	LOG("Tokens itited.\n");

	return FRD_ALL_GOOD;
}

void tokens_dtor(Tokens *tokens)
{
	free(tokens->data);

	tokens->data = NULL;

	tokens->size = 0;
}

frd_err_t add_node(Tokens *tokens, Node_type type, Node_value value)
{
	if(tokens->size >= tokens->capacity)
	{
		LOG("Token buffer reallocation. \n\tNew size: %lu\n", tokens->capacity * 2);

		REALLOC(tokens->data, tokens->capacity * 2, B_tree_node);

		tokens->capacity *= 2;
	}

	tokens->data[tokens->size].type  = type;
	tokens->data[tokens->size].value = value;

	LOG("New token\n");
	LOG("\taddress: %p\n", tokens->data[tokens->size]);
	LOG("\ttype: %d\n", tokens->data[tokens->size].type);

	tokens->size++;

	return FRD_ALL_GOOD;
}

Ops get_op(char sym, frd_err_t error_code = FRD_ALL_GOOD)
{
	switch(sym)
	{
		case '+':
		{
			return ADD;
		}
		case '-':
		{
			return SUB;
		}
		case '*':
		{
			return MUL;
		}
		case '/':
		{
			return DIV;
		}
		case '^':
		{
			return POW;
		}
		case '=':
		{
			return ASS;
		}
		default:
		{
			error_code = FRD_UKNOWN_OP;
			return ASS;
		}
	}
}

frd_err_t add_token(Tokens *tokens, char *token)
{
	frd_err_t error_code = FRD_ALL_GOOD;

	if(is_kwd(token))
	{
		CALL(add_node(tokens, KWD, {.var_value = token}));
	}
	else
	{
		LOG("It's VAR: %s\n", token);
		CALL(add_node(tokens, VAR, {.var_value = token}));
	}

	return error_code;
}

bool is_kwd(char *token)
{
	FOR(size_t kwd_id = 0; kwd_id < kwds_amount; kwd_id++)
	{
		if(!strncmp(token, kwds[kwd_id], strlen(token)))
		{
			LOG("It's KWD: %s\n", token);
			return true;
		}

		FOR_END
	}

	return false;
}

void dump_tokens(Tokens *tokens)
{
	FOR(size_t token_id = 0; token_id < tokens->size; token_id++)
	{
		LOG("%p\n", tokens->data[token_id]);
		switch(tokens->data[token_id].type)
		{
			case NUM:
			{
				LOG("\tNUM");
				LOG("\t%.2lf\n", tokens->data[token_id].value.num_value);
				break;
			}
			case OP:
			{
				LOG("\tOP");
				log_op(tokens->data[token_id].value.op_value);
				break;
			}
			case VAR:
			{
				LOG("\tVAR");
				LOG("\t%s\n", tokens->data[token_id].value.var_value);
				break;
			}
			case KWD:
			{
				LOG("\tKWD");
				LOG("\t%s\n", tokens->data[token_id].value.var_value);
				break;
			}
			case OBR:
			{
				LOG("\tOBR\n");
				break;
			}
			case CBR:
			{
				LOG("\tCBR\n");
				break;
			}
			case OCBR:
			{
				LOG("\tOCBR\n");
				break;
			}
			case CCBR:
			{
				LOG("\tCCBR\n");
				break;
			}
			case SMC:
			{
				LOG("\tSMC\n");
				break;
			}
			default:
			{
				LOG("UKNOWN TYPE\n");
			}
		}

	}
}

#define CASE(op)			\
	case op:				\
	{						\
		LOG("\t"#op"\n");	\
		break;				\
	}

void log_op(Ops op)
{
	switch(op)
	{
		CASE(DO_NOTHING)
		CASE(ADD)
		CASE(SUB)
		CASE(MUL)
		CASE(DIV)
		CASE(POW)
		CASE(LN)
		CASE(SIN)
		CASE(COS)
		CASE(SQRT)
		CASE(ASS)
		default:
		{
			LOG("UKNOWN OPERATION\n")
		}
	}
}

#undef CASE