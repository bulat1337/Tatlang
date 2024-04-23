#ifndef MIDEND_SECONDARY_H
#define MIDEND_SECONDARY_H

#include "midend.h"
#include "utils.h"

#define LOG(...)\
	mid_write_log("midend_log", __VA_ARGS__);

B_tree_node *simplify     (B_tree_node *root, mid_err_t *error_code);

B_tree_node *fold_consts  (B_tree_node *node, mid_err_t *error_code);

btr_elem_t   eval         (B_tree_node *node, mid_err_t *error_code);

B_tree_node *solve_trivial(B_tree_node *node);

void         mid_write_log(const char *file_name, const char *fmt, ...);

#endif
