#ifndef B_TREE_SECONDARY_H
#define B_TREE_SECONDARY_H

#include "b_tree.h"

void         node_delete           (B_tree_node *node);

void         gr_dump_connect_nodes (B_tree_node *node,
									FILE *graphic_dump_code_file_ptr);

void         txt_dump_node         (B_tree_node *node, FILE *console_dump_file);

B_tree_node *allocate_node_memory  (void);

error_t      deallocate_node_memory(B_tree_node *node);

error_t      print_regular_nodes   (B_tree_node *node,
									Node_charachteristics *nd_description,
							        FILE *graphic_dump_code_file_ptr);

error_t      dump_node             (B_tree_node *node,
									Node_charachteristics *nd_description,
				                    FILE *graphic_dump_code_file_ptr);

error_t      gr_dump_node          (B_tree_node *node,
									Node_charachteristics *nd_description,
					                FILE *graphic_dump_code_file_ptr);
									
error_t      compile_dot           (const char *b_tree_name);

char        *get_operation_token   (enum Ops op_type);


#endif
