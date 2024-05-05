#include <stdio.h>
#include <stdarg.h>
#include <string.h>
#include <wchar.h>


#include "frontend_secondary.h"
#include "utils.h"
#include "file_parser.h"

#include "def_scnd_dsl.h"


void frd_write_log(const char *file_name, const wchar_t *fmt, ...)
{
    static FILE *log_file = fopen(file_name, "w");

    if(log_file == NULL)
	{
        perror("Error opening log_file");
        return;
    }

    va_list args = NULL;

    va_start(args, fmt);

    vfwprintf(log_file, fmt, args);

    va_end(args);
}

wchar_t *skip_nums(wchar_t *symbs)
{
	while(	(*(symbs) >= L'0' && *(symbs) <= L'9') ||
			 *(symbs) == L'.')
	{
		symbs++;
	}

	return symbs;
}

bool is_number(wchar_t sym)
{
	if(sym >= L'0' && sym <= L'9')
	{
		return true;
	}
	else
	{
		return false;
	}
}

bool is_op(wchar_t sym)
{
	if(	sym == L'+' ||
		sym == L'-' ||
		sym == L'*' ||
		sym == L'/' ||
		sym == L'=' ||
		sym == L'^')
	{
		return true;
	}
	else
	{
		return false;
	}
}

bool is_blank(wchar_t sym)
{
	if(	sym == L' '  ||
		sym == L'\n' ||
		sym == L'\0' ||
		sym == L'\t')
	{
		return true;
	}
	else
	{
		return false;
	}
}

wchar_t *skip_comment(wchar_t *symbs, size_t left_amount)
{
	size_t skip_ct = 0;

	while(*symbs != L'\n' && skip_ct < left_amount)
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

	LOG(L"Tokens itited.\n");

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
		LOG(L"Token buffer reallocation. \n\tNew size: %lu\n", tokens->capacity * 2);

		tokens->capacity *= 2;

		REALLOC(tokens->data, tokens->capacity, B_tree_node);

		if(tokens->capacity > 10000)
		{
			printf("inf tok\n");
			exit(EXIT_FAILURE);
		}

		LOG(L"tokens->data reallocated successfuly.");
	}

	tokens->data[tokens->size].type  = type;
	tokens->data[tokens->size].value = value;

	LOG(L"New token\n");
	LOG(L"\taddress: %p\n", tokens->data[tokens->size]);
	LOG(L"\ttype: %d\n", tokens->data[tokens->size].type);

	tokens->size++;

	return FRD_ALL_GOOD;
}

Ops get_op(wchar_t sym, frd_err_t *error_code)
{
	switch(sym)
	{
		case L'+':
		{
			return ADD;
		}
		case L'-':
		{
			return SUB;
		}
		case L'*':
		{
			return MUL;
		}
		case L'/':
		{
			return DIV;
		}
		case L'^':
		{
			return POW;
		}
		case L'=':
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

frd_err_t add_id(Tokens *tokens, wchar_t *token, bool is_func)
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
		LOG(L"It's func.\n");
		CALL(add_token(tokens, FUNC, {.var_value = token}));
	}
	else
	{
		LOG(L"It's VAR: %ls\n", token);
		CALL(add_token(tokens, VAR, {.var_value = token}));
	}

	return error_code;
}

bool is_kwd(wchar_t *token, Node_type *type, Node_value *value)
{
	for(size_t kwd_id = 0; kwd_id < kwds_amount; kwd_id++)
	{
		if(!wcsncmp(token, kwds[kwd_id], MAX_TOKEN_SIZE))
		{
			LOG(L"It's KEYWORD: %ls\n", token);

			*type = get_type(token, value);

			return true;
		}

	}

	return false;
}

void dump_tokens(Tokens *tokens)
{
	LOG(L"\nTokens:\n");
	FOR(size_t token_id = 0; token_id < tokens->size; token_id++)
	{
		LOG(L"%p\n", tokens->data[token_id]);
		switch(tokens->data[token_id].type)
		{
			case NUM:
			{
				LOG(L"\tNUM");
				LOG(L"\t%.2lf\n", tokens->data[token_id].value.num_value);
				break;
			}
			case OP:
			{
				LOG(L"\tOP");
				log_op(tokens->data[token_id].value.op_value);
				break;
			}
			case VAR:
			{
				LOG(L"\tVAR");
				LOG(L"\t%ls\n", tokens->data[token_id].value.var_value);
				break;
			}
			case KEYWORD:
			{
				LOG(L"\tKEYWORD");
				LOG(L"\t%s\n", tokens->data[token_id].value.var_value);
				break;
			}
			case FUNC:
			{
				LOG(L"\tFUNC");
				LOG(L"\t%ls\n", tokens->data[token_id].value.var_value);
				break;
			}
			case STD_FUNC:
			{
				LOG(L"\tSTD_FUNC");
				log_std_func(tokens->data[token_id].value.func);
				break;
			}
			case UNR_OP:
			{
				LOG(L"\tUN_OP");
				log_op(tokens->data[token_id].value.op_value);
				break;
			}
			case SCOPE_START:
			{
				LOG(L"\tSCS");
				LOG(L"\t%s\n", tokens->data[token_id].value.var_value);
				break;
			}
			case SCOPE_END:
			{
				LOG(L"\tSCE");
				LOG(L"\t%s\n", tokens->data[token_id].value.var_value);
				break;
			}
			case ABOVE:
			{
				LOG(L"\tABOVE\n");
				break;
			}
			case BELOW:
			{
				LOG(L"\tBELOW\n");
				break;
			}
			case ABOVE_EQUAL:
			{
				LOG(L"\tABOVE_EQUAL\n");
				break;
			}
			case BELOW_EQUAL:
			{
				LOG(L"\tBELOW_EQUAL\n");
				break;
			}
			case EQUAL:
			{
				LOG(L"\tEQUAL\n");
				break;
			}
			case NOT_EQUAL:
			{
				LOG(L"\tNOT_EQUAL\n");
				break;
			}
			case OPEN_BR:
			{
				LOG(L"\tOPEN_BR\n");
				break;
			}
			case CLOSE_BR:
			{
				LOG(L"\tCLOSE_BR\n");
				break;
			}
			case FUNC_DECL:
			{
				LOG(L"\tFUNC_DECL\n");
				break;
			}
			case CMD_FUNC:
			{
				LOG(L"\tCMD_FUNC\n");
				break;
			}
			case OPEN_CBR:
			{
				LOG(L"\tOPEN_CBR\n");
				break;
			}
			case CLOSE_CBR:
			{
				LOG(L"\tCLOSE_CBR\n");
				break;
			}
			case IF:
			{
				LOG(L"\tIF\n");
				break;
			}
			case WHILE:
			{
				LOG(L"\tWHILE\n");
				break;
			}
			case SEMICOLON:
			{
				LOG(L"\tSEMICOLON\n");
				break;
			}
			case MAIN:
			{
				LOG(L"\tMAIN\n");
				break;
			}
			case RETURN:
			{
				LOG(L"\tRETURN\n");
				break;
			}
			case DECLARE:
			{
				LOG(L"\tDECLARE\n");
				break;
			}
			case COMMA:
			{
				LOG(L"\tCOMMA\n");
				break;
			}
			case END:
			{
				LOG(L"\tEND\n");
				break;
			}
			default:
			{
				LOG(L"UKNOWN TYPE\n");
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
			LOG(L"UKNOWN OPERATION\n")
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
			LOG(L"UKNOWN STD_FUNC\n")
		}
	}
}

#include "undef_log_op_dsl.h"

#define IS_KWD(check_kwd)\
	!wcsncmp(token, check_kwd, MAX_TOKEN_SIZE)

#define IF_UNR_OP(name, type)						\
	else if(IS_KWD(name))							\
	{												\
		LOG(L"It's unary operation: %ls.\n", name);	\
		*value = {.op_value = type};				\
													\
		return UNR_OP;								\
	}

Node_type get_type(wchar_t *token, Node_value *value)
{

	if(IS_KWD(L"рәис"))
	{
		LOG(L"It's main.\n");
		*value = {.num_value = 0};

		return MAIN;
	}
	else if(IS_KWD(L"мисалныяз"))
	{
		LOG(L"It's standart func: putexpr.\n");
		*value = {.func =  PUTEXPR};

		return STD_FUNC;
	}
	else if(IS_KWD(L"алалмаш"))
	{
		LOG(L"It's standart func: getvar.\n");
		*value = {.func =  GETVAR};

		return STD_FUNC;
	}
	else if(IS_KWD(L"булганда"))
	{
		LOG(L"It's while.\n");
		*value = {.num_value = 0};

		return WHILE;
	}
	else if(IS_KWD(L"әгәр"))
	{
		LOG(L"It's if.\n");
		*value = {.num_value = 0};

		return IF;
	}
	else if(IS_KWD(L"белдерү"))
	{
		LOG(L"It's declare.\n");
		*value = {.num_value = 0};

		return DECLARE;
	}
	else if(IS_KWD(L"киребир"))
	{
		LOG(L"It's return.\n");
		*value = {.num_value = 0};

		return RETURN;
	}
	IF_UNR_OP(L"син",  SIN)
	IF_UNR_OP(L"кос",  COS)
	IF_UNR_OP(L"лн",   LN)
	IF_UNR_OP(L"тамырасты", SQRT)
	else
	{
		LOG(L"%s: ERROR:\n\tKeyword could not be handled: %ls.\n", __func__, token);

		return NUM;
	}
}

#undef IF_UNR_OP

#include "def_get_sym_dsl.h"

wchar_t *get_symbs(const char *file_name, frd_err_t *error_code, size_t *file_len)
{
	WITH_OPEN
	(

		file_name, "r", code,

		LOG(L"%s is opened.\n", file_name);

		size_t length = get_file_length(code);
		*file_len = length;

		LOG(L"File length: %lu\n", length);

		wchar_t *symbs = NULL;
		CALLOC(symbs, length + 1, wchar_t);
		LOG(L"symbs is allocated.\n");




		size_t id = 0;
		wint_t ch;

		while ((ch = fgetwc(code)) != WEOF && id < length)
		{
			symbs[id++] = (wchar_t) ch;
		}
		symbs[id] = L'\0';

		LOG(L"Successful fread code -> symbs.\n");

		LOG(L"read code:\n");

		LOG(L"%ls\n", symbs);


		return symbs;
	)
}

#include "undef_get_sym_dsl.h"

frd_err_t add_num(Tokens *tokens, wchar_t * *symbs_ptr)
{
	frd_err_t error_code = FRD_ALL_GOOD;

	LOG(L"It's a number.\n");

	double num = 0;

	swscanf(*symbs_ptr, L"%lf", &num);

	LOG(L"\tnum: %lf\n", num);

	CALL(add_token(tokens, NUM, {.num_value = num}));

	*symbs_ptr = skip_nums(*symbs_ptr);

	return error_code;
}

frd_err_t process_sym(wchar_t * *symbs_ptr, Tokens *tokens, size_t left_amount, bool *processed)
{
	frd_err_t error_code = FRD_ALL_GOOD;
	*processed = true;

	wchar_t cur_symb = *(*symbs_ptr);

	switch(cur_symb)
	{
		CASE(L'(', OPEN_BR)
		CASE(L')', CLOSE_BR)
		CASE(L'{', OPEN_CBR)
		CASE(L'}', CLOSE_CBR)
		CASE(L';', SEMICOLON)
		CASE(L',', COMMA)
		CASE(L'>', ABOVE)
		CASE(L'<', BELOW)
		CASE(L'≥', ABOVE_EQUAL)
		CASE(L'≤', BELOW_EQUAL)
		CASE(L'≡', EQUAL)
		CASE(L'≠', NOT_EQUAL)
		case L'#':
		{
			LOG(L"Skipping comment.\n");
			(*symbs_ptr) = skip_comment((*symbs_ptr), left_amount);

			return error_code;
		}
	}

	*processed = false;
	return error_code;
}

frd_err_t process_op(wchar_t * *symbs_ptr, Tokens *tokens)
{
	frd_err_t error_code = FRD_ALL_GOOD;

	LOG(L"It's OP.\n");

	Ops op = get_op(*(*symbs_ptr), &error_code);
	CHECK_ERROR;

	CALL(add_token(tokens, OP, {.op_value = op}));

	(*symbs_ptr)++;

	return error_code;
}

frd_err_t process_id(wchar_t * *symbs_ptr, Tokens *tokens)
{
	frd_err_t error_code = FRD_ALL_GOOD;

	wchar_t *token = NULL;
	CALLOC(token, MAX_TOKEN_SIZE, wchar_t);


	token = get_token(token, symbs_ptr);
	if(token == NULL)
	{
		return FRD_INVALID_VAR_SYMBOL;
	}


	bool is_func = false;

	if(*(*symbs_ptr) == L'(')
	{
		LOG(L"Next one after %ls is %lc so it's function type token\n",
			 token, *(*symbs_ptr));

		is_func = true;
	}

	LOG(L"\ttoken: %ls\n", token);

	CALL(add_id(tokens, token, is_func));


	return error_code;
}

wchar_t *get_token(wchar_t *token, wchar_t * *symbs_ptr)
{
	wchar_t *token_start = token;

	wchar_t cur_sym = *(*symbs_ptr);

	if(!is_reserved(cur_sym) && !is_number(cur_sym))
	{
		*token = cur_sym;

		token++;
		(*symbs_ptr)++;
		cur_sym = *(*symbs_ptr);
	}
	else
	{
		LOG(L"Invalid first  symbol for variable: %lc\n", cur_sym);
		return NULL;
	}

	while(!is_reserved(cur_sym))
	{
		*token = cur_sym;

		token++;
		(*symbs_ptr)++;
		cur_sym = *(*symbs_ptr);
	}



	*token = L'\0';

	return token_start;
}

bool is_reserved(wchar_t symb)
{
	if(	symb == L'('  ||
		symb == L')'  ||
		symb == L';'  ||
		symb == L','  ||
		symb == L'\n' ||
		symb == L'\t' ||
		symb == L' '  ||
		symb == L'+'  ||
		symb == L'-'  ||
		symb == L'*'  ||
		symb == L'/'  ||
		symb == L'^'  ||
		symb == L'='  ||
		symb == L'>'  ||
		symb == L'<'  ||
		symb == L'≥'  ||
		symb == L'≤'  ||
		symb == L'≡'  ||
		symb == L'≠'  ||
		symb == L'#' )

	{
		return true;
	}
	else
	{
		return false;
	}
}
