#include <stdio.h>
#include <math.h>
#include <string.h>

#include "secondary.h"

void print_binary(char *buf, size_t size, const char *buf_name,
			      void (*write_log)(const char *, ...))
{
	write_log("%s:\n", buf_name);
    for (size_t ID = 0; ID < size; ID++)
	{
		if(ID % sizeof(double) == 0)
		{
			write_log("double[%lu] - %lf\n", ID / sizeof(double), *(double *)(buf + ID));
		}

        char cur_elem = buf[ID];
		write_log("[%2.lu] - ", ID);

        for (int bit_offset = 7; bit_offset >= 0; bit_offset--)
		{
            write_log("%d", (cur_elem >> bit_offset) & 1);
        }

        write_log("\n");
    }
}

void clear_buffer(void)
{
    while(getchar() != '\n');
}

size_t max_len(const char *str_1, const char *str_2)
{
	size_t size_1 = strlen(str_1);
	size_t size_2 = strlen(str_2);
	if(size_1 > size_2)
	{
		return size_1;
	}
	else
	{
		return size_2;
	}
}
