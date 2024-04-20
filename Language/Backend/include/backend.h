#ifndef BACKEND_H
#define BACKEND_H

#include "assembler.h"
#include "SPU.h"
#include "drivers.h"
#include "b_tree.h"

#define ASSEMBLY(root)\
	assembly(root, #root);

typedef enum
{
    BKD_ALL_GOOD            = 0, /**< No errors occurred. */
    BKD_UNABLE_TO_OPEN_FILE = 1 << 0, /**< Unable to open file error. */
    BKD_UNABLE_TO_ALLOCATE  = 1 << 1, /**< Memory allocation error. */
	BKD_UKNOWN_OPERATION    = 1 << 2,
} bkd_err_t;

bkd_err_t assembly(B_tree_node *root, const char *name);

#endif
