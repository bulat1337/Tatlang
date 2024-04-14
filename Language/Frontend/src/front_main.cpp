#include <stdio.h>

#include "frontend.h"
#include "b_tree.h"

int main(int argc, const char *argv[])
{
	if(argc > 2)
	{
		fprintf(stderr, "ERROR: invalid amount of main function arguments(argc = %d)\n", argc);

		return EXIT_FAILURE;
	}

	frd_err_t error = tokenize(argv[1]);

	if(error != FRD_ALL_GOOD)
	{
		printf("error: %d", error);
	}

	return 0;
}
