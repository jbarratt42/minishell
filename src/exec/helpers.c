/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   helpers.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: chuezeri <chuezeri@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/29 16:13:34 by chuezeri          #+#    #+#             */
/*   Updated: 2025/10/15 15:26:34 by jbarratt         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/* expand variables in tokens */
bool	expand_tokens(t_token **token, t_context *context)
{
	while (*token && (*token)->type < PIPE && (*token)->type != EOF_T)
	{
		if ((*token)->type == WORD)
		{
			(*token)->value = expand((*token)->value, context);
			if (!*token)
				return (false);
			if (ft_strlen((*token)->value) == 0)
			{
				delete_tokens(token, 1);
				continue ;
			}
			token = separate_words(*token);
		}
		else
			token = &((*token)->next);
	}
	return (true);
}

/* open the file in token->next->value, setting the file desciptor in
 * context open, and closing fds as required */
bool	reassign_fd(t_token *token, t_context *context)
{
	const int	fd = token->type >= REDIR_OUT;
	int			mode;

	if (token->type == REDIR_IN)
		mode = O_RDONLY;
	else if (token->type == REDIR_OUT)
		mode = O_WRONLY | O_CREAT;
	else if (token->type == REDIR_APPEND)
		mode = O_WRONLY | O_APPEND | O_CREAT;
	else
		return (heredoc(token, context));
	if (context->open[fd] > 2 && close(context->open[fd]) == -1)
		return (perror("reassign_fd"), false);
	context->open[fd] = open(token->next->value, mode,
			S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH);
	if (context->open[fd] == -1)
		return (perror("reassign_fd"), (false));
	return (true);
}
