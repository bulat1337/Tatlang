#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>

#include "utils.h"

char *create_file_name(const char *name, const char *postfix)
{
	size_t byte_code_file_name_size =
		strlen(postfix) + strlen(name) + ADDITIONAL_CONCATENATION_SPACE;

	char *byte_code_file_name =
		(char *)calloc(byte_code_file_name_size, sizeof(char));
	if(byte_code_file_name == NULL)
	{
		return NULL;
	}

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

