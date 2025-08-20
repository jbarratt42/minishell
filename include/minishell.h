/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jbarratt <jbarratt@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/14 10:46:56 by jbarratt          #+#    #+#             */
/*   Updated: 2025/08/20 14:33:56 by jbarratt         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H
# include <unistd.h>
# include <stdlib.h>
# include <sys/wait.h>
# include <stdbool.h>
# include "../libft/libft.h"

typedef enum e_toktype
{
	WORD,
	FILE_IN,
	FILE_OUT,
	FILE_APPEND,
	HEREDOC,
	OPEN,
	CLOSE,
	PIPE,
	AND,
	OR,
	SEMICOLON,
	MAX_TOKTYPE
} t_toktype;

typedef struct s_token
{
	char			*txt;
	enum e_toktype	type;
	struct s_token	*next;
}	t_token;

typedef struct s_node	t_node;

typedef struct s_operator
{
	enum e_toktype	type;
	t_node		*left;
	t_node		*right;
}	t_operator;

union u_data
{
	t_operator	op;
	t_token		*tokens;
};

typedef struct s_node
{
	bool	is_terminal;
	union	u_data	data;
}	t_node;

/* env is a modifiable duplicate of main's env */
typedef struct s_context
{
	int		argc;
	char	**argv;
	char	**env;
	int		status;
	char	*line;
	t_token	*tokens;
	t_node	*tree;
}	t_context;

/* prototypes */
/* src/lex/expand.c */
int		expand_pos_param(char **ret, char **str, int argc, char **argv);
char	*shell_getenv(char *name, char **env);
int		expand_variable(char **ret, char **line, char **env);
int		expand_special(char **ret, char **line, t_context *context);
bool	is_valid_input(char *str);
void	expand_all(t_context *context);
/* src/lex/lex.c */
t_token	*extract_token(char *s, size_t size);
char	*consume_whitespace(char *p);
char	*consume_quote(char *s, t_token *token);
char	*consume_word(char *s, t_token *token);
char	*consume_redirect(char *s, t_token *token);
char	*consume_operator(char *s, t_token *token);
t_token	*lex(t_context *context);
int		print_tokens(t_token *token);
/* src/parse/parse.c */
t_node	*parse(t_token **token, int min_precedence);
int		is_builtin(t_token *token);
int		exec_builtin(t_token *token);
int		is_arg(t_token *token);
int		try_open(char *path, int flags);
void	parse_redirect(t_token *token, int *fd[2]);
t_node	*parse_command(t_token *token, char *path, char **argv, int *fd);
int		exec(t_token *tokens, int fd[3], char **env);
void	try_pipe(int fd[2]);
int		traverse(t_node *node, int fd[3], char **env);
/* src/util/env.c */
char	**copy_env(char **env);
char	**push_env(char **env, char *str);
/* src/util/free.c */
void	free_token(t_token *token);
void	free_node(t_node *node);
void	free_context(t_context *context);
/* src/util/init.c */
bool	init_context(int argc, char **argv, char **env, t_context *context);
/* src/util/print_tree.c */
void	print_indentation(int depth);
void	print_tree_structure(t_node *node, int depth);
/* src/exec/exec.c */
int		collect_process(pid_t pid);
pid_t	exec_terminal(t_token *tokens, t_context *context);
#endif
