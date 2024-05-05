#ifndef FRONTEND_H
#define FRONTEND_H


#include "b_tree.h"
#include "recursive_parser.h"

typedef enum
{
    FRD_ALL_GOOD            = 0, /**< No errors occurred. */
    FRD_UNABLE_TO_OPEN_FILE = 1 << 0, /**< Unable to open file error. */
    FRD_UNABLE_TO_ALLOCATE  = 1 << 1, /**< Memory allocation error. */
	FRD_INVALID_FWRITE      = 1 << 2, /**< The amount of written elements is unexpexted. */
	FRD_INVALID_FREAD       = 1 << 3, /**< The amount of read elements is unexpexted. */
	FRD_UKNOWN_OP           = 1 << 4,
	FRD_INVALID_VAR_SYMBOL  = 1 << 5,
} frd_err_t;

Tokens *tokenize(const char *file, frd_err_t *error_code);

#endif
