#ifndef B_TREE_H
#define B_TREE_H

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>

typedef double btr_elem_t;

#define GR_DUMP_CODE_GEN(root) gr_dump_code_gen(root, #root);

#define TXT_DUMP(root) txt_dump(root, #root);

typedef enum
{
	B_TREE_ALL_GOOD 		   = 0,
	B_TREE_UNABLE_TO_OPEN_FILE = 1 << 0,
	UNABLE_TO_ALLOCATE         = 1 << 1,
	UNEXPECTED_WRITTEN_ELEMS   = 1 << 3,
	B_TREE_NULL_PTR            = 1 << 4,
	B_TREE_NODE_NULL_PTR       = 1 << 5,
	INVALID_VALUE              = 1 << 6,
	INVALID_INDEX              = 1 << 7,
	PARENT_NODE_IS_FREE        = 1 << 8,
	UNKNOWN_NODE_TYPE          = 1 << 9,
} error_t;


enum Node_type
{
	NUM         = 0,
	OP          = 1,
	VAR         = 2,
	OPEN_BR     = 3,
	CLOSE_BR    = 4,
	OPEN_CBR    = 5,
	CLOSE_CBR   = 6,
	SEMICOLON   = 7,
	KEYWORD     = 8,
	END         = 9,
	SCOPE_START = 10,
	SCOPE_END   = 11,
	IF          = 12,
	WHILE       = 13,
	FUNC        = 14,
	UNR_OP      = 15,
};

enum Ops
{
	DO_NOTHING = 0,
	ADD        = 1,
	SUB        = 2,
	MUL        = 3,
	DIV        = 4,
	POW        = 5,
	LN         = 6,
	SIN        = 7,
	COS        = 8,
	SQRT       = 9,
	ASS        = 10,
};

struct Node_value
{
	double   num_value;
	Ops op_value;
	char 	*var_value;
};

struct B_tree_node
{
	Node_type    type;
	Node_value   value;
	B_tree_node *left;
	B_tree_node *right;
};

union Second_arg
{
	B_tree_node *node;
	FILE        *file_ptr;
};

struct Uni_ret
{
	error_t    error_code;
	Second_arg arg;
};

struct Node_charachteristics
{
	const char *name;
	const char *color;
	char       *label;
};

const size_t NODE_LABEL_STR_SIZE   = 100;
const size_t OP_TOKEN_SIZE 		   = 15;
const size_t GR_DUMP_GEN_CMD_SIZE  = 100;
const bool   RIGHT_CHILD           = true;
const bool   LEFT_CHILD            = false;

const char * const PASTEL_GREEN    = "#9AFF55";
const char * const LIGHT_GREEN     = "#D7FF25";
const char * const LIGHT_BLUE      = "#2595FF";
const char * const CORAL_PINK      = "#F08080";
const char * const MARGENGO        = "#48565D";
const char * const RED_PINK        = "#FF5555";
const char * const AGRESSIVE_RED   = "#E72900";
const char * const NEUTRAL_GREY    = "#A4A4A4";
const char * const PURPLE          = "#B700EC";
const char * const DARK_PURPLE     = "#7A009D";
const char * const MUSTARD_YELLOW  = "#C4B500";
const char * const LIGHT_YELLOW    = "#FFE438";
const char * const SKY_BLUE        = "#5AD5FF";
const char * const RASPBERRIE_PINK = "#C40062";



Uni_ret create_node     (Node_type type, Node_value value, B_tree_node *left_child,
						 B_tree_node *right_child);

error_t add_child       (B_tree_node *parent, B_tree_node *child, bool is_right_child);

error_t destroy_subtree (B_tree_node *parent_node, bool is_right_child);

error_t txt_dump        (B_tree_node *root, const char *name);

Uni_ret gr_dump_code_gen(B_tree_node *root, const char *b_tree_name);

bool    cmp_nodes       (B_tree_node *node_1, B_tree_node *node_2);


#endif
