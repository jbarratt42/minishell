/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jbarratt <jbarratt@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/03 14:01:49 by jbarratt          #+#    #+#             */
/*   Updated: 2025/10/10 12:29:45 by jbarratt         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	cleanup_heredoc(int fd, char *tmp_file)
{
	close(fd);
	(void)tmp_file;
	free(tmp_file);
}

static bool	read_heredoc(char *tmp_file, char *delimiter, t_context *context)
{
	int		fd;
	char	*line;

	context->is_heredoc = true;
	fd = open(tmp_file, O_WRONLY | O_CREAT | O_TRUNC, 0644);
	if (fd == -1)
		return (free(tmp_file), (false));
    while (true)
	{
		line = readline("> ");
		if (!line)
			return (cleanup_heredoc(fd, tmp_file), false);
		if (ft_strcmp(line, delimiter) == 0)
		{
			free(line);
			break ;
		}
        line = expand(line, context);
        if (!line)
            return (cleanup_heredoc(fd, tmp_file), false);
        write(fd, line, ft_strlen(line));
        write(fd, "\n", 1);
        free(line);
	}
	close(fd);
	context->is_heredoc = false;
	return (true);
}

/* process a heredoc */
bool	heredoc(t_token *token, t_context *context)
{
	char	*delimiter;
	char	*tmp_file;

	if (!token || !token->next || token->next->type != WORD)
		return (false);
	delimiter = token->next->value;
	tmp_file = ft_strjoin(".minishell/heredoc_", delimiter);
	if (!tmp_file)
		return (false);
    read_heredoc(tmp_file, delimiter, context);
	if (context->open[0] > 2)
		close(context->open[0]);
	context->open[0] = open(tmp_file, O_RDONLY);
	if (context->open[0] == -1)
		return (free(tmp_file), false);
	free(tmp_file);
	return (true);
}
