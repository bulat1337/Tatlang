#include "midend_secondary.h"

extern bool change_flag;

B_tree_node *simplify(B_tree_node *root)
{
	if(root == NULL)
	{
		return root;
	}

	root->left  = simplify(root->left);
	root->right = simplify(root->right);

	root = fold_consts(root);
	root = solve_trivial(root);

	return root;
}

B_tree_node *fold_consts(B_tree_node *node)
{
	if(node == NULL)
	{
		return NULL;
	}

	if(node->left == NULL || node->right == NULL)
	{
		return node;
	}

	if(node->left->type == NUM && node->right->type == NUM)
	{
		btr_elem_t result = eval(node);

		change_flag = true;

		return create_node(NUM, {.num_value = result}, NULL, NULL).arg.node;
	}
	else
	{
		return node;
	}
}

btr_elem_t eval(B_tree_node *node)
{
	if(node == NULL)
	{
		return NAN;
	}

	if(node->type == NUM)
	{
		return node->value.num_value;
	}

	btr_elem_t left_node_value  = eval(node->left);
	btr_elem_t right_node_value = eval(node->right);

	switch(node->value.op_value)
	{
		case ADD:
		{
			return left_node_value + right_node_value;
			break;
		}
		case SUB:
		{
			return left_node_value - right_node_value;
			break;
		}
		case MUL:
		{
			return left_node_value * right_node_value;
			break;
		}
		case DIV:
		{
			if(cmp_double(right_node_value, 0) == 0)
			{
				return NAN;
			}

			return left_node_value / right_node_value;

			break;
		}
		case POW:
		{
			return pow(left_node_value, right_node_value);
		}
		case LN:
		{
			return log(right_node_value);
		}
		case SIN:
		{
			return sin(right_node_value);
		}
		case COS:
		{
			return cos(right_node_value);
		}
		case SQRT:
		{
			return sqrt(right_node_value);
		}
		case DO_NOTHING:
		{
			return NAN;
			break;
		}
		default:
		{
			return NAN;
		}

	}

}

#include "def_triv_dsl.h"

B_tree_node *solve_trivial(B_tree_node *node)
{
	if(node == NULL)
	{
		return NULL;
	}

	if(node->left == NULL || node->right == NULL)
	{
		return node;
	}

	if(LEFT_IS_ZERO)
	{
		if(ZERO__MUL_DIV_POW__ANY)
		{
			change_flag = true;
			return ZERO;
		}
		else if(ZERO__ADD__ANY)
		{
			change_flag      = true;
			return node->right; // might be simplified further
		}
	}
	else if(RIGHT_IS_ZERO)
	{
		if(ANY__MUL__ZERO)
		{
			change_flag = true;
			return ZERO;
		}
		else if(ANY__POW__ZERO)
		{
			change_flag = true;
			return ONE;
		}
		else if	(ANY__DIV__ZERO)
		{
			change_flag = true;
			return create_node(NUM, {.num_value = NAN}, NULL, NULL).arg.node;
		}
		else if(ANY__ADD_SUB__ZERO)
		{
			change_flag      = true;
			return node->left; // might be simplified further
		}
	}
	else if(LEFT_IS_ONE)
	{
		if(ONE__MUL__ANY)
		{
			change_flag      = true;
			return node->right;	// might be simplified further
		}
		else if(ONE__POW__ANY)
		{
			change_flag = true;
			return ONE;
		}
	}
	else if(RIGHT_IS_ONE)
	{
		if(ANY__MUL_POW_DIV__ONE)
		{
			change_flag      = true;
			return node->left;	// might be simplified further
		}
	}
	else if(VAR__SUB__VAR)
	{
		change_flag = true;
		return ZERO;
	}
// 	else if(SAME_OP_W_CONSTS)
//
// 	#define SAME_OP_W_CONSTS\
// 	left or right is const
// 		and the other one is op
// 			and left or right of the op is the same op
// 				and left or right child of the ops op is const


	return node;
}

#include "undef_triv_dsl.h"

