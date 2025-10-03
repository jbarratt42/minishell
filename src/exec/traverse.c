/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   traverse.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jbarratt <jbarratt@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/03 13:11:36 by jbarratt          #+#    #+#             */
/*   Updated: 2025/10/03 13:21:05 by jbarratt         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	set_status(pid_t pids[2], t_context *context)
{
	int	status;

	if (pids[1] == -1)
		context->status = 1;
	status = collect2(pids);
	if (pids[1] && pids[1] != -1)
		context->status = status;
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
	set_status(pids, context);
	return (0);
}
