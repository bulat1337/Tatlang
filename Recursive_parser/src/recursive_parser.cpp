#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#include "recursive_parser.h"
#include "recursive_parser_secondary.h"

Tokens *tokens = NULL;
size_t id = 0;

B_tree_node *parse_tokens(Tokens *passed_tokens)
{
	tokens = passed_tokens;

	id = 0;

	if(CUR_TYPE == END)
	{
		return CR_SMC(NULL, NULL);
	}

	B_tree_node *root = get_all_scopes(false, END);
	CHECK_RET(root);

	return root;
}
