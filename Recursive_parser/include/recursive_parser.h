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

struct B_tree_node *parse_tokens(Tokens *tokens);
struct B_tree_node *get_num();
struct B_tree_node *get_add();
struct B_tree_node *get_mul();
struct B_tree_node *get_par();
struct B_tree_node *get_id();
struct B_tree_node *get_pow();

#endif
