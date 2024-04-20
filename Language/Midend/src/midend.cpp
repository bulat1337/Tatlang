#include "midend_secondary.h"

bool change_flag = false;

B_tree_node *optimize(B_tree_node *root)
{
	do
	{
		change_flag = false;

		root = simplify(root);

	} while(change_flag == true);

	return root;
}
