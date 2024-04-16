#ifndef FRONTEND_SECONDARY_H
#define FRONTEND_SECONDARY_H

#include "frontend.h"

const size_t kwds_amount = 5;

const char * const kwds[] =
{
	"while",
	"if",
	"sin",
	"cos",
	"ln",
};

const size_t STARTER_TOKENS_AMOUNT = 5;
const int    POISON_OP             = -666;

#define MAX_TOKEN_SIZE\
	256

#define LOG(...)\
	write_log("frontend_log", __VA_ARGS__);

void write_log(const char *file_name, const char *fmt, ...);

char *skip_nums(char *symbs);

bool is_number(char sym);

bool is_op(char sym);

bool is_blank(char sym);

char *skip_comment(char *symbs, size_t left_amount);

frd_err_t init_tokens(Tokens *tokens);

void tokens_dtor(Tokens *tokens);

frd_err_t add_node(Tokens *tokens, Node_type type, Node_value value);

Ops get_op(char sym, frd_err_t error_code);

frd_err_t add_token(Tokens *tokens, char *token);

bool is_kwd(char *token);

void dump_tokens(Tokens *tokens);

void log_op(Ops op);

#endif
