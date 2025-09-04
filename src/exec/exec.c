/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jbarratt <jbarratt@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/03 11:31:14 by jbarratt          #+#    #+#             */
/*   Updated: 2025/09/04 11:07:53 by jbarratt         ###   ########.fr       */
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

static int	collect(int pids[2])
{
	int	wstatus[2];
	int	i;

	i = 1;
	while (i >= 0)
	{
		wstatus[i] = 0;
		if ((pids[i] && waitpid(pids[i], &wstatus[i], 0) == -1)
				|| !WIFEXITED(wstatus[i]))
		{
			perror("collect");
			return (-1);
		}
		i--;
	}
	return(WEXITSTATUS(wstatus[1]));
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

char	*get_path(t_token *tokens)
{
	return (tokens->value);
}

bool	is_builtin(t_token *token)
{
	const char builtins[][10] = {"echo", "cd", "pwd", "export", "unset",
								 "env", "exit", ""};
	int i;

	i = 0;
	while (*builtins[i])
		if (!ft_strncmp(token->value, builtins[i++], 10))
			return (true);
	return (false);
}

void	exec_builtin(t_token *token, t_context *context)
{
	(void)token;
	(void)context;
	// builtin if-else ladder
	exit(0);
}

pid_t	exec_terminal(t_token *tokens, t_context *context)
{
	pid_t	pid;

	pid = fork();
	if (pid)
		return (pid);
	try_dup2(context->open);
	if (is_builtin(tokens))
		exec_builtin(tokens, context);
	execve(get_path(tokens), get_args(tokens), context->env);
	perror("exec_terminal");
	exit(1);
}

void	exec_sequential(t_node *node, t_context *context)
{	
	(void)node;
	(void)context;
	/*
	int		w_status;
	pid_t	pid;

	pid = traverse(node->data.op.left);
	if (pid)
	{
		if (waitpid(pid, w_status, 0) == -1 || !WIFEXITED(w_status))
		{
			perror("exec_sequential");
			return (-1);
		}
		w_status = WEXITSTATUS(w_status);
		if ((node->data.op.type == AND && ret != 0)
					|| (node->data.op.type == OR && ret == 0))
				context->status = w_status;
				return
	else
		pid = tran
		*/
	return ;
}
		
pid_t	traverse(t_node *node, t_context *context)
{
	pid_t	pids[2];
	if (node->is_terminal)
		return (exec_terminal(node->data.tokens, context));
	if (node->data.op.type != PIPE)
	{
		exec_sequential(node, context);
		return (0);
	}
	if (!try_pipe(&context->open[1]))
		return (-1);
	pids[0] = traverse(node->data.op.left, context);
	if (!try_close2(context->open))
		return (-1);
	context->open[0] = context->open[2];
	context->open[1] = 1;
	context->open[2] = -1;
	pids[1] = traverse(node->data.op.right, context);
	context->status = collect(pids);
	return (0);
}
