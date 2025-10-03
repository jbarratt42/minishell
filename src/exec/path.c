/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   path.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jbarratt <jbarratt@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/03 10:40:39 by jbarratt          #+#    #+#             */
/*   Updated: 2025/10/03 11:30:52 by jbarratt         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*append_path(char *a, char *b)
{
	char			*c;
	const size_t	len = ft_strlen(a) + ft_strlen(b) + 2;

	c = malloc(len);
	if (!c)
		return (NULL);
	ft_strcpy(c, a);
	ft_strlcat(c, "/", len);
	ft_strlcat(c, b, len);
	return (c);
}

static char	*check_path(char **path, char *s)
{
	char	*end;
	char	*try_path;

	end = ft_strchr(*path, ':');
	if (end)
		*end = '\0';
	try_path = append_path(*path, s);
	if (!try_path)
		return (NULL);
	if (try_path && access(try_path, F_OK) == 0)
		if (access(try_path, R_OK) == 0 && access(try_path, X_OK) == 0)
			return (try_path);
	if (end)
		*path = end + 1;
	else
		*path += ft_strlen(*path);
	free (try_path);
	return (NULL);
}

char	*search_path(char *s, char **env)
{
	char	*paths;
	char	*try_path;
	char	*path_copy;

	paths = ft_getenv("PATH", env);
	if (!paths || !*paths)
		return (NULL);
	path_copy = ft_strdup(paths);
	if (!path_copy)
		return (NULL);
	paths = path_copy;
	while (*paths)
	{
		try_path = check_path(&paths, s);
		if (try_path)
			return (try_path);
	}
	free(path_copy);
	return (NULL);
}

char	*get_path(t_token *tokens, char **env)
{
	char	*path;

	path = tokens->value;
	if (!ft_strchr(tokens->value, '/'))
		path = search_path(path, env);
	return (path);
}
