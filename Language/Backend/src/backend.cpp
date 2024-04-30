#include "backend_secondary.h"

bkd_err_t assembly(B_tree_node *root, const char *name)
{
	bkd_err_t error_code = BKD_ALL_GOOD;
	WITH_OPEN
	(
		name, "w", asm_file,

		LOG("%s: asm_file is opened.\n", __func__);

		Nm_tbl_mngr nm_tbl_mngr = {};

		CALL(asmbl(root, asm_file, &nm_tbl_mngr));

		WRITE_ASM("hlt\n");
	)

	return error_code;
}
