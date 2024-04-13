#ifndef RECURSIVE_PARSER_SECONDARY_H
#define RECURSIVE_PARSER_SECONDARY_H

#include "recursive_parser.h"

size_t count_symbols   (const char *cur_str);

char * skip_spaces     (const char *str, size_t size);

size_t count_non_spaces(const char *str, size_t size);


#endif
