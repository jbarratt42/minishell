/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: chuezeri <chuezeri@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/17 11:32:13 by chuezeri          #+#    #+#             */
/*   Updated: 2025/10/17 11:33:26 by chuezeri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	cleanup_heredoc(int fd)
{
	if (fd >= 0)
		close(fd);
}

bool	has_quotes(const char *s)
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

int	is_delim_line(const char *line, const char *cmp_delim)
{
	size_t	len_line;
	size_t	len_cmp;
	size_t	cmp_len;

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

char	*maybe_expand_line(char *line, bool do_expand, t_context *context)
{
	char	*expanded;

	if (!do_expand)
		return (line);
	expanded = expand(line, context);
	return (expanded);
}
