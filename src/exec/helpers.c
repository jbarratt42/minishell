/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   helpers.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: chuezeri <chuezeri@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/29 16:13:34 by chuezeri          #+#    #+#             */
/*   Updated: 2025/11/18 14:29:57 by chuezeri         ###   ########.fr       */
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
				delete_tokens(token, &context->tokens, 1);
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
		return (perror("reassign_fd"), (false));
	context->open[fd] = open(token->next->value, mode,
			S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH);
	if (context->open[fd] == -1)
		return (perror("reassign_fd"), (false));
	return (true);
}

bool	open_redir_and_store(t_token *scan, int tmp_fd[2], t_context *context)
{
	int	fd_index;
	int	mode;
	int	ofd;

	fd_index = (scan->type >= REDIR_OUT);
	if (scan->type == REDIR_IN)
		mode = O_RDONLY;
	else if (scan->type == REDIR_OUT)
		mode = O_WRONLY | O_CREAT | O_TRUNC;
	mode = O_WRONLY | O_CREAT | O_APPEND;
	ofd = open(scan->next->value, mode, S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH);
	if (ofd == -1)
	{
		if (tmp_fd[0] != -1)
			close(tmp_fd[0]);
		if (tmp_fd[1] != -1)
			close(tmp_fd[1]);
		perror("reassign_fd");
		context->status = 1;
		return (false);
	}
	if (tmp_fd[fd_index] != -1)
		close(tmp_fd[fd_index]);
	tmp_fd[fd_index] = ofd;
	return (true);
}

bool	scan_non_heredoc(t_context *context, t_token **token)
{
	t_token	*scan;
	int		tmp_fd[2];

	scan = *token;
	tmp_fd[0] = -1;
	tmp_fd[1] = -1;
	while (scan && scan->type < PIPE && scan->type != EOF_T)
	{
		if (scan->type >= REDIR_IN && scan->type <= REDIR_APPEND)
		{
			if (scan->type != HEREDOC)
			{
				if (!open_redir_and_store(scan, tmp_fd, context))
					return (false);
			}
		}
		if (!scan->next)
			break ;
		scan = scan->next;
	}
	return (true);
}
