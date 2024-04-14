#ifndef RECURSIVE_PARSER_SECONDARY_H
#define RECURSIVE_PARSER_SECONDARY_H

#include "recursive_parser.h"

#define PARSE_LOG(...)\
	write_log("parse_log.txt", __VA_ARGS__);

size_t count_symbols   (const char *cur_str);

char * skip_spaces     (const char *str, size_t size);

size_t count_non_spaces(const char *str, size_t size);

void   write_log       (const char *file_name, const char *fmt, ...);


#endif
