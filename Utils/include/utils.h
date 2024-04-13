#ifndef UTILS_H
#define UTILS_H

#define WITH_OPEN(file_name, mode, file_ptr, ...)	\
	FILE *file_ptr = fopen(file_name, mode);		\
	FILE_PTR_CHECK(file_ptr);						\
													\
	__VA_ARGS__										\
													\
	fclose(file_ptr);


const unsigned char ADDITIONAL_CONCATENATION_SPACE = 2;

char * create_file_name(const char *name, const char *postfix);

int    cmp_double      (double first_double, double second_double);

#endif
