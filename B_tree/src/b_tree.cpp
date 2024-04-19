#include "b_tree.h"
#include "b_tree_secondary.h"
#include "utils.h"

Uni_ret create_node(Node_type type, Node_value value, B_tree_node *left_child,
													  B_tree_node *right_child)
{
	Uni_ret result =
	{
		.arg.node = allocate_node_memory(),
		.error_code = B_TREE_ALL_GOOD,
	};

	if(result.arg.node == NULL)
	{
		result.error_code = UNABLE_TO_ALLOCATE;
		return result;
	}

	result.arg.node->type  = type;

	result.arg.node->left  = left_child;
	result.arg.node->right = right_child;

	result.arg.node->value = value;

	return result;
}

error_t add_child(struct B_tree_node *parent, struct B_tree_node *child, bool is_right_child)
{
	if(parent == NULL || child == NULL)
	{
		return B_TREE_NODE_NULL_PTR;
	}

	if(is_right_child)
	{
		parent->right = child;
	}
	else
	{
		parent->left  = child;
	}

	return B_TREE_ALL_GOOD;
}

error_t destroy_subtree(struct B_tree_node *parent_node, bool is_right_child)
{
	if(parent_node == NULL)
	{
		return PARENT_NODE_IS_FREE;
	}

	if(is_right_child)
	{
		node_delete(parent_node->right);
		parent_node->right = NULL;
	}
	else
	{
		node_delete(parent_node->left);
		parent_node->left = NULL;
	}


	return B_TREE_ALL_GOOD;
}

Uni_ret gr_dump_code_gen(B_tree_node *root, const char *b_tree_name)
{
	char *file_name = create_file_name(b_tree_name, ".dot");
	Uni_ret result =
	{
		.error_code   = B_TREE_ALL_GOOD,
		.arg.file_ptr = fopen(file_name, "w"),
	};
	free(file_name);

	if(result.arg.file_ptr == NULL)
	{
		fprintf(stderr, "Unable to open list_graphic_dump.dot\n");

		result.error_code = B_TREE_UNABLE_TO_OPEN_FILE;
		return result;
	}

	#define WRITE_TO_DUMP_FILE(...) fprintf(result.arg.file_ptr, __VA_ARGS__);

	WRITE_TO_DUMP_FILE("digraph BinaryTree {\n"
	"bgcolor = \"%s\";\n"
	"edge[minlen = 3, penwidth = 3; color = \"black\"];\n"
	"node[shape = \"rectangle\", style = \"rounded, filled\",\n"
	"\tfillcolor = \"%s\",\n"
	"\tfontsize = 30,\n"
	"\theight = 3,\n"
	"\tpenwidth = 5, color = \"white\"];\n", MARGENGO, CORAL_PINK);

	struct Node_charachteristics nd_description =
	{
		.color = CORAL_PINK,
		.name = "node_",
	};

	nd_description.label = (char *)calloc((NODE_LABEL_STR_SIZE), sizeof(char));
	if(nd_description.label == NULL)
	{
		fprintf(stderr, "Unable to allocate nd_description.label\n");

		result.error_code = UNABLE_TO_ALLOCATE;

		return result;
	}

	//	print_list_manager
	WRITE_TO_DUMP_FILE("{rank = min;\n"
    	"\tlist_manager [shape = Mrecord, fillcolor = \"%s\", "
		"label = \"{ROOT: %p}\"];\n"
	"}\n", LIGHT_GREEN, root);

	if(root != NULL)
	{
		result.error_code = print_regular_nodes(root, &nd_description, result.arg.file_ptr);

		if(result.error_code != B_TREE_ALL_GOOD)
		{
			free(nd_description.label);
			fclose(result.arg.file_ptr);

			return result;
		}
	}

	gr_dump_connect_nodes(root, result.arg.file_ptr);

	WRITE_TO_DUMP_FILE("}");

	#undef WRITE_TO_DUMP_FILE

	free(nd_description.label);
	fclose(result.arg.file_ptr);

	result.error_code = compile_dot(b_tree_name);

	if(result.error_code != B_TREE_ALL_GOOD)
	{
		return result;
	}


	return result;
}

error_t txt_dump(struct B_tree_node *root, const char *name)
{
	#define DUMP(...) fprintf(dump_file, __VA_ARGS__);

	char *file_name = create_file_name(name, "_dump.txt");

	FILE *dump_file = fopen(file_name, "w");
	free(file_name);
	if(dump_file == NULL)
	{
		return B_TREE_UNABLE_TO_OPEN_FILE;
	}

	DUMP("address:");
	DUMP("             type:");
	DUMP("             value:");
	DUMP("             left:");
	DUMP("             right:\n");

	txt_dump_node(root, dump_file);

	#undef DUMP

	return B_TREE_ALL_GOOD;
}

// struct Construct_b_tree_result construct_b_tree(const char *data_base_file_name)
// {
// 	WRITE_IN_LOG_FILE("construct_b_tree log:\n");
// 	Construct_b_tree_result result =
// 	{
// 		.error_code = B_TREE_ALL_GOOD,
// 	};
//
// 	FILE *data_base = fopen(data_base_file_name, "r");
// 	if (data_base == NULL)
// 	{
// 		fprintf(stderr, "I cant open ur data base(%s)!\n", data_base_file_name);
// 		result.error_code = AKI_B_TREE_UNABLE_TO_OPEN_FILE;
//
// 		return result;
// 	}
//
// 	struct Buf_w_carriage_n_len data_base_buf_w_info = {};
//
// 	data_base_buf_w_info.length = get_file_length(data_base);
// 	data_base_buf_w_info.buf = (char *)calloc(data_base_buf_w_info.length, sizeof(char));
// 	if(data_base_buf_w_info.buf == NULL)
// 	{
// 		perror("ERROR:");
// 		result.error_code = UNABLE_TO_ALLOCATE;
// 		return result;
// 	}
//
// 	size_t read_elems_amount =
// 		fread(data_base_buf_w_info.buf, sizeof(char), data_base_buf_w_info.length, data_base);
// 	if(read_elems_amount != data_base_buf_w_info.length)
// 	{
// 		if(ferror(data_base))
// 		{
// 			perror("ERROR:");
//
// 			result.error_code = UNEXPECTED_WRITTEN_ELEMS;
//
// 			return result;
// 		}
// 		else if(feof(data_base))
// 		{
// 			fprintf(stderr ,
// 			"read_elems_amount != data_base_buf_w_info.length because end of the file "
// 			"EOF was reached.\n");
// 		}
// 	}
//
// 	size_t amount_of_lexems = count_buf_lexemes(data_base_buf_w_info);
// 	struct Lexemes_w_carriage data_base_lexemes =
// 	{
// 		.buf = (char * *)calloc(amount_of_lexems, sizeof(char *)),
// 		.carriage = 0,
// 	};
//
// 	if(data_base_lexemes.buf == NULL)
// 	{
// 		perror("ERROR:");
// 		result.error_code = UNABLE_TO_ALLOCATE;
// 		return result;
// 	}
//
// 	lexemes_ptr_arranger(data_base_lexemes.buf, data_base_buf_w_info);
//
// 	size_t amount_of_nodes = get_amount_of_nodes(data_base_lexemes.buf, amount_of_lexems);
//
//
//
// 	result.btr = b_tree_ctor(amount_of_nodes + SPACE_FOR_NEW_NODES).new_btr;
//
// 	int current_node_ID = 0;
// 	result.btr->root = &(result.btr->node[current_node_ID]);
// 	result.btr->capacity = amount_of_nodes + SPACE_FOR_NEW_NODES;
// 	read_node(&data_base_lexemes, &(current_node_ID), result.btr->node);
//
// 	result.btr->current_free = &(result.btr->node[current_node_ID + 1]);
//
// 	free(data_base_lexemes.buf);
// 	free(data_base_buf_w_info.buf);
//
// 	fclose(data_base);
//
// 	return result;
// }

//оптимизация

bool cmp_nodes(B_tree_node *node_1, B_tree_node *node_2)
{
	if(node_1 == NULL && node_2 == NULL)
	{
		return true;
	}

	if(node_1 == NULL || node_2 == NULL)
	{
		return false;
	}

	if(	cmp_nodes(node_1->left, node_2->left) &&
		cmp_nodes(node_1->right, node_2->right) &&
		(
			(	(node_1->type == NUM && node_2->type == NUM) &&
				(!cmp_double(node_1->value.num_value, node_2->value.num_value)	)	) ||
			(	(node_1->type == OP && node_2->type == OP) &&
				(node_1->value.op_value == node_2->value.op_value	)	) ||
			(	(node_1->type == VAR && node_2->type == VAR) &&
				(!strncmp(node_1->value.var_value, node_2->value.var_value, strlen(node_2->value.var_value))	)	)
		)	)
	{
		return true;
	}
	else
	{
		return false;
	}
}
