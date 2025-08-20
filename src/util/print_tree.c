#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "minishell.h"

/* from gemini */

// Helper function to get a string for the node type enum
const char* get_nodetype_str(bool is_terminal) {
    if (is_terminal)
        return "TERMINAL";
    return "OPERATOR";
}

// Helper function to get a string for the token type enum
const char* get_toktype_str(e_toktype type) {
    switch (type) {
        case WORD: return "WORD";
        case PIPE: return "PIPE";
        case AND: return "AND";
        case OR: return "OR";
        case SEMICOLON: return "SEMICOLON";
        case FILE_IN: return "FILE_IN";
        case FILE_OUT: return "FILE_OUT";
        case FILE_OUT_APPEND: return "FILE_OUT_APPEND";
        default: return "UNKNOWN_TOKEN";
    }
}

// Helper function to print indentation
void print_indentation(int depth) {
    for (int i = 0; i < depth; i++) {
        printf("  ");
    }
}

// Main function to print the tree structure
void print_tree_structure(t_node *node, int depth) {
    if (node == NULL) {
        return;
    }

    print_indentation(depth);
    printf("Node Type: %s\n", get_nodetype_str(node->type));

    if (node->type == OPERATOR) {
        print_indentation(depth);
        printf("Operator: %s\n", get_toktype_str(node->content.op.type));

        // Recursively print left child
        print_indentation(depth);
        printf("├── Left Child:\n");
        print_tree_structure(node->content.op.left, depth + 1);

        // Recursively print right child
        print_indentation(depth);
        printf("└── Right Child:\n");
        print_tree_structure(node->content.op.right, depth + 1);

    } else if (node->type == TERMINAL) {
        print_indentation(depth);
        printf("Tokens: ");

        t_token *current_token = node->content.terminal.tokens;
        while (current_token != NULL) {
            printf("[%s] ", current_token->txt);
            current_token = current_token->next;
        }
        printf("\n");
    }
}
