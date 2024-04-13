#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdarg.h>
#include <math.h>

#include "utils.h"

char *create_file_name(const char *name, const char *postfix)
{
	size_t byte_code_file_name_size =
		strlen(postfix) + strlen(name) + ADDITIONAL_CONCATENATION_SPACE;

	char *byte_code_file_name =
		(char *)calloc(byte_code_file_name_size, sizeof(char));

	snprintf(byte_code_file_name, byte_code_file_name_size, "%s%s", name, postfix);

	return byte_code_file_name;
}

int cmp_double(double first_double, double second_double)
{
    const double eps = 1e-7;

    if(isnan(first_double) && isnan(second_double))
    {
        return 0;
    }
    if (fabs(first_double - second_double) < eps)
    {
        return 0;
    }
    else if ((first_double - second_double) > eps)
    {
        return 1;
    }
    else
    {
        return -1;
    }
}

void write_log(const char *file_name, const char *fmt, ...)
{
    static FILE *log_file = fopen(file_name, "w");

    if (log_file == NULL)
	{
        perror("Error opening log_file");
        return;
    }

    va_list args = NULL;

    va_start(args, fmt);

	// fprintf(log_file, "file: %s func: %s on line : %d\n", file_name, func_name, line);
    vfprintf(log_file, fmt, args);

    va_end(args);
}
