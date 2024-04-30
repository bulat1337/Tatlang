#include <stdio.h>
#include <stdarg.h>
#include <string.h>

#include "frontend_secondary.h"
#include "utils.h"
#include "file_parser.h"

#include "def_scnd_dsl.h"


void frd_write_log(const char *file_name, const char *fmt, ...)
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

frd_err_t add_token(Tokens *tokens, Node_type type, Node_value value)
{
	if(tokens->size >= tokens->capacity)
	{
		LOG("Token buffer reallocation. \n\tNew size: %lu\n", tokens->capacity * 2);

		tokens->capacity *= 2;

		REALLOC(tokens->data, tokens->capacity, B_tree_node);

		LOG("tokens->data reallocated successfuly.");
	}

	tokens->data[tokens->size].type  = type;
	tokens->data[tokens->size].value = value;

	LOG("New token\n");
	LOG("\taddress: %p\n", tokens->data[tokens->size]);
	LOG("\ttype: %d\n", tokens->data[tokens->size].type);

	tokens->size++;

	return FRD_ALL_GOOD;
}

Ops get_op(char sym, frd_err_t *error_code)
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
			*error_code = FRD_UKNOWN_OP;
			return ASS;
		}
	}
}

frd_err_t add_id(Tokens *tokens, char *token, bool is_func)
{
	frd_err_t error_code = FRD_ALL_GOOD;
	Node_type type = VAR;
	Node_value val = {.num_value = 0};

	if(is_kwd(token, &type, &val))
	{
		CALL(add_token(tokens, type, val));
	}
	else if(is_func)
	{
		LOG("It's func.\n");
		CALL(add_token(tokens, FUNC, {.var_value = token}));
	}
	else
	{
		LOG("It's VAR: %s\n", token);
		CALL(add_token(tokens, VAR, {.var_value = token}));
	}

	return error_code;
}

bool is_kwd(char *token, Node_type *type, Node_value *value)
{
	FOR(size_t kwd_id = 0; kwd_id < kwds_amount; kwd_id++)
	{
		if(!strncmp(token, kwds[kwd_id], MAX_TOKEN_SIZE))
		{
			LOG("It's KEYWORD: %s\n", token);

			*type = get_type(token, value);

			return true;
		}

		FOR_END
	}

	return false;
}

void dump_tokens(Tokens *tokens)
{
	LOG("\nTokens:\n");
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
			case KEYWORD:
			{
				LOG("\tKEYWORD");
				LOG("\t%s\n", tokens->data[token_id].value.var_value);
				break;
			}
			case FUNC:
			{
				LOG("\tFUNC");
				LOG("\t%s\n", tokens->data[token_id].value.var_value);
				break;
			}
			case STD_FUNC:
			{
				LOG("\tSTD_FUNC");
				log_std_func(tokens->data[token_id].value.func);
				break;
			}
			case UNR_OP:
			{
				LOG("\tUN_OP");
				log_op(tokens->data[token_id].value.op_value);
				break;
			}
			case SCOPE_START:
			{
				LOG("\tSCS");
				LOG("\t%s\n", tokens->data[token_id].value.var_value);
				break;
			}
			case SCOPE_END:
			{
				LOG("\tSCE");
				LOG("\t%s\n", tokens->data[token_id].value.var_value);
				break;
			}
			case OPEN_BR:
			{
				LOG("\tOPEN_BR\n");
				break;
			}
			case CLOSE_BR:
			{
				LOG("\tCLOSE_BR\n");
				break;
			}
			case OPEN_CBR:
			{
				LOG("\tOPEN_CBR\n");
				break;
			}
			case CLOSE_CBR:
			{
				LOG("\tCLOSE_CBR\n");
				break;
			}
			case IF:
			{
				LOG("\tIF\n");
				break;
			}
			case WHILE:
			{
				LOG("\tWHILE\n");
				break;
			}
			case SEMICOLON:
			{
				LOG("\tSEMICOLON\n");
				break;
			}
			case MAIN:
			{
				LOG("\tMAIN\n");
				break;
			}
			case RETURN:
			{
				LOG("\tRETURN\n");
				break;
			}
			case DECLARE:
			{
				LOG("\tDECLARE\n");
				break;
			}
			case COMMA:
			{
				LOG("\tCOMMA\n");
				break;
			}
			case END:
			{
				LOG("\tEND\n");
				break;
			}
			default:
			{
				LOG("UKNOWN TYPE\n");
			}
		}

	}
}

#include "def_log_op_dsl.h"

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

void log_std_func(Std_func func_type)
{
	switch(func_type)
	{
		CASE(GETVAR)
		CASE(PUTEXPR)
		default:
		{
			LOG("UKNOWN STD_FUNC\n")
		}
	}
}

#include "undef_log_op_dsl.h"

#define IS_KWD(check_kwd)\
	!strncmp(token, check_kwd, MAX_TOKEN_SIZE)

#define IF_UNR_OP(name, type)						\
	else if(IS_KWD(name))							\
	{												\
		LOG("It's unary operation: %s.\n", name);	\
		*value = {.op_value = type};				\
													\
		return UNR_OP;								\
	}

Node_type get_type(char *token, Node_value *value)
{

	if(IS_KWD("main"))
	{
		LOG("It's main.\n");
		*value = {.num_value = 0};

		return MAIN;
	}
	else if(IS_KWD("putexpr"))
	{
		LOG("It's standart func: putexpr.\n");
		*value = {.func =  PUTEXPR};

		return STD_FUNC;
	}
	else if(IS_KWD("getvar"))
	{
		LOG("It's standart func: getvar.\n");
		*value = {.func =  GETVAR};

		return STD_FUNC;
	}
	else if(IS_KWD("while"))
	{
		LOG("It's while.\n");
		*value = {.num_value = 0};

		return WHILE;
	}
	else if(IS_KWD("if"))
	{
		LOG("It's if.\n");
		*value = {.num_value = 0};

		return IF;
	}
	else if(IS_KWD("declare"))
	{
		LOG("It's declare.\n");
		*value = {.num_value = 0};

		return DECLARE;
	}
	else if(IS_KWD("return"))
	{
		LOG("It's return.\n");
		*value = {.num_value = 0};

		return RETURN;
	}
	IF_UNR_OP("sin",  SIN)
	IF_UNR_OP("cos",  COS)
	IF_UNR_OP("ln",   LN)
	IF_UNR_OP("sqrt", SQRT)
	else
	{
		LOG("%s: ERROR:\n\tKeyword could not be handled: %s.\n", __func__, token);

		return NUM;
	}
}

#undef IF_UNR_OP

#include "def_get_sym_dsl.h"

char *get_symbs(const char *file_name, frd_err_t *error_code, size_t *file_len)
{
	WITH_OPEN
	(

		file_name, "r", code,

		LOG("%s is opened.\n", file_name);

		size_t length = get_file_length(code);
		*file_len = length;

		LOG("File length: %lu\n", length);

		char *symbs = NULL;
		CALLOC(symbs, length + 1, char);
		LOG("symbs is allocated.\n");

		FREAD(symbs, sizeof(char), length, code);

		symbs[length] = '\0';

		LOG("Successful fread code -> symbs.\n");

		return symbs;
	)
}

#include "undef_get_sym_dsl.h"

frd_err_t add_num(Tokens *tokens, char * *symbs_ptr)
{
	frd_err_t error_code = FRD_ALL_GOOD;

	LOG("It's a number.\n");

	double num = 0;

	sscanf(*symbs_ptr, "%lf", &num);

	LOG("\tnum: %lf\n", num);

	CALL(add_token(tokens, NUM, {.num_value = num}));

	*symbs_ptr = skip_nums(*symbs_ptr);

	return error_code;
}

frd_err_t process_sym(char * *symbs_ptr, Tokens *tokens, size_t left_amount, bool *processed)
{
	frd_err_t error_code = FRD_ALL_GOOD;
	*processed = true;

	char cur_symb = *(*symbs_ptr);

	switch(cur_symb)
	{
		CASE('(', OPEN_BR)
		CASE(')', CLOSE_BR)
		CASE('{', OPEN_CBR)
		CASE('}', CLOSE_CBR)
		CASE(';', SEMICOLON)
		CASE(',', COMMA)
		case '#':
		{
			LOG("Skipping comment.\n");
			(*symbs_ptr) = skip_comment((*symbs_ptr), left_amount);

			return error_code;
		}
	}

	*processed = false;
	return error_code;
}

frd_err_t process_op(char * *symbs_ptr, Tokens *tokens)
{
	frd_err_t error_code = FRD_ALL_GOOD;

	LOG("It's OP.\n");

	Ops op = get_op(*(*symbs_ptr), &error_code);
	CHECK_ERROR;

	CALL(add_token(tokens, OP, {.op_value = op}));

	(*symbs_ptr)++;

	return error_code;
}

frd_err_t process_id(char * *symbs_ptr, Tokens *tokens)
{
	frd_err_t error_code = FRD_ALL_GOOD;

	char *token = NULL;
	CALLOC(token, MAX_TOKEN_SIZE, char);

	int amount = 0;

	sscanf((*symbs_ptr), "%[a-zA-Z0-9_$]%n", token, &amount);

	(*symbs_ptr) += amount;

	bool is_func = false;

	if(*(*symbs_ptr) == '(')
	{
		LOG("Next one after %s is %c so it's function type token\n",
			 token, *(*symbs_ptr));

		is_func = true;
	}

	LOG("\ttoken: %s\n", token);

	CALL(add_id(tokens, token, is_func));


	return error_code;
}
