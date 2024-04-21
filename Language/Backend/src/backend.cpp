#include "backend_secondary.h"

bkd_err_t assembly(B_tree_node *root, const char *name)
{
	WITH_OPEN
	(
		name, "w", asm_file,

		LOG("%s: asm_file is opened.\n", __func__);

		Nm_tbl_mngr nm_tbl_mngr = {};
		init_name_tables(&nm_tbl_mngr);

		WRITE_ASM(":main\n");

		asmbl(root, asm_file, &nm_tbl_mngr);

		WRITE_ASM("hlt\n");

		dtor_name_tables(&nm_tbl_mngr);
	)

	return BKD_ALL_GOOD;
}
