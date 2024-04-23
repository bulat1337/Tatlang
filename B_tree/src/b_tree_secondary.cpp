#include "b_tree_secondary.h"
#include "utils.h"

// #define LEN(str)\
// 	sizeof(str) / sizeof(char)

void node_delete(struct B_tree_node *node)
{
	if(node == NULL)
	{
		return;
	}

	node_delete(node->left);
	node_delete(node->right);

	deallocate_node_memory(node);
}

struct B_tree_node *allocate_node_memory(void)
{
	return (struct B_tree_node *)calloc(1, sizeof(struct B_tree_node));
}

error_t deallocate_node_memory(struct B_tree_node *node)
{
	if(node == NULL)
	{
		return B_TREE_NODE_NULL_PTR;
	}

	free(node);
	node = NULL;

	return B_TREE_ALL_GOOD;
}

error_t print_regular_nodes(struct B_tree_node *node,
							struct Node_charachteristics *nd_description,
							FILE *graphic_dump_code_file_ptr)
{
	if(node == NULL)
	{
		return B_TREE_ALL_GOOD;
	}

	//fill_node_label
	switch (node->type)
	{
		case NUM:
		{
			nd_description->color = LIGHT_BLUE;

			snprintf(nd_description->label, NODE_LABEL_STR_SIZE,
					 "{%p | {type: NUM | val: %.2lf} | {L: %p | R: %p}}",
					 node, node->value.num_value, node->left, node->right);
			break;
		}
		case OP:
		{
			char *operation_token = get_operation_token(node->value.op_value);
			if(operation_token == NULL)
			{
				//log_error
				return UNABLE_TO_ALLOCATE;
			}

			nd_description->color = RED_PINK;

			snprintf(nd_description->label, NODE_LABEL_STR_SIZE,
					"{%p | {type: OP | val: %s} | {L: %p | R: %p}}",
					node, operation_token, node->left, node->right);

			free(operation_token);

			break;
		}
		case VAR:
		{
			nd_description->color = PASTEL_GREEN;

			snprintf(nd_description->label, NODE_LABEL_STR_SIZE,
					"{%p | {type: VAR | val: %s} | {L: %p | R: %p}}",
					node, node->value.var_value, node->left, node->right);
			break;
		}
		case KWD:
		{
			nd_description->color = AGRESSIVE_RED;

			snprintf(nd_description->label, NODE_LABEL_STR_SIZE,
					"{%p | {type: KWD | val: %s} | {L: %p | R: %p}}",
					node, node->value.var_value, node->left, node->right);
			break;
		}
		case IF:
		{
			nd_description->color = MUSTARD_YELLOW;

			snprintf(nd_description->label, NODE_LABEL_STR_SIZE,
					"{%p | {type: IF | val: -} | {L: %p | R: %p}}",
					node, node->left, node->right);
			break;
		}
		case WHILE:
		{
			nd_description->color = LIGHT_YELLOW;

			snprintf(nd_description->label, NODE_LABEL_STR_SIZE,
					"{%p | {type: WHILE | val: -} | {L: %p | R: %p}}",
					node, node->left, node->right);
			break;
		}
		case FUNC:
		{
			nd_description->color = SKY_BLUE;

			snprintf(nd_description->label, NODE_LABEL_STR_SIZE,
					"{%p | {type: FUNC | val: %s} | {L: %p | R: %p}}",
					node, node->value.var_value, node->left, node->right);
			break;
		}
		case SMC:
		{
			nd_description->color = NEUTRAL_GREY;

			snprintf(nd_description->label, NODE_LABEL_STR_SIZE,
					"{%p | {type: SMC | val: -} | {L: %p | R: %p}}",
					node, node->left, node->right);
			break;
		}
		case SCS:
		{
			nd_description->color = PURPLE;

			snprintf(nd_description->label, NODE_LABEL_STR_SIZE,
					"{%p | {type: SCS | val: -} | {L: %p | R: %p}}",
					node, node->left, node->right);
			break;
		}
		case SCE:
		{
			nd_description->color = DARK_PURPLE;

			snprintf(nd_description->label, NODE_LABEL_STR_SIZE,
					"{%p | {type: SCE | val: -} | {L: %p | R: %p}}",
					node, node->left, node->right);
			break;
		}
		case OBR:
		{
			nd_description->color = NEUTRAL_GREY;

			snprintf(nd_description->label, NODE_LABEL_STR_SIZE,
					"{%p | {type: OBR | val: -} | {L: %p | R: %p}}",
					node, node->left, node->right);
			break;
		}
		case CBR:
		{
			nd_description->color = NEUTRAL_GREY;

			snprintf(nd_description->label, NODE_LABEL_STR_SIZE,
					"{%p | {type: CBR | val: -} | {L: %p | R: %p}}",
					node, node->left, node->right);
			break;
		}
		case OCBR:
		{
			nd_description->color = NEUTRAL_GREY;

			snprintf(nd_description->label, NODE_LABEL_STR_SIZE,
					"{%p | {type: OCBR | val: -} | {L: %p | R: %p}}",
					node, node->left, node->right);
			break;
		}
		case CCBR:
		{
			nd_description->color = NEUTRAL_GREY;

			snprintf(nd_description->label, NODE_LABEL_STR_SIZE,
					"{%p | {type: CCBR | val: -} | {L: %p | R: %p}}",
					node, node->left, node->right);
			break;
		}
		case END:
		{
			nd_description->color = NEUTRAL_GREY;

			snprintf(nd_description->label, NODE_LABEL_STR_SIZE,
					"{%p | {type: END | val: -} | {L: %p | R: %p}}",
					node, node->left, node->right);
			break;
		}
		default:
		{
			fprintf(stderr, "Unknown node type\n");
		}
	}

	gr_dump_node(node, nd_description, graphic_dump_code_file_ptr);

	print_regular_nodes(node->left,
						nd_description,
						graphic_dump_code_file_ptr);
	print_regular_nodes(node->right,
						nd_description,
						graphic_dump_code_file_ptr);

	return B_TREE_ALL_GOOD;
}

void gr_dump_connect_nodes(struct B_tree_node *node, FILE *graphic_dump_code_file_ptr)
{
	if(node == NULL)
	{
		return;
	}

	if(node->left != NULL)
	{
		fprintf(graphic_dump_code_file_ptr, "%lu -> %lu\n", (unsigned long)node,
															(unsigned long)node->left);
		gr_dump_connect_nodes(node->left, graphic_dump_code_file_ptr);
	}

	if(node->right != NULL)
	{
		fprintf(graphic_dump_code_file_ptr, "%lu -> %lu\n", (unsigned long)node,
															(unsigned long)node->right);
		gr_dump_connect_nodes(node->right, graphic_dump_code_file_ptr);
	}
}

char *get_operation_token(enum Ops op_type)
{
	#define CASE(op_type)\
	case op_type:\
	{\
		strncpy(operation_token, #op_type, OP_TOKEN_SIZE);\
		return operation_token;\
		break;\
	}\

	char *operation_token = (char *)calloc(OP_TOKEN_SIZE, sizeof(char));
	if(operation_token == NULL)
	{
		return NULL;
	}

	switch(op_type)
	{
		CASE(DO_NOTHING)
		CASE(ADD)
		CASE(SUB)
		CASE(MUL)
		CASE(DIV)
		CASE(POW)
		CASE(LN)
		CASE(SIN)
		CASE(COS)
		CASE(SQRT)
		CASE(ASS)
		default:
		{
			strncpy(operation_token, "UNKNOWN", OP_TOKEN_SIZE);
			return operation_token;
			break;
		}
	}

	#undef CASE
}

error_t gr_dump_node(struct B_tree_node *node, struct Node_charachteristics *nd_description,
					FILE *graphic_dump_code_file_ptr)
{
	fprintf(graphic_dump_code_file_ptr, "\t%lu [shape = Mrecord, "
			"fillcolor = \"%s\", label = \"%s\" ];\n",
			(unsigned long)node, nd_description->color, nd_description->label);

	return B_TREE_ALL_GOOD;
}

void txt_dump_node(struct B_tree_node *node, FILE *console_dump_file)
{
	#define DUMP(...) fprintf(console_dump_file, __VA_ARGS__);

	if(node == NULL)
	{
		return;
	}

	DUMP("%p", node);

	switch(node->type)
	{
		case NUM:
		{
			DUMP("          NUM");
			DUMP("%18.2lf", node->value.num_value);
			break;
		}
		case OBR:
		{
			DUMP("          OBR");
			break;
		}
		case CBR:
		{
			DUMP("          CBR");
			break;
		}
		case OCBR:
		{
			DUMP("          OCBR");
			break;
		}
		case CCBR:
		{
			DUMP("          CCBR");
			break;
		}
		case SMC:
		{
			DUMP("          SMC");
			break;
		}
		case IF:
		{
			DUMP("          IF");
			break;
		}
		case WHILE:
		{
			DUMP("          WHILE");
			break;
		}
		case FUNC:
		{
			DUMP("          FUNC");
			break;
		}
		case END:
		{
			DUMP("          END");
			break;
		}
		case SCS:
		{
			DUMP("          SCS");
			break;
		}
		case SCE:
		{
			DUMP("          SCE");
			break;
		}
		case OP:
		{
			DUMP("          OP");

			#define CASE(OP_TYPE)\
			case OP_TYPE:\
			{\
				DUMP("                "#OP_TYPE);\
				break;\
			}

			switch(node->value.op_value)
			{
				CASE(ADD)
				CASE(SUB)
				CASE(MUL)
				CASE(DIV)
				CASE(POW)
				CASE(LN)
				CASE(SIN)
				CASE(COS)
				CASE(SQRT)
				CASE(ASS)
				CASE(DO_NOTHING)
				default:
				{
					DUMP("UNKNOWN");
				}
			}

			#undef CASE
			break;
		}
		case VAR:
		{
			DUMP("          VAR");
			DUMP("               %s", node->value.var_value);
			break;
		}
		case KWD:
		{
			DUMP("          KWD");
			DUMP("               %s", node->value.var_value);
			break;
		}
		default:
		{
			DUMP("UNKNOWN");
		}
	}

	DUMP("%27.p", node->left);
	DUMP("%18.p", node->right);

	DUMP("\n");

	txt_dump_node(node->left,  console_dump_file);
	txt_dump_node(node->right, console_dump_file);
}

error_t compile_dot(const char *b_tree_name)
{
	char *gr_dump_gen_cmd = (char *)calloc(GR_DUMP_GEN_CMD_SIZE, sizeof(char));

	if(gr_dump_gen_cmd == NULL)
	{
		fprintf(stderr, "Unable to allocate\n");
		return UNABLE_TO_ALLOCATE;
	}
	snprintf(gr_dump_gen_cmd, GR_DUMP_GEN_CMD_SIZE,
		"dot -Tpng %s.dot -o %s.png -Gdpi=100\n", b_tree_name, b_tree_name);

	system(gr_dump_gen_cmd);

	free(gr_dump_gen_cmd);

	return B_TREE_ALL_GOOD;
}

bool exists(const char *file_name)
{
    FILE *file = NULL;
    if ((file = fopen(file_name, "r")))
    {
        fclose(file);
        return true;
    }
    return false;
}
