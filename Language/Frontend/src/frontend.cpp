#include <stdio.h>
#include <stdlib.h>
#include <clocale>

#include "frontend.h"
#include "frontend_secondary.h"
#include "utils.h"

#include "def_frd_dsl.h"

Tokens *tokenize(const char *file, frd_err_t *error_code)
{
	setlocale(LC_ALL, "");

	Tokens *tokens = NULL;
	CALLOC(tokens, 1, Tokens);
	CALL(init_tokens(tokens));

	size_t file_len = 0;

	wchar_t *symbs = get_symbs(file, error_code, &file_len);
	CHECK_ERROR(error_code);

	wchar_t *symbs_start = symbs;


	while((size_t)(symbs - symbs_start) < file_len)
	{
		wchar_t cur_symb = *symbs;
		LOG(L"\nCurrent symbol: %lc\n", cur_symb);

		if(is_blank(cur_symb))
		{
			LOG(L"It's blank.\n");

			symbs++;
		}
		else if(is_number(cur_symb))
		{
			CALL(add_num(tokens, &symbs));
		}
		//else if(process_sym(&symbs, tokens, LEFT_AMOUNT, &processed));) //returns bool
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
