#ifndef PARSER_H
#define PARSER_H

#include "b_tree.h"
#include "utils.h"

struct Tokens
{
	B_tree_node *data;
	size_t size;
	size_t capacity;
};

B_tree_node *parse_tokens(Tokens *tokens);

#endif
