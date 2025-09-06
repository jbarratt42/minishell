/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lex.h                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: chuezeri <chuezeri@student.42.de>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/24 17:16:07 by chuezeri          #+#    #+#             */
/*   Updated: 2025/09/02 12:37:40 by jbarratt         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef LEX_H
#define LEX_H

typedef enum e_token_type
{
    EOF_T,
    ERROR,
    WORD,
    REDIR_IN,     // <  (input redirection)
    REDIR_OUT,    // >  (output redirection)
    REDIR_APPEND, // >> (output append)
    HEREDOC,      // << (here document)
    SQUOTE,       // '  (single quote)
    DQUOTE,       // "  (double quote)
    PIPE,         // |  (pipe)
    AND,          // &  (AND)
    OR,           // ||  (OR)
    SEMICOLON,    // ;  (statement separator)
	OPEN,
	CLOSE,
	MAX_TOKEN_TYPE
} t_token_type;

typedef struct s_token
{
    int pos;
    char *value;
    t_token_type type;
    struct s_token *prev;
    struct s_token *next;
} t_token;

/**
 * @brief Tokenize the input string into a list of tokens.
 * @param input The input string to tokenize.
 * @return A pointer to the head of the token list, or NULL on failure.
 */
t_token *lex(const char *input);
/**
 * @brief Free the memory allocated for the token list.
 * @param tok The token list to free.
 */
void free_tokens(t_token *tok);

/**
 * @brief Print the tokens in a human-readable format.
 * @param tok The token list to print.
 */
void print_tokens(const t_token *tok);

/**
 * @brief free all malloc'd elements of context.  should include a
 * readline'd line, a token list and a parse tree;
 */
void free_context(t_context *context);

/**
 * @brief expand all variables and positional parameters and store the
 * updated input line in a new string, freeing the original
 */
bool expand(t_context *context);

/**
 * @brief Get the string representation of a token type.
 * @param type The token type to convert.
 * @return A string representation of the token type.
 */
const char *token_type_str(t_token_type type);

#endif
