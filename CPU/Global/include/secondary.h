#ifndef SECONDARY_H
#define SECONDARY_H

/**
 * @file secondary.h
 * @brief Header file containing declarations of secondary functions.
 */

#include <stdarg.h>

#define WITH_OPEN(file_name, mode, file_ptr, ...)	\
	FILE *file_ptr = fopen(file_name, mode);		\
	FILE_PTR_CHECK(file_ptr);						\
													\
	__VA_ARGS__										\
													\
	fclose(file_ptr);

const  char    RAM_MASK                  = (const char)(1 << 7);
const  char    REG_MASK                  = (const char)(1 << 6);
const  char    IMM_MASK                  = (const char)(1 << 5);


/**
 * @brief Prints the binary representation of a buffer.
 *
 * Prints the binary representation of the specified buffer, interpreting its contents as individual bytes.
 *
 * @param buf Pointer to the buffer.
 * @param size Size of the buffer.
 * @param buf_name Name of the buffer for identification purposes.
 */
void print_binary(char *buf, size_t size, const char *buf_name,
				  void (*write_log)(const char *, ...));

/**
 * @brief Clears the input buffer.
 *
 * Clears the input buffer by reading and discarding characters until a newline character is encountered.
 */
void clear_buffer(void);

/**
 * @brief Returns the maximum length between two strings.
 *
 * This function calculates the length of two given strings and returns the
 * maximum length between them.
 *
 * @param str_1 Pointer to the first string.
 * @param str_2 Pointer to the second string.
 * @return The maximum length between str_1 and str_2.
 */
size_t max_len(const char *str_1, const char *str_2);

#endif
