#ifndef UTILS_H
#define UTILS_H

#define LEN(str)\
	sizeof(str) / sizeof(char) - 1

#define CALL(...)				\
	error_code = __VA_ARGS__;	\
	CHECK_ERROR;

#define CALLOC(ptr, amount, type)				\
	ptr = (type *)calloc(amount, sizeof(type));	\
	ALLOCATION_CHECK(ptr);

#define REALLOC(ptr, new_amount, type)						\
	ptr = (type *)realloc(ptr, new_amount * sizeof(type));	\
	ALLOCATION_CHECK(ptr);

#define FREAD(buf, elem_size, amount, file_ptr)								\
	size_t read_elems = fread(buf, elem_size, amount, file_ptr);			\
	FREAD_CHECK(read_elems, amount);

#define FWRITE(buf, elem_size, amount, file_ptr)						\
	size_t written_elems = fwrite(buf, elem_size, amount, file_ptr);	\
	FWRITE_CHECK(written_elems, amount);								\

/**
 * @brief Macro to start a loop with safety check.
 *
 * This macro initializes a global loop counter and starts a loop with a safety check against excessive iterations.
 * Usage: FOR { ... FOR_END }
 */
#define FOR						\
	GLOBAL_CYCLE_COUNTER = 0;	\
	for

/**
 * @brief Macro to end a loop with a safety check against excessive iterations.
 *
 * This macro increments the loop counter and checks if it exceeds a predefined limit.
 * If the limit is reached, it prints an error message and breaks the loop.
 * Usage: FOR { ... FOR_END }
 */
#define FOR_END																			\
	GLOBAL_CYCLE_COUNTER++;																\
	if(GLOBAL_CYCLE_COUNTER >= CYCLE_LIMIT)												\
	{																					\
		LOG("ERROR:\n\tCycle in %s on the line %d is off the limit\n",					\
				__func__, __LINE__);													\
		break;																			\
	}

#define WITH_OPEN(file_name, mode, file_ptr, ...)	\
	FILE *file_ptr = fopen(file_name, mode);		\
	FILE_PTR_CHECK(file_ptr);						\
													\
	__VA_ARGS__										\
													\
	fclose(file_ptr);

static size_t  GLOBAL_CYCLE_COUNTER      = 0; /**< Global counter for loop iterations. */
const  size_t  CYCLE_LIMIT               = 10000; /**< Limit for loop iterations. */


const unsigned char ADDITIONAL_CONCATENATION_SPACE = 2;

char * create_file_name(const char *name, const char *postfix);

int    cmp_double      (double first_double, double second_double);

// void   write_log       (const char *file_name, const char *fmt, ...);

#endif
