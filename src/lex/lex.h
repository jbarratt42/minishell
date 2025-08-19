#ifndef LEX_H
# define LEX_H
# include <stdio.h>
# include <stdlib.h>
# include <string.h>

typedef struct s_context
{
	int	argc;
	char	**argv;
	char	**env;
	char	**env_copy;
	char	*line;
	t_token	*tokens;
	t_node	*tree;
	int	last_exit_status;
}	t_context;

enum e_toktype
{
	WORD = 1,
	HEREDOC,
	REDIR_IN,
	REDIR_APPEND,
	REDIR_OUT,
	OPEN,
	CLOSE,
	PIPE,
	AND,
	OR,
	SEMICOLON,
	MAX_TYPE
};

typedef struct s_token t_token;

typedef struct s_token
{
	char	 		*txt;
	t_token			*next;
	enum e_toktype	type;
	char			delim;
}	t_token;

/* prototypes */
/* lex.c */
enum	e_toktype token_type(t_token *token);
t_token	*extract_token(char *s, size_t size);
void	free_token(t_token *token);
char	*consume_whitespace(char *p);
t_token	*lex(char *s, char delim);
int		print_tokens(t_token *token);
#endif
