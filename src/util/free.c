/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: chuezeri <chuezeri@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/14 11:49:07 by jbarratt          #+#    #+#             */
/*   Updated: 2025/11/12 13:30:44 by jbarratt         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	free_tokens(t_token *token)
{
	t_token	*next;

	if (!token)
		return ;
	/* Save next before freeing current token, in case token->next
	 * gets corrupted during free or token is part of a larger list
	 * that's being freed elsewhere */
	next = token->next;
	/* Free value if it exists */
	if (token->value)
	{
		free(token->value);
		token->value = NULL;
	}
	/* Clear next pointer before freeing to prevent accidental traversal
	 * of freed memory */
	token->next = NULL;
	token->prev = NULL;
	free(token);
	/* Recursively free the rest of the list */
	if (next)
		free_tokens(next);
}

void	free_node(t_node *node)
{
	if (!node)
		return ;
	if (node->is_terminal)
		free_tokens(node->data.tokens);
	else
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
	free_node(context->tree);
	context->tree = NULL;
}

void	cleanup_and_exit(t_context *context)
{
	free_context(context);
	exit(context->status);
}
