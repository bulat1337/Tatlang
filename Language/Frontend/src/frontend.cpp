#include <stdio.h>
#include <stdlib.h>

#include "frontend.h"
#include "frontend_secondary.h"
#include "utils.h"

#include "def_frd_dsl.h"

Tokens *tokenize(const char *file, frd_err_t *error_code)
{
	Tokens *tokens = NULL;
	CALLOC(tokens, 1, Tokens);
	CALL(init_tokens(tokens));

	size_t file_len = 0;

	char *symbs = get_symbs(file, error_code, &file_len);
	CHECK_ERROR(error_code);

	char *symbs_start = symbs;


	while((size_t)(symbs - symbs_start) < file_len)
	{
		char cur_symb = *symbs;
		LOG("\nCurrent symbol: %c\n", cur_symb);

		if(is_blank(cur_symb))
		{
			LOG("It's blank.\n");

			symbs++;
		}
		else if(is_number(cur_symb))
		{
			CALL(add_num(tokens, &symbs));
		}
		else
		{
			bool processed = false;

			CALL(process_sym(&symbs, tokens, LEFT_AMOUNT, &processed));

			if(!processed)
			{
				if(is_op(cur_symb))
				{
					CALL(process_op(&symbs, tokens));
				}
				else
				{
					CALL(process_id(&symbs, tokens));
				}
			}
		}


	}

	CALL(add_token(tokens, END, {.num_value = 0}));
	dump_tokens(tokens);

	return tokens;
}
