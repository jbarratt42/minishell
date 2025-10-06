/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jbarratt <jbarratt@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/03 11:31:14 by jbarratt          #+#    #+#             */
/*   Updated: 2025/10/06 12:07:20 by jbarratt         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

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
	t_token	*t;

	if (!expand_tokens(tokens, context))
		return (false);
	t = *tokens;
	while (t && t->type < PIPE && t->type != EOF_T)
	{
		if (t->type == WORD)
			dequote(t->value);
		t = t->next;
	}
	t->next = NULL;
	if (!is_command(*tokens))
		if (!assign(tokens, context))
			return (false);
	if (!redirect(tokens, context))
		return (false);
	return (true);
}

pid_t	exec_terminal(t_token **tokens, t_context *context)
{
	pid_t	pid;
	char	*path;

	if (!exec_preprocess(tokens, context))
		return (-1);
	if (!is_command(*tokens))
		return (0);
	if (is_builtin(*tokens))
		return (handle_builtins(tokens, context));
	pid = fork();
	if (pid > 0)
	{
		if (!cleanup_parent(context))
			return (-1);
		return (pid);
	}
	if (!set_exp_vars(tokens, context) || !try_dup2(context->open))
		return (-1);
	path = get_path(*tokens, context->env);
	check_path_child(path, tokens);
	execve(path, get_args(*tokens), context->env);
	handle_execve_fail(path);
	return (pid);
}

bool	exec_sequential(t_node *node, t_context *context)
{
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
