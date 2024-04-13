#include <stdio.h>

#include "recursive_parser_secondary.h"

#define FIRST_SYM_COND									\
	('a' <= *(cur_str) && *(cur_str) <= 'z') ||			\
	('A' <= *(cur_str) && *(cur_str) <= 'Z') ||			\
	(*(cur_str) == '_') ||								\
	(*(cur_str) == '$')

#define SYM_COND																\
	('a' <= *(cur_str + sym_counter) && *(cur_str + sym_counter) <= 'z') ||		\
	('A' <= *(cur_str + sym_counter) && *(cur_str + sym_counter) <= 'Z') ||		\
	('0' <= *(cur_str + sym_counter) && *(cur_str + sym_counter) <= '9') ||		\
	(*(cur_str + sym_counter) == '_') ||										\
	(*(cur_str + sym_counter) == '$')

size_t count_symbols(const char *cur_str)
{
	PARSE_LOG("%s log:\n", __func__);
	size_t sym_counter = 0;

	PARSE_LOG("reading symbols:\n");

	if(FIRST_SYM_COND)
	{
		PARSE_LOG("%c ", *(cur_str));
		sym_counter++;
	}
	else
	{
		exit(EXIT_FAILURE);
	}

	while(SYM_COND)
	{
		PARSE_LOG("%c ", *(cur_str + sym_counter));
		sym_counter++;
	}

	PARSE_LOG("\n\n");

	return sym_counter;
}

char *skip_spaces(const char *str, size_t size)
{
	size_t copy_size = count_non_spaces(str, size);

	char *str_copy = (char *)calloc(copy_size + 1, sizeof(char));
	if(str_copy == NULL)
	{
		PARSE_LOG("ERROR: Unable to allocate memory for str_copy\n");
		return NULL;
	}

	size_t copy_ID = 0;
	for(size_t sym_ID = 0; sym_ID < size; sym_ID++)
	{
		if(str[sym_ID] != ' ')
		{
			str_copy[copy_ID] = str[sym_ID];
			copy_ID++;
		}
		else
		{
			;
		}
	}

	str_copy[copy_size] = '\0';

	return str_copy;
}

size_t count_non_spaces(const char *str, size_t size)
{
	size_t non_spaces = 0;
	for(size_t sym_ID = 0; sym_ID < size; sym_ID++)
	{
		if(str[sym_ID] != ' ')
		{
			non_spaces++;
		}
	}

	return non_spaces;
}

#undef SYM_COND
#undef FIRST_SYM_COND
