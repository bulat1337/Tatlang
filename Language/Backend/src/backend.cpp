#include "backend_secondary.h"

bkd_err_t assembly(B_tree_node *root, const char *name)
{
	WITH_OPEN
	(
		name, "w", asm_file,

		LOG("%s: asm_file is opened.\n", __func__);

		ASMBL(root);
	)

	return BKD_ALL_GOOD;
}
