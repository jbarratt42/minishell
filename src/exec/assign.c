/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   assign.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: chuezeri <chuezeri@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/29 16:03:37 by chuezeri          #+#    #+#             */
/*   Updated: 2025/10/06 12:40:17 by jbarratt         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/* delete len tokens starting with the token pointed to by token */
/*
void	delete_tokens(t_token **token, size_t len)
{
	t_token	*tmp;
	t_token	*end;

	tmp = *token;
	if (!len)
		return ;
	end = *token;
	while (len--)
		end = end->next;
	*token = end;
	end = NULL;
	free(tmp);
}
*/
void	delete_tokens(t_token **token, size_t len)
{
	t_token	*tmp;

	if (!len)
		return ;
	free((*token)->value);
	tmp = *token;
	*token = (*token)->next;
	delete_tokens(token, len - 1);
	free(tmp);
}

/* check if a token list contains a command */
bool	is_command(t_token *token)
{
	while (token && token->type < PIPE && token->type != EOF_T)
	{
		if (token->type >= REDIR_IN && token->type <= REDIR_APPEND)
			token = token->next->next;
		else if (token->type == WORD && token->value && token->value[0] == '\0')
			token = token->next;
		else if (token->type == WORD && ft_strchr(token->value, '='))
			token = token->next;
		else
			return (true);
	}
	return (false);
}

/* implement and delete assignment tokens from left to right */
bool	assign(t_token **token, t_context *context)
{
	while (*token && (*token)->type != EOF_T)
	{
		if ((*token)->type == WORD && ft_strchr((*token)->value, '='))
		{
			context->local = set_env(ft_strdup((*token)->value),
					context->local);
			if (!context->local)
				return (false);
			delete_tokens(token, 1);
		}
		else
			token = &((*token)->next->next);
	}
	return (true);
}

/* redirect file descriptors left to right and delete the corresponding tokens*/
bool	redirect(t_token **token, t_context *context)
{
	while (*token && (*token)->type < PIPE && (*token)->type != EOF_T)
	{
		if ((*token)->type >= REDIR_IN && (*token)->type <= REDIR_APPEND)
		{
			if (!reassign_fd(*token, context))
			{
				context->status = 1;
				return (false);
			}
			delete_tokens(token, 2);
		}
		else
			token = &((*token)->next);
	}
	return (true);
}
