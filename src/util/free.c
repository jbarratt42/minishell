/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jbarratt <jbarratt@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/14 11:49:07 by jbarratt          #+#    #+#             */
/*   Updated: 2025/08/20 12:54:51 by jbarratt         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	free_token(t_token *token)
{
	if (token->next)
		free_token(token->next);
	free(token->txt);
	free(token);
}

void	free_node(t_node *node)
{
	if (!node->is_terminal)
	{
		free_node(node->data.left);
		free_node(node->data.right);
	}
	free(node);
}

void	free_context(t_context *context)
{
	if (context->env)
		while (*context->env)
			free(*context->env++);
	if (context->line)
		free(context->line);
	if (context->tokens)
		free_token(context->tokens);
	if (context->tree)
		free_node(context->tree);
}
