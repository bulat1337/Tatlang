#include <stdio.h>

#include "frontend.h"
#include "midend.h"
#include "backend.h"

int main(int argc, const char *argv[])
{
	if(argc > 2)
	{
		fprintf(stderr, "ERROR: invalid amount of main function arguments(argc = %d)\n", argc);

		return EXIT_FAILURE;
	}

	frd_err_t error = FRD_ALL_GOOD;


// Frontend
	Tokens *tokens = tokenize(argv[1], error);

	B_tree_node *root = parse_tokens(tokens);


// Midend
	root = optimize(root);
	GR_DUMP_CODE_GEN(root);





// 	asm_err_t asm_error = compile("prog");
//
// 	spu_err_t spu_error = execute("prog.bin", "config", &window_draw);

	if(error != FRD_ALL_GOOD)
	{
		printf("error: %d", error);
	}

	return 0;
}
