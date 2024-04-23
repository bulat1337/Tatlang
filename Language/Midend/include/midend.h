#ifndef MIDEND_H
#define MIDEND_H

#include "b_tree.h"

typedef enum
{
    MID_ALL_GOOD            = 0, /**< No errors occurred. */
    MID_UNABLE_TO_OPEN_FILE = 1 << 0, /**< Unable to open file error. */
    MID_UNABLE_TO_ALLOCATE  = 1 << 1, /**< Memory allocation error. */
	MID_NULL_NODE           = 1 << 2,
	MID_DIV_BY_ZERO         = 1 << 3,
	MID_INVALID_OP          = 1 << 4,
} mid_err_t;

B_tree_node *optimize(B_tree_node *root, mid_err_t *error_code);

#endif
