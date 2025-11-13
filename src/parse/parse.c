/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: chuezeri <chuezeri@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/29 13:33:32 by chuezeri          #+#    #+#             */
/*   Updated: 2025/10/17 15:33:10 by jbarratt         ###   ########.fr       */
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
	if (!is_command(token))
		return (free(node), NULL);
	return (node);
}

/* Helper function to nullify all tokens in a node tree.
 * This prevents free_node() from freeing tokens that are part of the
 * original list when freeing a parse tree from a recursive call. */
static void	nullify_all_tokens(t_node *node)
{
	if (!node)
		return ;
	if (node->is_terminal)
		node->data.tokens = NULL;
	else
	{
		nullify_all_tokens(node->data.op.left);
		nullify_all_tokens(node->data.op.right);
	}
}

/* Helper function to check if a node tree contains the original start token */
static bool	contains_original_start(t_node *node, t_token *original_start)
{
	if (!node)
		return (false);
	if (node->is_terminal)
		return (node->data.tokens == original_start);
	return (contains_original_start(node->data.op.left, original_start)
		|| contains_original_start(node->data.op.right, original_start));
}

static t_node	*parse_operator(t_token *op, t_node *left, t_node *right, t_token ***original_ptr, t_token *original_start)
{
	t_node	*node;
	t_token	*left_start;

	node = malloc(sizeof(t_node));
	if (!node)
	{
		/* If malloc fails, we need to free left and right nodes.
		 * But we need to be careful: if left contains the original tokens,
		 * we should free them and mark as freed. Otherwise, we're in a
		 * recursive call and shouldn't free tokens that are part of the
		 * original list. */
		/* Free left node. If it contains original tokens, mark them as freed.
		 * Otherwise, set tokens to NULL before freeing to prevent freeing
		 * tokens that are part of the original list. */
		if (left && left->is_terminal)
		{
			left_start = left->data.tokens;
			if (*original_ptr && left_start == original_start)
			{
				/* Top-level: free tokens and mark as freed */
				**original_ptr = NULL;
				free_node(left);
			}
			else
			{
				/* Recursive call: don't free tokens */
				left->data.tokens = NULL;
				free_node(left);
			}
		}
		else if (left)
		{
			/* Non-terminal node: it might contain terminal children with
			 * tokens from the original list. Check if it contains the original
			 * start token BEFORE nullifying, then nullify all tokens before
			 * freeing to prevent freeing tokens that context->tokens still
			 * points to. */
			if (*original_ptr && contains_original_start(left, original_start))
				**original_ptr = NULL;
			nullify_all_tokens(left);
			free_node(left);
		}
		/* right is from a recursive call that succeeded, so it's a valid
		 * parse tree. However, it contains tokens from the original list.
		 * Check if it contains the original start token BEFORE nullifying,
		 * then nullify all tokens before freeing to prevent freeing tokens
		 * that context->tokens still points to. */
		if (right)
		{
			if (*original_ptr && contains_original_start(right, original_start))
				**original_ptr = NULL;
			nullify_all_tokens(right);
			free_node(right);
		}
		return (NULL);
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
static t_node	*parse_impl(t_token **token, int min_precedence, t_token ***original_ptr, t_token *original_start)
{
	t_token	*op;
	t_node	*left;
	t_node	*right;
	t_token	*first_token;

	left = parse_terminal(*token);
	if (!left)
		return (NULL);
	/* If this is the first call, save the original token pointer address */
	first_token = left->data.tokens;
	if (!*original_ptr)
		*original_ptr = token;
	*token = next_operator(*token);
	while (*token && (*token)->type >= PIPE
		&& precedence(*token) >= min_precedence)
	{
		op = *token;
		*token = (*token)->next;
		if (op->type == PIPE)
			right = parse_impl(token, precedence(op), original_ptr, original_start);
		else
			right = parse_impl(token, precedence(op) + 1, original_ptr, original_start);
		if (!right)
		{
			/* When a recursive call fails, we must not free tokens because
			 * they're part of the original token list. Only the top-level call
			 * should free tokens when freeing the original start token.
			 * 
			 * Strategy: Always nullify tokens before freeing nodes to prevent
			 * free_node() from freeing them. Only at the top level, if we're
			 * freeing the original start token, do we actually free tokens
			 * and mark context->tokens as NULL. */
			if (*original_ptr && first_token == original_start)
			{
				/* Top-level call: free tokens starting from original_start.
				 * CRITICAL: Set context->tokens = NULL BEFORE freeing to prevent
				 * main.c from trying to free them again, and to prevent accessing
				 * freed memory.
				 * 
				 * If left is a non-terminal node, it contains terminal children
				 * with tokens from the original list. free_tokens() recursively
				 * frees the entire list, so we need to nullify all tokens except
				 * the original start before freeing, then free the original start
				 * token manually. */
				**original_ptr = NULL;
				if (left->is_terminal)
				{
					/* Terminal node: free tokens normally */
					free_node(left);
				}
				else
				{
					/* Non-terminal node: nullify all tokens in the tree first,
					 * then free the original start token manually (which frees
					 * the entire list), then free the node structures. */
					nullify_all_tokens(left);
					free_tokens(original_start);
					/* Free the node structures (tokens are already nullified) */
					free_node(left->data.op.left);
					free_node(left->data.op.right);
					free(left);
				}
			}
			else
			{
				/* Recursive call: nullify tokens to prevent freeing.
				 * Don't mark context->tokens as NULL because we're not
				 * freeing the original start token - it's still valid. */
				if (left->is_terminal)
					left->data.tokens = NULL;
				else
					nullify_all_tokens(left);
				free_node(left);
			}
			return (NULL);
		}
		left = parse_operator(op, left, right, original_ptr, original_start);
		if (!left)
		{
			/* parse_operator() already freed left and right nodes (and their tokens)
			 * and set original_ptr to NULL if needed */
			return (NULL);
		}
	}
	return (left);
}

t_node	*parse(t_token **token, int min_precedence)
{
	t_token	**original_ptr;
	t_token	*original_start;
	t_node	*result;

	original_ptr = NULL;
	original_start = *token;  /* Save original start before parse modifies it */
	result = parse_impl(token, min_precedence, &original_ptr, original_start);
	return (result);
}
