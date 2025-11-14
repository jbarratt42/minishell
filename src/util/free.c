/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: chuezeri <chuezeri@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/14 11:49:07 by jbarratt          #+#    #+#             */
/*   Updated: 2025/11/14 08:37:42 by jbarratt         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	free_tokens(t_token *token)
{
	if (!token)
		return ;
	if (token->next)
		free_tokens(token->next);
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

void	free_env(char **env)
{
	char	**p;

	if (!env)
		return ;
	p = env;
	while (*p)
		free(*p++);
	free(env);
}

void	free_context(t_context *context)
{
	free_env(context->env);
	context->env = NULL;
	free_env(context->local);
	context->local = NULL;
	free(context->input);
	context->input = NULL;
	free_tokens(context->tokens);
	context->tokens = NULL;
	free_node(context->tree);
	context->tree = NULL;
}

void	cleanup_and_exit(t_context *context)
{
	free_context(context);
	exit(context->status);
}
