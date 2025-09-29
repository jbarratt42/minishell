/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: chuezeri <chuezeri@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/29 13:33:32 by chuezeri          #+#    #+#             */
/*   Updated: 2025/09/29 16:21:54 by chuezeri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static t_token	*next_operator(t_token *token)
{
	while (token && token->type < PIPE)
		token = token->next;
	return (token);
}

static int	precedence(t_token *token)
{
	if (token->type == SEMICOLON)
		return (1);
	if (token->type == AND || token->type == OR)
		return (2);
	if (token->type == PIPE)
		return (3);
	return (0);
}

static t_node	*parse_terminal(t_token *token)
{
	t_node	*node;

	node = malloc(sizeof(t_node));
	if (!node)
		return (NULL);
	node->is_terminal = true;
	node->data.tokens = token;
	return (node);
}

static t_node	*parse_operator(t_token *op, t_node *left, t_node *right)
{
	t_node	*node;

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
t_node	*parse(t_token **token, int min_precedence)
{
	t_token	*op;
	t_node	*left;
	t_node	*right;

	left = parse_terminal(*token);
	if (!left)
		return (NULL);
	*token = next_operator(*token);
	while (*token && (*token)->type >= PIPE
		&& precedence(*token) >= min_precedence)
	{
		op = *token;
		*token = (*token)->next;
		if (op->type == PIPE)
			right = parse(token, precedence(op));
		else
			right = parse(token, precedence(op) + 1);
		if (!right)
			return (free_node(left), (NULL));
		left = parse_operator(op, left, right);
		if (!left)
			return (NULL);
	}
	return (left);
}
