#include "minishell.h"
#include <sys/fcntl.h>

static void free_node(t_node *node)
{
	if (!node->is_terminal)
	{
		free_node(node->data.op.left);
		free_node(node->data.op.right);
	}
	free(node);
}

static t_token *next_operator(t_token *token)
{
	while (token->next && token->type < PIPE)
		token = token->next;
	return (token);
}

static int precedence(t_token *token)
{
	if (token->type == SEMICOLON)
		return (1);
	if (token->type == AND || token->type == OR)
		return (2);
	if (token->type == PIPE)
		return (3);
	return (0);
}

static t_node *parse_terminal(t_token *token)
{
	t_node *node;

	node = malloc(sizeof(t_node));
	if (!node)
		return (NULL);
	node->is_terminal = true;
	node->data.tokens = token;
	return (node);
}

static t_node *parse_operator(t_token *op, t_node *left, t_node *right)
{
	t_node *node;

	node = malloc(sizeof(t_node));
	if (!node)
	{
		free_node(left);
		free_node(right);
	}
	node->is_terminal = false;
	node->data.op.type = op->type;
	node->data.op.left = left;
	node->data.op.right = right;
	return (node);
}

/* take the address of a token list and create a parse tree.
 * - all pipes are treated as right-associative.  this means we only need to
 *   keep 3 open fds at any given time.
 * - all other operators are left-associative
 */
t_node *parse(t_token **token, int min_precedence)
{
	t_token *op;
	t_node *left;
	t_node *right;

	left = parse_terminal(*token);
	if (!left)
		return (NULL);
	*token = next_operator(*token);

	while (*token && (*token)->type >= PIPE && precedence(*token) >= min_precedence)
	{
		op = *token;
		*token = (*token)->next;
		if ((*token)->type == PIPE)
			right = parse(token, precedence(op)); // right-associative for pipes
		else
			right = parse(token, precedence(op) + 1);
		if (!right)
		{
			free_node(left);
			return (NULL);
		}
		left = parse_operator(op, left, right);
		if (!left)
			return (NULL);
	}
	return (left);
}

int is_builtin(t_token *token)
{
	const char builtins[][10] = {"echo", "cd", "pwd", "export", "unset",
								 "env", "exit"};
	int i;

	i = 0;
	while (builtins[i])
		if (!ft_strncmp(token->txt, builtins[i++], 10))
			return (1);
	return (0);
}

int exec_builtin(t_token *token, t_context *context)
{
	(void)token;
	(void)context;
	// builtin if-else ladder
	return (1);
}

int is_arg(t_token *token)
{
	if (*token->txt == '<' || *token->txt == '>')
		return (0);
	return (1);
}

int try_open(char *path, int flags)
{
	int ret;

	ret = open(path, flags);
	if (ret == -1)
		exit(1);
	return ret;
}

void parse_redirect(t_token *token, int fd[2])
{
	const int ind = *token->txt == '>';
	int mode;

	// if <, close fd[0] if necessary, open filename,
	// and set fd[0] to the result.
	// if >, do the same for fd1
	if (fd[ind] > 2)
		close(fd[ind]);
	mode = O_WRONLY;
	if (!ind)
		mode = O_RDONLY;
	if (mode == O_WRONLY && token->txt[1] == '>')
		mode = mode & O_APPEND;
	token = token->next;
	if (!token)
		exit(1);
	fd[ind] = try_open(token->txt, mode);
}

char *expand_vars(t_token *token)
{
	(void)token;
	return NULL;
}

t_node *parse_command(t_token *token, char *path, char **argv, int *fd)
{
	int i;

	path = token->txt;
	token = token->next;
	i = 0;
	(void)path;
	while (token && token->type < PIPE)
	{
		if (is_arg(token))
		{
			argv[i] = expand_vars(token);
			i++;
		}
		else
			parse_redirect(token, fd);
	}
	return NULL;
}

int get_return_code(int wstatus);

/* execute the command or builtin specified by tokens
 * - the child (if any) should close each element of fd that is greater than 2
 * 		(not stdout, stdin, or stderr)
 * - the parent should only close fd[1] and fd[2] after waitpid().
 */
int exec(t_token *tokens, int fd[3], char **env)
{
	char *path = NULL;
	char **argv = NULL;
	int pid;
	int wstatus;

	// exec shoulld have access to context
	if (is_builtin(tokens))
		return (exec_builtin(tokens, NULL));
	parse_command(tokens, path, argv, fd);
	pid = fork();
	if (!pid)
	{
		if (fd[0] > 2)
		{
			dup2(STDIN_FILENO, fd[0]);
			close(fd[0]);
		}
		if (fd[1] > 2)
		{
			dup2(STDOUT_FILENO, fd[1]);
			close(fd[1]);
		}
		if (fd[2] > 2)
			close(fd[2]);
		execve(path, argv, env);
		perror(path);
	}
	waitpid(pid, &wstatus, 0);
	if (fd[0] > 2)
		close(fd[0]);
	if (fd[1] > 2)
		close(fd[1]);
	return (get_return_code(wstatus));
}

int try_pipe(int fd[2])
{
	int ret;

	ret = pipe(fd);
	if (ret == -1)
	{
		perror("pipe");
		exit(1);
	}
	return (ret);
}

/* @brief recursively traverse a node in an AST, left to right
 * @param node the node to traverse
 * @param fd array of three fds
 *  - fd[0] is the fd to read from
 *  - fd[1] is the fd to write to
 *  - fd[2] if > 2, is the other half of fd[1]'s pipe: it should be
 *  	closed in the child process but not the parent
 *  if node is another operator:
 *  	if node is a pipe:
 *  		- create innerfd as a pipe
 *  	otherwise, innerfd = {0, 1};
 *  	for left:
 *  		fd[0] = fd[0];
 *  		fd[1] = innerfd[1];
 *  		fd[2] = innerfd[0];
 *  	for right:
 *  		fd[0] = innerfd[0];
 *  		fd[1] = fd[1];
 *  		fd[2] = -1;
 *	if the node is terminal, pass on the tokens and fds to exec
 */
// int traverse(t_node *node, int fd[3], char **env)
// {
// 	int innerfd[2];
// 	int outerfd[2];
// 	int ret;

// 	if (node->is_terminal)
// 		return (exec_terminal(node));
// 	outerfd[0] = fd[0];
// 	outerfd[1] = fd[1];
// 	if (node->data.op.type == PIPE)
// 		try_pipe(innerfd);
// 	else
// 	{
// 		innerfd[0] = 0;
// 		innerfd[1] = 1;
// 	}
// 	fd[1] = innerfd[1];
// 	fd[2] = innerfd[0]; // to be closed in the child process
// 	ret = traverse(node->data.op.left, fd, env);
// 	fd[1] = outerfd[1];
// 	if (node->data.op.type == AND && !ret)
// 		return (0);
// 	if (node->data.op.type == OR && ret)
// 		return (ret);
// 	fd[0] = innerfd[0];
// 	fd[2] = 0; // nothing to close on the right
// 	ret = traverse(node->data.op.right, fd, env);
// 	fd[0] = outerfd[0];
// 	return (ret);
// }
