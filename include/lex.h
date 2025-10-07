/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lex.h                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: chuezeri <chuezeri@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/24 17:16:07 by chuezeri          #+#    #+#             */
/*   Updated: 2025/10/07 13:27:52 by chuezeri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef LEX_H
# define LEX_H

typedef enum e_token_type
{
	EOF_T,
	ERROR,
	WORD,
	REDIR_IN,
	HEREDOC,
	REDIR_OUT,
	REDIR_APPEND,
	SQUOTE,
	DQUOTE,
	PIPE,
	AND,
	OR,
	SEMICOLON,
	OPEN,
	CLOSE,
	MAX_TOKEN_TYPE
}					t_token_type;

typedef struct s_token
{
	int				pos;
	char			*value;
	t_token_type	type;
	struct s_token	*prev;
	struct s_token	*next;
}					t_token;

/**
 * @brief Tokenize the input string into a list of tokens.
 * @param input The input string to tokenize.
 * @return A pointer to the head of the token list, or NULL on failure.
 */
t_token				*lex(const char *input);
/**
 * @brief Free the memory allocated for the token list.
 * @param tok The token list to free.
 */
void				free_tokens(t_token *tok);

/**
 * @brief Print the tokens in a human-readable format.
 * @param tok The token list to print.
 */
void				print_tokens(const t_token *tok);

/**
 * @brief free all malloc'd elements of context.  should include a
 * readline'd line, a token list and a parse tree;
 */
void				free_context(t_context *context);

/**
 * @brief expand all variables and positional parameters and store the
 * updated input line in a new string, freeing the original
 */
char				*expand(char *str, t_context *context);

/**
 * @brief Get the string representation of a token type.
 * @param type The token type to convert.
 * @return A string representation of the token type.
 */
const char			*token_type_str(t_token_type type);

char				*ft_getenv(char *name, char **env);
int					expand_variable(char **ret, char **line,
						t_context *context);

int					expand_special(char **ret, char **line, t_context *context);
int					expand_status(char **ret, char **str, t_context *context);
int					expand_pos_param(char **ret, char **str,
						t_context *context);

t_token				*token_new(t_token_type type, const char *val, int pos);

t_token				*lex_and(const char *input, int *i);
t_token				*lex_redir_out(const char *input, int *i);
t_token				*lex_redir_in(const char *input, int *i);
t_token				*lex_pipe_or(const char *input, int *i);

int					is_metachar(char c);
int					validate_sequence(t_token *prev, t_token *next, int i,
						t_token *head);

void				lexer_error(const char *msg, int pos, char *token);
void				skip_whitespace(const char *input, int *i);
#endif
