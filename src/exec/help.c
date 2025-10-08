/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   help.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jbarratt <jbarratt@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/03 12:05:13 by jbarratt          #+#    #+#             */
/*   Updated: 2025/10/08 12:12:12 by jbarratt         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	**get_args(t_token *tokens)
{
	int		i;
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

bool	set_exp_vars(t_token **tokens, t_context *context)
{
	while (ft_strchr((*tokens)->value, '=')
		&& is_valid_identifier((*tokens)->value))
	{
		if (!set_env((*tokens)->value, context->env))
			return (false);
		delete_tokens(tokens, 1);
	}
	return (true);
}

bool	cleanup_parent(t_context *context)
{
	if (!try_close2(context->open))
		return (false);
	context->open[0] = 0;
	context->open[1] = 1;
	return (true);
}
