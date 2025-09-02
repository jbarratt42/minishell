/* src/debug/print_tree.c */
const char *get_nodetype_str(_Bool is_terminal);
const char *get_toktype_str(enum e_token_type type);
void print_indentation(int depth);
void print_tree_structure(t_node *node, int depth);
