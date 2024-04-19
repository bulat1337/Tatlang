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

	PARSE_LOG("Getting first scope.\n");

	B_tree_node *root = get_scope();
	CHECK_RET(root);
	
	B_tree_node *cur_node = root;

	while(CUR_TYPE != END)
	{
		B_tree_node *scope_end = get_scope_end(cur_node);

		PARSE_LOG("Getting scope.\n");
		scope_end->right = get_scope();

		cur_node = cur_node->right;
	}

	return root;
}
