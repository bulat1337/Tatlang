#include <stdio.h>

#include "frontend.h"
#include "midend.h"
#include "backend.h"

int main(int argc, const char *argv[])
{
	if(argc != 2)
	{
		fprintf(stderr, "ERROR: invalid amount of main function arguments(argc = %d)\n", argc);

		return EXIT_FAILURE;
	}

	frd_err_t frd_error_code = FRD_ALL_GOOD;

// Frontend
	Tokens *tokens = tokenize(argv[1], &frd_error_code);
	if(frd_error_code != FRD_ALL_GOOD)
	{
		fprintf(stderr, "tokenize error: %d.\n", frd_error_code);

		return EXIT_FAILURE;
	}

	B_tree_node *root = parse_tokens(tokens);
	if(root == NULL)
	{
		fprintf(stderr, "parse_tokens error: %d.\n", frd_error_code);

		return EXIT_FAILURE;
	}


// Midend
	mid_err_t mid_error_code = MID_ALL_GOOD;

	root = optimize(root, &mid_error_code);

	if(mid_error_code != MID_ALL_GOOD)
	{
		fprintf(stderr, "optimize error: %d.\n", mid_error_code);

		return EXIT_FAILURE;
	}

	GR_DUMP_CODE_GEN(root);


//Backend
	bkd_err_t bkd_error_code = ASSEMBLY(root);
	if(bkd_error_code != BKD_ALL_GOOD)
	{
		fprintf(stderr, "assembly error: %d.\n", mid_error_code);

		return EXIT_FAILURE;
	}




	asm_err_t asm_error = compile("root");
	if(asm_error != ASM_ALL_GOOD)
	{
		fprintf(stderr, "compile error: %d.\n", mid_error_code);

		return EXIT_FAILURE;
	}

	spu_err_t spu_error = execute("root.bin", "config", &window_draw);
	if(spu_error != SPU_ALL_GOOD)
	{
		fprintf(stderr, "execute error: %d.\n", mid_error_code);

		return EXIT_FAILURE;
	}

	return 0;
}
