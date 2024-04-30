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
		case UNR_OP:
		{
			char *operation_token = get_operation_token(node->value.op_value);
			if(operation_token == NULL)
			{
				//log_error
				return UNABLE_TO_ALLOCATE;
			}

			nd_description->color = RASPBERRIE_PINK;

			snprintf(nd_description->label, NODE_LABEL_STR_SIZE,
					"{%p | {type: UNR_OP | val: %s} | {L: %p | R: %p}}",
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
		case KEYWORD:
		{
			nd_description->color = AGRESSIVE_RED;

			snprintf(nd_description->label, NODE_LABEL_STR_SIZE,
					"{%p | {type: KEYWORD | val: %s} | {L: %p | R: %p}}",
					node, node->value.var_value, node->left, node->right);
			break;
		}
		case MAIN:
		{
			nd_description->color = AGRESSIVE_RED;

			snprintf(nd_description->label, NODE_LABEL_STR_SIZE,
					"{%p | {type: MAIN | val: -} | {L: %p | R: %p}}",
					node, node->left, node->right);
			break;
		}
		case FUNC:
		{
			nd_description->color = BLUE;

			snprintf(nd_description->label, NODE_LABEL_STR_SIZE,
					"{%p | {type: FUNC | val: %s} | {L: %p | R: %p}}",
					node, node->value.var_value, node->left, node->right);
			break;
		}
		case FUNC_DECL:
		{
			nd_description->color = FOX_ORANGE;

			snprintf(nd_description->label, NODE_LABEL_STR_SIZE,
					"{%p | {type: FUNC_DECL | val: %s} | {L: %p | R: %p}}",
					node, node->value.var_value, node->left, node->right);
			break;
		}
		case RETURN:
		{
			nd_description->color = GREEN;

			snprintf(nd_description->label, NODE_LABEL_STR_SIZE,
					"{%p | {type: RETURN | val: -} | {L: %p | R: %p}}",
					node, node->left, node->right);
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
		case STD_FUNC:
		{
			char *func_type = get_func_token(node->value.func);
			if(func_type == NULL)
			{
				//log_error
				return UNABLE_TO_ALLOCATE;
			}

			nd_description->color = SKY_BLUE;

			snprintf(nd_description->label, NODE_LABEL_STR_SIZE,
					"{%p | {type: STD_FUNC | val: %s} | {L: %p | R: %p}}",
					node, func_type, node->left, node->right);

			free(func_type);

			break;
		}
		case SEMICOLON:
		{
			nd_description->color = NEUTRAL_GREY;

			snprintf(nd_description->label, NODE_LABEL_STR_SIZE,
					"{%p | {type: SEMICOLON | val: -} | {L: %p | R: %p}}",
					node, node->left, node->right);
			break;
		}
		case COMMA:
		{
			nd_description->color = LIGHT_GREY;

			snprintf(nd_description->label, NODE_LABEL_STR_SIZE,
					"{%p | {type: COMMA | val: -} | {L: %p | R: %p}}",
					node, node->left, node->right);
			break;
		}
		case SCOPE_START:
		{
			nd_description->color = PURPLE;

			snprintf(nd_description->label, NODE_LABEL_STR_SIZE,
					"{%p | {type: SCOPE_START | val: -} | {L: %p | R: %p}}",
					node, node->left, node->right);
			break;
		}
		case SCOPE_END:
		{
			nd_description->color = DARK_PURPLE;

			snprintf(nd_description->label, NODE_LABEL_STR_SIZE,
					"{%p | {type: SCOPE_END | val: -} | {L: %p | R: %p}}",
					node, node->left, node->right);
			break;
		}
		case OPEN_BR:
		{
			nd_description->color = NEUTRAL_GREY;

			snprintf(nd_description->label, NODE_LABEL_STR_SIZE,
					"{%p | {type: OPEN_BR | val: -} | {L: %p | R: %p}}",
					node, node->left, node->right);
			break;
		}
		case DECLARE:
		{
			nd_description->color = NEUTRAL_GREY;

			snprintf(nd_description->label, NODE_LABEL_STR_SIZE,
					"{%p | {type: DECLARE | val: -} | {L: %p | R: %p}}",
					node, node->left, node->right);
			break;
		}
		case CLOSE_BR:
		{
			nd_description->color = NEUTRAL_GREY;

			snprintf(nd_description->label, NODE_LABEL_STR_SIZE,
					"{%p | {type: CLOSE_BR | val: -} | {L: %p | R: %p}}",
					node, node->left, node->right);
			break;
		}
		case OPEN_CBR:
		{
			nd_description->color = NEUTRAL_GREY;

			snprintf(nd_description->label, NODE_LABEL_STR_SIZE,
					"{%p | {type: OPEN_CBR | val: -} | {L: %p | R: %p}}",
					node, node->left, node->right);
			break;
		}
		case CLOSE_CBR:
		{
			nd_description->color = NEUTRAL_GREY;

			snprintf(nd_description->label, NODE_LABEL_STR_SIZE,
					"{%p | {type: CLOSE_CBR | val: -} | {L: %p | R: %p}}",
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
		case OPEN_BR:
		{
			DUMP("          OPEN_BR");
			break;
		}
		case CLOSE_BR:
		{
			DUMP("          CLOSE_BR");
			break;
		}
		case OPEN_CBR:
		{
			DUMP("          OPEN_CBR");
			break;
		}
		case CLOSE_CBR:
		{
			DUMP("          CLOSE_CBR");
			break;
		}
		case SEMICOLON:
		{
			DUMP("          SEMICOLON");
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
		case STD_FUNC:
		{
			DUMP("          STD_FUNC");
			break;
		}
		case FUNC:
		{
			DUMP("          FUNC");
			break;
		}
		case COMMA:
		{
			DUMP("          COMMA");
			break;
		}
		case DECLARE:
		{
			DUMP("          DECLARE");
			break;
		}
		case RETURN:
		{
			DUMP("          RETURN");
			break;
		}
		case END:
		{
			DUMP("          END");
			break;
		}
		case SCOPE_START:
		{
			DUMP("          SCOPE_START");
			break;
		}
		case SCOPE_END:
		{
			DUMP("          SCOPE_END");
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
		case UNR_OP:
		{
			DUMP("          UNR_OP");

			#define CASE(OP_TYPE)\
			case OP_TYPE:\
			{\
				DUMP("                "#OP_TYPE);\
				break;\
			}

			switch(node->value.op_value)
			{
				CASE(LN)
				CASE(SIN)
				CASE(COS)
				CASE(SQRT)
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
		case KEYWORD:
		{
			DUMP("          KEYWORD");
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

char *get_func_token(Std_func func_type)
{
	#define CASE(func_type)										\
	case func_type:												\
	{															\
		strncpy(func_token, #func_type, STD_FUNC_TOKEN_SIZE);	\
		return func_token;										\
		break;													\
	}

	char *func_token = (char *)calloc(STD_FUNC_TOKEN_SIZE, sizeof(char));
	if(func_token == NULL)
	{
		return NULL;
	}

	switch(func_type)
	{
		CASE(GETVAR)
		CASE(PUTEXPR)
		default:
		{
			strncpy(func_token, "UNKNOWN", STD_FUNC_TOKEN_SIZE);
			return func_token;
			break;
		}
	}

	#undef CASE
}
