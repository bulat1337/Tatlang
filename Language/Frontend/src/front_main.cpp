#include <stdio.h>

#include "frontend.h"

int main(int argc, const char *argv[])
{
	if(argc > 2)
	{
		fprintf(stderr, "ERROR: invalid amount of main function arguments(argc = %d)\n", argc);

		return EXIT_FAILURE;
	}

	frd_err_t error = FRD_ALL_GOOD;

	Tokens *tokens = tokenize(argv[1], error);

	B_tree_node *root = parse_tokens(tokens);

	GR_DUMP_CODE_GEN(root);

	// check if correct

	if(error != FRD_ALL_GOOD)
	{
		printf("error: %d", error);
	}

	return 0;
}
