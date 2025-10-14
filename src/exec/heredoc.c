/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: chuezeri <chuezeri@student.42.de>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/03 14:01:49 by jbarratt          #+#    #+#             */
/*   Updated: 2025/10/13 11:39:01 by chuezeri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include <unistd.h>

static void cleanup_heredoc(int fd)
{
	if (fd >= 0)
		close(fd);
}

static bool has_quotes(const char *s)
{
	if (!s)
		return (false);
	while (*s)
	{
		if (*s == '\'' || *s == '"')
			return (true);
		s++;
	}
	return (false);
}

static int is_delim_line(const char *line, const char *cmp_delim)
{
	size_t len_line;
	size_t len_cmp;
	size_t cmp_len;

	len_line = ft_strlen(line);
	len_cmp = ft_strlen(cmp_delim);
	if (len_line > len_cmp)
		cmp_len = len_line;
	else
		cmp_len = len_cmp;
	if (ft_strncmp(line, cmp_delim, cmp_len + 1) == 0)
		return (1);
	return (0);
}

static char *maybe_expand_line(char *line, bool do_expand, t_context *context)
{
	char *expanded;

	if (!do_expand)
		return (line);
	expanded = expand(line, context);
	// free(line);
	return (expanded);
}

static bool write_line_to_fd(int fd, const char *line)
{
	size_t len;

	if (!line)
		return (false);
	len = ft_strlen(line);
	if (write(fd, line, len) < 0)
		return (false);
	if (write(fd, "\n", 1) < 0)
		return (false);
	return (true);
}

static bool heredoc_loop(int fd, char *cmp_delim, bool do_expand, t_context *context)
{
	char *line;

	while (true)
	{
		line = readline("> ");
		if (!line)
		{
			err_printf("minishell: warning: here-document delimited by end-of-file (wanted `%s')\n",
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

static bool read_heredoc(char *tmp_file, char *delimiter, t_context *context)
{
	int fd;
	bool do_expand;
	char *cmp_delim;
	bool ok;

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
bool heredoc(t_token *token, t_context *context)
{
	char *delimiter;
	char *tmp_file;
	char *minishell_dir;
	char *fname;

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
	// if (!ft_strnstr(context->input, "echo", ft_strlen(context->input)))
	// 	context->open[0] = open(tmp_file, O_RDONLY);
	if (context->open[0] == -1)
		return (free(tmp_file), (false));
	unlink(tmp_file);
	free(tmp_file);
	return (true);
}
