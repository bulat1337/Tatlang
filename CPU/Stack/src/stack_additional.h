#ifndef STACK_ADDITIONAL_H
#define STACK_ADDITIONAL_H

#include "utils.h"

#define LOGGING

#ifdef LOGGING
	#define LOG_FUNC(stk) fprintf((stk)->log_file ,"\n%s LOG:\n", __func__)
#endif


void show_bits(int x, FILE *log_file);

#endif
