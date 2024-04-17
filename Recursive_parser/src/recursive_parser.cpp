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

	PARSE_LOG("Getting first command.\n");

	B_tree_node *root = get_cmd();
	CHECK_RET(root);

	B_tree_node *cur_node = root;
	CHECK_RET(cur_node);

	while(CUR_TYPE != END)
	{
		PARSE_LOG("Getting command.\n");
		cur_node->right = get_cmd();
		cur_node = cur_node->right;
	}

	return root;
}
