#ifndef BACKEND_SECONDAARY_H
#define BACKEND_SECONDAARY_H

#include "backend.h"
#include "utils.h"

const size_t ST_CELLS_AMOUNT = 10;
const size_t LOC_SIZE        = 20;
const size_t REALLOC_COEFF   = 2;
const size_t AMOUNT_OF_REGS  = 4;

enum Loc_type
{
	REG = 0,
	RAM = 1,
};

union Loc
{
	unsigned char reg_id;
	size_t        RAM_address;
};

struct Table_cell
{
	wchar_t    *name;
	Loc_type    type;
	Loc         loc;
};

struct Name_table
{
	Table_cell *cells;
	size_t capacity;
	size_t size;
};

struct Nm_tbl_mngr
{
	size_t cur_lvl;
	Name_table *name_tables;
	bool in_func_start;
};


#define ASMBL(node)											\
	error_code = asmbl(node, asm_file, nm_tbl_mngr);		\
	if(error_code != BKD_ALL_GOOD)							\
	{														\
		LOG("%s: ERROR:\n\tasmbl error: %d.\n", __func__);	\
		return error_code;									\
	}

#define LOG(...)\
	bkd_write_log("backend_log", __VA_ARGS__);

#define WRITE_ASM(...)\
	fprintf(asm_file, __VA_ARGS__);

#define FILE_PTR_CHECK(file_ptr)									\
	if(file_ptr == NULL)											\
	{																\
		LOG("\nERROR: Unable to open "#file_ptr"\n");				\
		return BKD_UNABLE_TO_OPEN_FILE;								\
	}

#define ALLOCATION_CHECK(ptr)								\
	if(ptr == NULL)											\
	{														\
		LOG("ERROR:\n\tUnable to allocate "#ptr".\n");		\
															\
		return BKD_UNABLE_TO_ALLOCATE;						\
	}

#define CHECK_ERROR													\
	if(error_code != BKD_ALL_GOOD)									\
		return error_code;

void        write_num        (double num, FILE *asm_file);

bkd_err_t   asmbl            (B_tree_node *node, FILE *asm_file, Nm_tbl_mngr *nm_tbl_mngr);

void        bkd_write_log    (const char *file_name, const char *fmt, ...);

bkd_err_t   write_while      (B_tree_node *node, FILE *asm_file, Nm_tbl_mngr *nm_tbl_mngr);

bkd_err_t   write_if         (B_tree_node *node, FILE *asm_file, Nm_tbl_mngr *nm_tbl_mngr);

bkd_err_t   write_var        (wchar_t *var, FILE *asm_file, Nm_tbl_mngr *nm_tbl_mngr);

bkd_err_t   write_op         (B_tree_node *node, FILE *asm_file, Nm_tbl_mngr *nm_tbl_mngr);

bkd_err_t   init_name_tables (Nm_tbl_mngr *nm_tbl_mngr);

bkd_err_t   upgrade_n_table  (Nm_tbl_mngr *nm_tbl_mngr);

bkd_err_t   downgrade_n_table(Nm_tbl_mngr *nm_tbl_mngr);

bkd_err_t   dtor_name_tables (Nm_tbl_mngr *nm_tbl_mngr);

char       *get_loc          (wchar_t *var, Nm_tbl_mngr *nm_tbl_mngr,
		  					  bool init_flag, bkd_err_t *error_code);

bkd_err_t   write_getvar     (B_tree_node *node, FILE *asm_file, Nm_tbl_mngr *nm_tbl_mngr);

bkd_err_t   write_putexpr    (B_tree_node *node, FILE *asm_file, Nm_tbl_mngr *nm_tbl_mngr);

char       *init_var         (wchar_t *var, struct Name_table *cur_table,
		  	                  bkd_err_t *error_code, size_t overall_size);

char       *get_init_var     (Table_cell *cell, bkd_err_t *error_code);

bkd_err_t   write_func_decl  (B_tree_node *node, FILE *asm_file);

bkd_err_t   write_return     (B_tree_node *node, FILE *asm_file, Nm_tbl_mngr *nm_tbl_mngr);

bkd_err_t   write_main       (B_tree_node *node, FILE *asm_file);

bkd_err_t   push_all         (Nm_tbl_mngr *nm_tbl_mngr, FILE *asm_file);

bkd_err_t   write_func       (B_tree_node *node, FILE *asm_file, Nm_tbl_mngr *nm_tbl_mngr);

char       *get_loc_in_order (size_t arg_counter, bkd_err_t *error_code);

bkd_err_t   pop_all          (Nm_tbl_mngr *nm_tbl_mngr, FILE *asm_file);

bkd_err_t   write_cond_expr  (B_tree_node *node, FILE *asm_file, Nm_tbl_mngr *nm_tbl_mngr);

const char *get_cond_type    (Node_type type);

bkd_err_t   save_ret_reg     (FILE *asm_file, Node_type type, Nm_tbl_mngr *nm_tbl_mngr);

#endif
