#ifndef FRONTEND_SECONDARY_H
#define FRONTEND_SECONDARY_H

#include "frontend.h"

const size_t kwds_amount = 11;

const wchar_t * const kwds[] =
{
	L"булганда",
	L"әгәр",
	L"sin",
	L"cos",
	L"ln",
	L"тамырасты",
	L"алалмаш",
	L"мисалныяз",
	L"белдерү",
	L"киребир",
	L"рәис",
};

const size_t STARTER_TOKENS_AMOUNT = 5;
const int    POISON_OP             = -666;

#define LOG(...)\
	frd_write_log("frontend_log", __VA_ARGS__);

void      frd_write_log(const char *file_name, const wchar_t *fmt, ...);

wchar_t  *skip_nums(wchar_t *symbs);

bool      is_number(wchar_t sym);

bool      is_op(wchar_t sym);

bool      is_blank(wchar_t sym);

wchar_t  *skip_comment(wchar_t *symbs, size_t left_amount);

frd_err_t init_tokens(Tokens *tokens);

void      tokens_dtor(Tokens *tokens);

frd_err_t add_token(Tokens *tokens, Node_type type, Node_value value);

Ops       get_op(wchar_t sym, frd_err_t *error_code);

frd_err_t add_id(Tokens *tokens, wchar_t *token, bool is_func);

bool      is_kwd(wchar_t *token, Node_type *type, Node_value *value);

void      dump_tokens(Tokens *tokens);

void      log_std_func(Std_func func_type);

void      log_op(Ops op);

Node_type get_type(wchar_t *token, Node_value *value);

wchar_t  *get_symbs(const char *file_name, frd_err_t *error_code, size_t *file_len);

frd_err_t add_num(Tokens *tokens, wchar_t * *symbs_ptr);

frd_err_t process_sym(wchar_t * *symbs_ptr, Tokens *tokens, size_t left_amount, bool *processed);

frd_err_t process_op(wchar_t * *symbs_ptr, Tokens *tokens);

frd_err_t process_id(wchar_t * *symbs_ptr, Tokens *tokens);


#endif
