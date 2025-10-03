/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jbarratt <jbarratt@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/03 11:31:14 by jbarratt          #+#    #+#             */
/*   Updated: 2025/10/03 11:25:41 by jbarratt         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include <sys/stat.h>
#include <errno.h>

static int	collect(int pid)
{
	int w_status;

	if (waitpid(pid, &w_status, 0) == -1)
	//		|| !WIFEXITED(w_status))
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
		if(pids[i] && pids[i] != -1)
		{
			status[i] = collect(pids[i]);
			if (status[i] == -1)
				return (-1);
		}
		else
			status[i] = 0;
		i--;
	}
	return(status[1]);
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

bool	is_builtin(t_token *token)
{
	if (!token || token->type != WORD)
		return (false);
	return (is_builtin_command(token->value));
}

bool	exec_builtin(t_token *tokens, t_context *context)
{
	int	status;

	if (!tokens || tokens->type != WORD)
		return (false);
	
	status = execute_builtin(tokens->value, tokens, context);
	context->status = status;
	return (status == 0);
}

bool	exec_preprocess(t_token **tokens, t_context *context)
{
	t_token *t;
	if (!expand_tokens(tokens, context))
		return (false);
	t = *tokens;
	while (t && t->type < PIPE && t->type != EOF_T)
	{
		if (t->type == WORD)
			dequote(t->value);
		t = t->next;
	}
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
	struct stat st;

	if (!exec_preprocess(tokens, context))
		return (-1);
	if (!is_command(*tokens))
		return (0);
	
	// Handle builtins
	if (is_builtin(*tokens))
	{
		if (!context->is_pipeline)
		{
			// Builtins that affect parent state run in parent when not in pipeline
			try_dup2(context->open);
			exec_builtin(*tokens, context);
			return (0);
		}
		else
		{
			// Builtins in pipeline run in child
			pid = fork();
			if (pid == 0)
			{
				try_dup2(context->open);
				exec_builtin(*tokens, context);
				exit(context->status);
			}
			if (pid > 0)
			{
				if (!cleanup_parent(context))
					return (-1);
				return (pid);
			}
			return (-1);
		}
	}
	
	// Handle external commands
	/*
	if (access(get_path(*tokens, context->env), X_OK) == -1)
	{
		perror("exec_terminal");
		return (-1);
	}
	*/
	pid = fork();
	if (pid > 0)
	{
		if (!cleanup_parent(context))
			return (-1);
		return (pid);
	}
	if(!set_exp_vars(tokens, context))
		return (-1);
	try_dup2(context->open);
	
	char *path = get_path(*tokens, context->env);
	if (!path)
	{
		err_printf("%s: command not found\n", (*tokens)->value);
		exit(127);
	}
	
	// Check if the path exists and is executable
	if (access(path, F_OK) == -1)
	{
		err_printf("%s: No such file or directory\n", path);
		exit(127);
	}
	if (access(path, X_OK) == -1)
	{
		
		if (stat(path, &st) == 0 && S_ISDIR(st.st_mode))
		{
			err_printf("%s: Is a directory\n", path);
			exit(126);
		}
		else
		{
			err_printf("%s: Permission denied\n", path);
			exit(126);
		}
	}
	
	execve(path, get_args(*tokens), context->env);
	// If we get here, execve failed
	if (errno == ENOENT)
	{
		err_printf("%s: No such file or directory\n", path);
		exit(127);
	}
	else if (errno == EACCES)
	{
		
		if (stat(path, &st) == 0 && S_ISDIR(st.st_mode))
		{
			err_printf("%s: Is a directory\n", path);
			exit(126);
		}
		else
		{
			err_printf("%s: Permission denied\n", path);
			exit(126);
		}
	}
	else
	{
		perror("exec_terminal");
		exit(1);
	}
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
	int		status;

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
	/*
	if (pids[0] == -1)
		return (-1);
		*/
	if (!try_close2(context->open))
		return (-1);
	context->open[0] = context->open[2];
	context->open[1] = 1;
	context->open[2] = -1;
	pids[1] = traverse(node->data.op.right, context);
	if (pids[1] == -1)
		context->status = 1;
	/*
	if (pids[1] == -1)
		return (-1);
		*/
	status = collect2(pids);
	if (pids[1] && pids[1] != -1)
		context->status = status;
	return (0);
}
