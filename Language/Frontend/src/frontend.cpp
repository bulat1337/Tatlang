#include <stdio.h>
#include <stdlib.h>

#include "frontend.h"
#include "frontend_secondary.h"
#include "utils.h"
#include "file_parser.h"

#define CHECK_ERROR\
	if(error_code != FRD_ALL_GOOD)\
		return NULL;

#define LEFT_AMOUNT\
	(file_len - (size_t)(symbs - symbs_start))

#define ALLOCATION_CHECK(ptr)								\
	if(ptr == NULL)											\
	{														\
		LOG("ERROR:\n\tUnable to allocate "#ptr".\n");		\
															\
		error_code = FRD_UNABLE_TO_ALLOCATE;				\
		return NULL;										\
	}

#define FILE_PTR_CHECK(ptr)								\
	if(ptr == NULL)										\
	{													\
		fprintf(stderr, "Unable to open "#ptr"\n");		\
		error_code = FRD_UNABLE_TO_OPEN_FILE;			\
		return NULL;									\
	}

#define FREAD_CHECK(read_elems, amount)										\
	if(read_elems != amount)												\
	{																		\
		LOG("ERROR: Fread read unexpected amount of elems.\n");				\
		LOG("\t expected amount: %lu.\n", amount);							\
		LOG("\t read amount: %lu.\n", read_elems);							\
																			\
		error_code = FRD_INVALID_FREAD;										\
		return NULL;														\
	}

Tokens *tokenize(const char *file, frd_err_t error_code = FRD_ALL_GOOD)
{
	Tokens *tokens = NULL;
	CALLOC(tokens, 1, Tokens);
	CALL(init_tokens(tokens));

	WITH_OPEN
	(
		file, "r", code,

		LOG("%s is opened!\n", file);

		size_t file_len = get_file_length(code);

		LOG("file length: %lu\n", file_len);

		char *symbs = {};
		CALLOC(symbs, file_len + 1, char);
		char *symbs_start = symbs;

		FREAD(symbs, sizeof(char), file_len, code);
		symbs[file_len] = '\0';

		double num = 0;

		while((size_t)(symbs - symbs_start) < file_len)
		{
			LOG("\nCurrent symbol: %c\n", *symbs);

			if(is_blank(*symbs))
			{
				LOG("It's blank.\n");

				symbs++;
			}
			else if(is_number(*symbs))
			{
				LOG("It's a number.\n");

				sscanf(symbs, "%lf", &num);

				LOG("\tnum: %lf\n", num);

				CALL(add_node(tokens, NUM, {.num_value = num}));

				symbs = skip_nums(symbs);
			}
			else if(*symbs == '(')
			{
				LOG("It's OBR.\n");

				CALL(add_node(tokens, OBR, {.num_value = 0}));

				symbs++;
			}
			else if(*symbs == ')')
			{
				LOG("It's CBR.\n");

				CALL(add_node(tokens, CBR, {.num_value = 0}));

				symbs++;
			}
			else if(*symbs == '{')
			{
				LOG("It's OCBR.\n");

				CALL(add_node(tokens, OCBR, {.num_value = 0}));

				symbs++;
			}
			else if(*symbs == '}')
			{
				LOG("It's CCBR.\n");

				CALL(add_node(tokens, CCBR, {.num_value = 0}));

				symbs++;
			}
			else if(*symbs == ';')
			{
				LOG("It's SMC.\n");

				CALL(add_node(tokens, SMC, {.num_value = 0}));

				symbs++;
			}
			else if(is_op(*symbs))
			{
				LOG("It's OP.\n");

				Ops op = get_op(*symbs, error_code);
				CHECK_ERROR;

				CALL(add_node(tokens, OP, {.op_value = op}));

				symbs++;
			}
			else if(*symbs == '#')
			{
				LOG("Skipping comment.\n");
				symbs = skip_comment(symbs, LEFT_AMOUNT);
			}
			else
			{
				char *token = NULL;
				CALLOC(token, MAX_TOKEN_SIZE, char);

				size_t amount = 0;

				sscanf(symbs, "%[a-zA-Z0-9,_,$]%n", token, &amount);

				LOG("\ttoken: %s\n", token);

				CALL(add_token(tokens, token));

				symbs += amount;
			}
		}
	)

	CALL(add_node(tokens, END, {.num_value = 0}));
	dump_tokens(tokens);

	return tokens;
}