#ifndef PARSE_H
# define PARSE_H
# include <stdlib.h>
# include <string.h>
# include <fcntl.h>
# include <unistd.h>
# include <sys/wait.h>
# include "../lexer/lex.h"

struct s_node;

typedef struct s_operator
{
	enum e_toktype	type;
	struct s_node	*left;
	struct s_node	*right;
}	t_operator;

enum e_nodetype
{
	OPERATOR,
	TERMINAL,
	MAX_NODETYPE
};

union u_node
{
	t_operator	op;
	t_token		*tokens;
};

typedef struct s_node 
{
	enum e_nodetype	type;
	union u_node	content;
}	t_node;

/* prototypes */
/* parse.c */
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
#endif
