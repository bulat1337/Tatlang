#ifndef FRONTEND_SECONDARY_H
#define FRONTEND_SECONDARY_H

#include "frontend.h"

const size_t kwds_amount = 8;

const char * const kwds[] =
{
	"while",
	"if",
	"sin",
	"cos",
	"ln",
	"sqrt",
	"getvar",
	"putexpr",
};

const size_t STARTER_TOKENS_AMOUNT = 5;
const int    POISON_OP             = -666;

#define MAX_TOKEN_SIZE\
	256

#define LOG(...)\
	frd_write_log("frontend_log", __VA_ARGS__);

void frd_write_log(const char *file_name, const char *fmt, ...);

char *skip_nums(char *symbs);

bool is_number(char sym);

bool is_op(char sym);

bool is_blank(char sym);

char *skip_comment(char *symbs, size_t left_amount);

frd_err_t init_tokens(Tokens *tokens);

void tokens_dtor(Tokens *tokens);

frd_err_t add_token(Tokens *tokens, Node_type type, Node_value value);

Ops get_op(char sym, frd_err_t *error_code);

frd_err_t add_id(Tokens *tokens, char *token);

bool is_kwd(char *token, Node_type *type);

void dump_tokens(Tokens *tokens);

void log_op(Ops op);

Node_type get_type(char *token);

char *get_symbs(const char *file_name, frd_err_t *error_code, size_t *file_len);

frd_err_t add_num(Tokens *tokens, char * *symbs_ptr);

frd_err_t process_sym(char * *symbs_ptr, Tokens *tokens, size_t left_amount, bool *processed);

frd_err_t process_op(char * *symbs_ptr, Tokens *tokens);

frd_err_t process_id(char * *symbs_ptr, Tokens *tokens);

#endif
