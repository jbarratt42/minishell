#include "minishell.h"

// ** begin Gemini-generated **
// --- Helper Functions for Printing ---

/**
 * @brief Returns a string representation of an e_nodetype.
 * @param type The e_nodetype enum value.
 * @return A constant string for the node type.
 */
const char* get_nodetype_str(bool is_terminal) {
	/*
    switch (type) {
        case OPERATOR: return "OPERATOR";
        case AND:  return "TERMINAL";
        default:       return "UNKNOWN_NODETYPE";
    }
	*/
	if (is_terminal)
		return "TERMINAL";
	else
		return "OPERATOR";
}

/**
 * @brief Returns a string representation of an e_toktype (for operators).
 * @param type The e_toktype enum value.
 * @return A constant string for the token type.
 */
const char* get_toktype_str(enum e_token_type type) {
    switch (type) {
        case WORD:      return "WORD";
        case OPEN:      return "OPEN_PAREN";
        case CLOSE:     return "CLOSE_PAREN";
        case PIPE:      return "PIPE";
        case AND:       return "AND (&&)";
        case OR:        return "OR (||)";
        case SEMICOLON: return "SEMICOLON (;)";
        default:        return "UNKNOWN_TOKTYPE";
    }
}

/**
 * @brief Prints indentation for tree visualization.
 * @param depth The current depth in the tree.
 */
void print_indentation(int depth) {
    for (int i = 0; i < depth; i++) {
        printf("  "); // Two spaces per depth level
    }
}

/**
 * @brief Recursively prints the structure of a t_node tree.
 * @param node A pointer to the current t_node.
 * @param depth The current depth of the node in the tree (start with 0 for root).
 */
void print_tree_structure(t_node *node, int depth) {
    if (node == NULL) {
        return;
    }

    print_indentation(depth);
    printf("Node Type: %s\n", get_nodetype_str(node->is_terminal));

    if (node->is_terminal) {
        print_indentation(depth);
        printf("  Command Tokens: ");
        t_token *current_token = node->data.tokens;
        if (current_token == NULL) {
            printf("[EMPTY]\n");
        } else {
            while (current_token != NULL && current_token->type < PIPE) {
                printf("'%s' ", current_token->value);
                current_token = current_token->next;
            }
            printf("\n");
        }
    } else {
        print_indentation(depth);
        printf("  Operator Type: %s\n", get_toktype_str(node->data.op.type));

        print_indentation(depth);
        printf("  Left Child:\n");
        print_tree_structure(node->data.op.left, depth + 1);

        print_indentation(depth);
        printf("  Right Child:\n");
        print_tree_structure(node->data.op.right, depth + 1);
	}
		/*
    } else {
        print_indentation(depth);
        printf("  (Invalid Node Type)\n");
    }
	*/
}
// end Gemini-generated

/*
int	main(int argc, char **argv)
{
	t_node	*tree;
	t_token	*tokens;
	
	if (argc != 2)
		return (1);
	tokens = lex(argv[1], ' ');
	tree = parse(&tokens, 0);
	if (!tree)
	{
		free_token(tokens);
		return (1);
	}
	print_tree_structure(tree, 0);
	return (0);
}
*/
