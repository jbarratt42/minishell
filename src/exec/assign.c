/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   assign.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: chuezeri <chuezeri@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/29 16:03:37 by chuezeri          #+#    #+#             */
/*   Updated: 2025/11/18 12:58:31 by chuezeri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	delete_tokens(t_token **token, t_token **head, size_t len)
{
	t_token	*tmp;
	bool	is_head;
	size_t	i;

	is_head = (*token == *head);
	i = 0;
	while (i < len && *token)
	{
		free((*token)->value);
		tmp = *token;
		*token = (*token)->next;
		free(tmp);
		i++;
	}
	if (is_head)
		*head = *token;
}
/* check if a token list contains a command */
bool	is_command(t_token *token)
{
	if (token->type >= PIPE)
		return (false);
	while (token && token->type < PIPE && token->type != EOF_T)
	{
		if (token->type >= REDIR_IN && token->type <= REDIR_APPEND)
			token = token->next->next;
		else if (token->type == WORD && token->value && token->value[0] == '\0')
			token = token->next;
		else if (token->type == WORD && ft_strchr(token->value, '=')
			&& is_valid_identifier(token->value))
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
		if ((*token)->type == WORD && ft_strchr((*token)->value, '=')
			&& is_valid_identifier((*token)->value))
		{
			context->local = set_env(ft_strdup((*token)->value),
					context->local);
			if (!context->local)
				return (false);
			delete_tokens(token, &context->tokens, 1);
		}
		else
			token = &((*token)->next->next);
	}
	return (true);
}

static bool	check_for_heredocs(t_token *scan, t_context *context)
{
	while (scan && scan->type < PIPE && scan->type != EOF_T)
	{
		if (scan->type == HEREDOC)
		{
			if (!heredoc(scan, context))
			{
				context->status = 1;
				return (false);
			}
		}
		if (scan->next)
			scan = scan->next;
		else
			break ;
	}
	return (true);
}

bool	scan_non_heredoc(t_context *context, t_token **token)
{
	t_token	*scan;
	int		fd_index;
	int		mode;
	int		ofd;
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
				fd_index = (scan->type >= REDIR_OUT);
				if (scan->type == REDIR_IN)
					mode = O_RDONLY;
				else if (scan->type == REDIR_OUT)
					mode = O_WRONLY | O_CREAT | O_TRUNC;
				ofd = open(scan->next->value, mode,
						S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH);
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
			}
		}
		if (!scan->next)
			break ;
		scan = scan->next;
	}
	return (true);
};

/* redirect file descriptors left to right and delete the corresponding tokens*/
/*
 * First pass: collect ALL heredocs before trying to open files so that
 * heredoc input is read even if a later open() would fail (matches bash)
 * Second pass: perform normal reassigns; heredocs were already handled
 * so when encountering a HEREDOC token just remove the tokens.
 */
bool	redirect(t_token **token, t_context *context)
{

	if (!check_for_heredocs(*token, context))
		return (false);
	if (!scan_non_heredoc(context, token))
		return (false);
	while (*token && (*token)->type < PIPE && (*token)->type != EOF_T)
	{
		if ((*token)->type >= REDIR_IN && (*token)->type <= REDIR_APPEND)
		{
			if ((*token)->type == HEREDOC)
			{
				delete_tokens(token, &context->tokens, 2);
				continue ;
			}
			if (!reassign_fd(*token, context))
			{
				context->status = 1;
				return (false);
			}
			delete_tokens(token, &context->tokens, 2);
		}
		else
			token = &((*token)->next);
	}
	return (true);
}
