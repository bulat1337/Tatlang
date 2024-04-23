#include "midend_secondary.h"

bool change_flag = false;

B_tree_node *optimize(B_tree_node *root, mid_err_t *error_code)
{
	*error_code = MID_ALL_GOOD;

	do
	{
		change_flag = false;

		root = simplify(root, error_code);

	} while(change_flag == true);

	return root;
}
