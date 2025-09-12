/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jbarratt <jbarratt@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/03 11:31:14 by jbarratt          #+#    #+#             */
/*   Updated: 2025/09/12 12:24:07 by jbarratt         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static bool	try_close2(int open[2])
{
	int	i;

	i = 0;
	while (i < 2)
	{
		if (open[i] > 2)
			if (close(open[i]) == -1)
			{
				perror("try_close");
				return (false);
			}
		i++;
	}
	return (true);
}

static bool	try_pipe(int fds[2])
{
	int	tmp[2];
	if (pipe(tmp) == -1)
	{
		perror("try_pipe");
		return (false);
	}
	fds[0] = tmp[1];
	fds[1] = tmp[0];
	return (true);
}

char	*append_path(char *a, char *b)
{
	char	*c;
	const size_t	len = ft_strlen(a) + ft_strlen(b) + 2;

	c = malloc(len);
	if (!c)
		return (NULL);
	ft_strcpy(c, a);
	ft_strlcat(c, "/", len);
	ft_strlcat(c, b, len);
	return (c);
}

char	*search_path(char *s, char **env)
{
	char	*paths;
	char	*try_path;
	char	*end;

	paths = ft_getenv("PATH", env);
	if (!paths || !*paths)
		return (NULL);
	end = paths;
	while (end)
	{
		end = ft_strchr(paths, ':');
		if (end)
			*end = '\0';
		try_path = append_path(paths, s);
		if (access(try_path, F_OK) == 0)
		{
			if (access(try_path, R_OK) != 0 || access(try_path, X_OK))
			{
				perror("search_path");
				free(try_path);
				return (NULL);
			}
			return (try_path);
		}
		free(try_path);
		paths = end + 1;
	}
	perror("search_path");
	return (NULL);
}

static int	collect(int pid)
{
	int w_status;

	if (waitpid(pid, &w_status, 0) == -1
			|| !WIFEXITED(w_status))
	{
			perror("collect");
			return (-1);
	}
	return (WEXITSTATUS(w_status));
}

static int	collect2(int pids[2])
{
	int	status[2];
	int	i;

	i = 1;
	while (i >= 0)
	{
		status[i] = collect(pids[i]);
		if (status[i] == -1)
			return (-1);
		i--;
	}
	return(status[1]);
}

bool	try_dup2(int open[3])
{
	int	i;

	i = 0;
	while (i < 2)
	{
		if(open[i] != i)
			if(dup2(open[i], i) == -1)
				return (false);
		i++;
	}
	i = 0;
	while (i < 3)
	{
		if (open[i] > 2)
			if (close(open[i]) == -1)
				return (false);
		i++;
	}
	return (true);
}

char	**get_args(t_token *tokens)
{
	int 	i;
	char	**args;
	t_token	*pos;

	i = 0;
	pos = tokens;
	while (pos && pos->type < PIPE && pos->type != EOF_T)
	{
		pos = pos->next;
		i++;
	}
	args = malloc((i + 1) * sizeof(char *));
	if (!args)
		return (NULL);
	i = 0;
	pos = tokens;
	while (pos && pos->type < PIPE && pos->type != EOF_T)
	{
		args[i++] = pos->value;
		pos = pos->next;
	}
	args[i] = NULL;
	return (args);
}

char	*get_path(t_token *tokens, char **env)
{
	char	*path;

	path = tokens->value;
	if (!ft_strchr(tokens->value, '/'))
		path = search_path(path, env);
	return (path);
}

bool	is_builtin(t_token *token)
{
#ifndef DEBUG
	const char builtins[][10] = {"echo", "cd", "pwd", "export", "unset",
								 "env", "exit", ""};
	int i;

	i = 0;
	while (*builtins[i])
		if (!ft_strncmp(token->value, builtins[i++], 10))
			return (true);
#else
	(void)token;
#endif
	return (false);
}

bool	exec_builtin(t_token *tokens, t_context *context)
{
	(void)tokens;
	(void)context;
	// builtin if-else ladder
	return (true);
}

bool	exec_preprocess(t_token **tokens, t_context *context)
{
	if (!expand_tokens(tokens, context))
		return (false);
	if (!is_command(*tokens))
		if (!assign(tokens, context))
			return (false);
	if (!redirect(tokens, context))
		return (false);
	return (true);
}

bool	set_exp_vars(t_token **tokens, t_context *context)
{
	while (ft_strchr((*tokens)->value, '='))
	{
		if(!set_env((*tokens)->value, context->env))
			return (false);
		delete_tokens(tokens, 1);
	}
	return (true);
}

bool	cleanup_parent(t_context *context)
{
	if(!try_close2(context->open))
		return (false);
	context->open[0] = 0;
	context->open[1] = 1;
	return (true);
}

pid_t	exec_terminal(t_token **tokens, t_context *context)
{
	pid_t	pid;

	if (!context->is_pipeline)
	{
		if (!exec_preprocess(tokens, context))
			return (-1);
		if (!is_command(*tokens))
			return (0);
		if (is_builtin(*tokens))
			return (exec_builtin(*tokens, context));
	}
	pid = fork();
	if (pid)
	{
		if (!cleanup_parent(context))
			return (-1);
		return (pid);
	}
	if (context->is_pipeline)
	{
		if (!exec_preprocess(tokens, context))
			return (-1);
		if (!is_command(*tokens))
			return (0);
		if (is_builtin(*tokens))
			return (exec_builtin(*tokens, context));
	}
	if(!set_exp_vars(tokens, context))
		return (-1);
	try_dup2(context->open);
	execve(get_path(*tokens, context->env), get_args(*tokens), 
			context->env);
	perror("exec_terminal");
	exit(1);
}

bool	exec_sequential(t_node *node, t_context *context)
{	
	(void)node;
	(void)context;
	
	pid_t	pid;

	pid = traverse(node->data.op.left, context);
	if (pid)
		context->status = collect(pid);
	if (context->status == -1)
		return (false);
	if ((node->data.op.type == AND && context->status != 0)
			|| (node->data.op.type == OR && context->status == 0))
			return (true);
	pid = traverse(node->data.op.right, context);
	if (pid)
		context->status = collect(pid);
	if (context->status == -1)
		return (false);
	return (true);
}
	
pid_t	traverse(t_node *node, t_context *context)
{
	pid_t	pids[2];
	if (node->is_terminal)
		return (exec_terminal(&node->data.tokens, context));
	if (node->data.op.type != PIPE)
	{
		context->is_pipeline = false;
		exec_sequential(node, context);
		return (0);
	}
	context->is_pipeline = true;
	if (!try_pipe(&context->open[1]))
		return (-1);
	pids[0] = traverse(node->data.op.left, context);
	if (!try_close2(context->open))
		return (-1);
	context->open[0] = context->open[2];
	context->open[1] = 1;
	context->open[2] = -1;
	pids[1] = traverse(node->data.op.right, context);
	context->status = collect2(pids);
	return (0);
}
