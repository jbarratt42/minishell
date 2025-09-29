/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   assign.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: chuezeri <chuezeri@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/29 16:03:37 by chuezeri          #+#    #+#             */
/*   Updated: 2025/09/29 16:16:38 by chuezeri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/* delete len tokens starting with the token pointed to by token */
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

/* check if a token list contains a command */
bool	is_command(t_token *token)
{
	while (token && token->type < PIPE && token->type != EOF_T)
		if (token->type >= REDIR_IN && token->type <= REDIR_APPEND)
			token = token->next->next;
		else if (token->type == WORD && token->value && token->value[0] == '\0')
			token = token->next;
		else if (token->type == WORD && ft_strchr(token->value, '='))
			token = token->next;
		else
			return (true);
	return (false);
}

/* implement and delete assignment tokens from left to right */
bool	assign(t_token **token, t_context *context)
{
	while (*token && (*token)->type != EOF_T)
		if ((*token)->type == WORD && ft_strchr((*token)->value, '='))
		{
			context->local = set_env((*token)->value, context->local);
			if (!context->local)
				return (false);
			delete_tokens(token, 1);
		}
		else
			token = &((*token)->next->next);
	return (true);
}

/* process a heredoc */
bool	heredoc(t_token *token, t_context *context)
{
	char	*delimiter;
	char	*line;
	char	*tmp_file;
	int		fd;

	if (!token || !token->next || token->next->type != WORD)
		return (false);
	delimiter = token->next->value;
	tmp_file = ft_strjoin(".minishell/heredoc_", delimiter);
	if (!tmp_file)
		return (false);
	fd = open(tmp_file, O_WRONLY | O_CREAT | O_TRUNC, 0644);
	if (fd == -1)
		return (free(tmp_file), (false));
	while (true)
	{
		line = readline("> ");
		if (!line)
		{
			close(fd);
			free(tmp_file);
			return (false);
		}
		if (ft_strcmp(line, delimiter) == 0)
		{
			free(line);
			break ;
		}
		write(fd, line, ft_strlen(line));
		write(fd, "\n", 1);
		free(line);
	}
	close(fd);
	if (context->open[0] > 2)
		close(context->open[0]);
	context->open[0] = open(tmp_file, O_RDONLY);
	if (context->open[0] == -1)
		return (free(tmp_file), false);
	free(tmp_file);
	return (true);
}

/* redirect file descriptors left to right and delete the corresponding tokens*/
bool	redirect(t_token **token, t_context *context)
{
	while (*token && (*token)->type < PIPE && (*token)->type != EOF_T)
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
	return (true);
}
