#ifndef MIDEND_SECONDARY_H
#define MIDEND_SECONDARY_H

#include "midend.h"
#include "utils.h"

B_tree_node *simplify     (B_tree_node *root);

B_tree_node *fold_consts  (B_tree_node *node);

btr_elem_t   eval         (B_tree_node *node);

B_tree_node *solve_trivial(B_tree_node *node);

#endif
