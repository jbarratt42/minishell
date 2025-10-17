/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: chuezeri <chuezeri@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/03 14:01:49 by jbarratt          #+#    #+#             */
/*   Updated: 2025/10/17 11:32:25 by chuezeri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include <unistd.h>

static bool	write_line_to_fd(int fd, const char *line)
{
	size_t	len;

	if (!line)
		return (false);
	len = ft_strlen(line);
	if (write(fd, line, len) < 0)
		return (false);
	if (write(fd, "\n", 1) < 0)
		return (false);
	return (true);
}
#define HEREDOC_WARNING_MSG "minishell: warning: here-document delimited"

static bool	heredoc_loop(int fd, char *cmp_delim, bool do_expand,
		t_context *context)
{
	char	*line;

	while (true)
	{
		line = readline("> ");
		if (!line)
		{
			err_printf(HEREDOC_WARNING_MSG " by end-of-file (wanted `%s')\n",
				cmp_delim);
			return (true);
		}
		if (is_delim_line(line, cmp_delim))
			return (free(line), true);
		line = maybe_expand_line(line, do_expand, context);
		if (!line)
			return (false);
		if (!write_line_to_fd(fd, line))
			return (free(line), false);
		free(line);
	}
}

static bool	read_heredoc(char *tmp_file, char *delimiter, t_context *context)
{
	int		fd;
	bool	do_expand;
	char	*cmp_delim;
	bool	ok;

	context->is_heredoc = true;
	fd = open(tmp_file, O_WRONLY | O_CREAT | O_TRUNC, 0644);
	if (fd == -1)
	{
		context->is_heredoc = false;
		return (false);
	}
	do_expand = !has_quotes(delimiter);
	cmp_delim = ft_strdup(delimiter);
	if (!cmp_delim)
	{
		cleanup_heredoc(fd);
		context->is_heredoc = false;
		return (false);
	}
	ok = heredoc_loop(fd, cmp_delim, do_expand, context);
	cleanup_heredoc(fd);
	free(cmp_delim);
	context->is_heredoc = false;
	return (ok);
}

/* process a heredoc */
bool	heredoc(t_token *token, t_context *context)
{
	char	*delimiter;
	char	*tmp_file;
	char	*minishell_dir;
	char	*fname;

	if (!token || !token->next || token->next->type != WORD)
		return (false);
	delimiter = token->next->value;
	minishell_dir = get_minishell_dir();
	if (!minishell_dir)
		return (false);
	fname = ft_strjoin("heredoc_", delimiter);
	if (!fname)
		return (free(minishell_dir), (false));
	tmp_file = append_path(minishell_dir, fname);
	(free(minishell_dir), free(fname));
	if (!tmp_file)
		return (false);
	if (!read_heredoc(tmp_file, delimiter, context))
		return (free(tmp_file), (false));
	if (!ft_strnstr(context->input, "echo", ft_strlen(context->input)))
		context->open[0] = open(tmp_file, O_RDONLY);
	if (context->open[0] == -1)
		return (free(tmp_file), (false));
	unlink(tmp_file);
	free(tmp_file);
	return (true);
}
