#ifndef DEF_TRIV_DSL_H
#define DEF_TRIV_DSL_H

#define LEFT_VAR												\
	node->left->value.var_value

#define RIGHT_VAR												\
	node->right->value.var_value

#define MAX_VAR_LEN												\
	max_len(strlen(LEFT_VAR), strlen(RIGHT_VAR))

#define LEFT_IS_ZERO\
	node->left->type == NUM && cmp_double(node->left->value.num_value, 0) == 0

#define ZERO__MUL_DIV_POW__ANY				\
	(node->type == OP) &&					\
	(	(node->value.op_value == MUL)	||	\
		(node->value.op_value == DIV)	||	\
		(node->value.op_value == POW)	)

#define ZERO__ADD__ANY\
	node->type == OP && node->value.op_value == ADD

#define RIGHT_IS_ZERO\
	node->right->type == NUM && cmp_double(node->right->value.num_value, 0) == 0

#define ANY__MUL__ZERO\
	node->type == OP && node->value.op_value == MUL

#define ANY__POW__ZERO\
	node->type == OP && node->value.op_value == POW

#define ANY__DIV__ZERO				\
	(node->type == OP) &&			\
	(node->value.op_value == DIV)

#define ANY__ADD_SUB__ZERO					\
	(node->type == OP) &&					\
	(	(node->value.op_value == ADD)	||	\
		(node->value.op_value == SUB)	)
#define LEFT_IS_ONE\
	node->left->type == NUM && cmp_double(node->left->value.num_value, 1) == 0

#define ONE__MUL__ANY\
	node->type == OP && node->value.op_value == MUL

#define ONE__POW__ANY\
	node->type == OP && node->value.op_value == POW

#define RIGHT_IS_ONE\
	node->right->type == NUM && cmp_double(node->right->value.num_value, 1) == 0

#define ANY__MUL_POW_DIV__ONE				\
	(node->type == OP) &&					\
	(	(node->value.op_value == MUL) ||	\
		(node->value.op_value == POW) ||	\
		(node->value.op_value == DIV)	)

#define VAR__SUB__VAR												\
	(node->left->type == VAR) &&									\
			(node->right->type == VAR) &&							\
			(node->type == OP && node->value.op_value == SUB) &&	\
			(!strncmp(LEFT_VAR, RIGHT_VAR, MAX_VAR_LEN))

#define ZERO													\
	create_node(NUM, {.num_value = 0}, NULL, NULL).arg.node

#define ONE														\
	create_node(NUM, {.num_value = 1}, NULL, NULL).arg.node


#endif
