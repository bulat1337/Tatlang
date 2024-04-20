#ifndef MIDEND_H
#define MIDEND_H

#include "b_tree.h"

typedef enum
{
    MID_ALL_GOOD            = 0, /**< No errors occurred. */
    MID_UNABLE_TO_OPEN_FILE = 1 << 0, /**< Unable to open file error. */
    MID_UNABLE_TO_ALLOCATE  = 1 << 1, /**< Memory allocation error. */
} mid_err_t;

B_tree_node *optimize(B_tree_node *root);

#endif
