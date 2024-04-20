#ifndef BACKEND_SECONDAARY_H
#define BACKEND_SECONDAARY_H

#include "backend.h"
#include "utils.h"

#define ASMBL(node)\
	asmbl(node, asm_file);

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

void      write_num    (double num, FILE *asm_file);

void      asmbl        (B_tree_node *node, FILE *asm_file);

void      bkd_write_log(const char *file_name, const char *fmt, ...);

void      write_var    (char *var, FILE *asm_file);

void      write_while  (B_tree_node *node, FILE *asm_file);

void      write_if     (B_tree_node *node, FILE *asm_file);

bkd_err_t write_op     (B_tree_node *node, FILE *asm_file);

#endif
