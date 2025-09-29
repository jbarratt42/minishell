

/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: chuezeri <chuezeri@student.42.de>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/14 11:49:07 by jbarratt          #+#    #+#             */
/*   Updated: 2025/09/06 13:33:59 by chuezeri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	free_token(t_token *token)
{
	if (token->next)
		free_token(token->next);
	free(token->value);
	free(token);
}

void	free_node(t_node *node)
{
	if (!node)
		return ;
	if (!node->is_terminal)
	{
		free_node(node->data.op.left);
		free_node(node->data.op.right);
	}
	free(node);
}

void	free_context(t_context *context)
{
	if (context->env)
		free_env(context->env);
}

void	cleanup_and_exit(t_context *context)
{
	free_tokens(context->tokens);
	free_node(context->tree);
	free_context(context);
	exit(context->status);
}