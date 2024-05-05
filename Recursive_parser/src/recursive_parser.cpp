#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <clocale>

#include "recursive_parser.h"
#include "recursive_parser_secondary.h"

B_tree_node *parse_tokens(Tokens *passed_tokens)
{
	setlocale(LC_ALL, "");

	B_tree_node *root = get_general(passed_tokens);
	CHECK_RET(root);

	return root;
}
