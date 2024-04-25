#include <stdio.h>
#include <stdlib.h>

#include "frontend.h"
#include "frontend_secondary.h"
#include "utils.h"
#include "file_parser.h"

#include "def_frd_dsl.h"

Tokens *tokenize(const char *file, frd_err_t error_code = FRD_ALL_GOOD)
{
	Tokens *tokens = NULL;
	CALLOC(tokens, 1, Tokens);
	CALL(init_tokens(tokens));

	WITH_OPEN
	(
		file, "r", code,

		LOG("%s is opened.\n", file);

		size_t file_len = get_file_length(code);

		LOG("File length: %lu\n", file_len);

		char *symbs = {};
		CALLOC(symbs, file_len + 1, char);
		char *symbs_start = symbs;

		LOG("symbs is allocated.\n");

		FREAD(symbs, sizeof(char), file_len, code);
		symbs[file_len] = '\0';
		LOG("Successful fread code -> symbs.\n");

		double num = 0;

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
				LOG("It's a number.\n");

				sscanf(symbs, "%lf", &num);

				LOG("\tnum: %lf\n", num);

				CALL(add_node(tokens, NUM, {.num_value = num}));

				symbs = skip_nums(symbs);
			}
			else
			{
				switch(cur_symb)
				{
					case '(':
					{
						LOG("It's OPEN_BR.\n");

						CALL(add_node(tokens, OPEN_BR, {.num_value = 0}));

						symbs++;

						break;
					}
					case ')':
					{
						LOG("It's CLOSE_BR.\n");

						CALL(add_node(tokens, CLOSE_BR, {.num_value = 0}));

						symbs++;

						break;
					}
					case '{':
					{
						LOG("It's OPEN_CBR.\n");

						CALL(add_node(tokens, OPEN_CBR, {.num_value = 0}));

						symbs++;

						break;
					}
					case '}':
					{
						LOG("It's CLOSE_CBR.\n");

						CALL(add_node(tokens, CLOSE_CBR, {.num_value = 0}));

						symbs++;

						break;
					}
					case ';':
					{
						LOG("It's SEMICOLON.\n");

						CALL(add_node(tokens, SEMICOLON, {.num_value = 0}));

						symbs++;

						break;
					}
					case '#':
					{
						LOG("Skipping comment.\n");
						symbs = skip_comment(symbs, LEFT_AMOUNT);

						break;
					}
					default:
					{
						if(is_op(cur_symb))
						{
							LOG("It's OP.\n");

							Ops op = get_op(cur_symb, &error_code);
							CHECK_ERROR;

							CALL(add_node(tokens, OP, {.op_value = op}));

							symbs++;
						}
						else
						{
							char *token = NULL;
							CALLOC(token, MAX_TOKEN_SIZE, char);

							int amount = 0;

							sscanf(symbs, "%[a-zA-Z0-9,_,$]%n", token, &amount);

							LOG("\ttoken: %s\n", token);

							CALL(add_token(tokens, token));

							symbs += amount;
						}

						break;
					}
				}
			}
		}
	)

	CALL(add_node(tokens, END, {.num_value = 0}));
	dump_tokens(tokens);

	return tokens;
}
