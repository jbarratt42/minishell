/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jbarratt <jbarratt@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/05 13:16:48 by jbarratt          #+#    #+#             */
/*   Updated: 2025/09/08 15:08:40 by jbarratt         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	**copy_env(char **env)
{
	char	**p;
	size_t	len;
	size_t	i;

	p = env;
	len = 0;
	while (*p++)
		len++;
	p = malloc((len + 1) * sizeof(char *));
	if (!p)
		return (NULL);
	i = 0;
	while (i < len)
	{
		p[i] = ft_strdup(env[i]);
		i++;
	}
	p[i] = NULL;
	return (p);
}

void	free_env(char **env)
{
	char	**p;

	p = env;
	while (*p++)
		free(p);
	free(env);
}

/* @param var malloc'd string of the form VAR=value
 */
char	**push_env(char *var, char **env)
{
	char	**p;
	size_t	len;
	size_t	i;

	len = 0;
	p = env;
	while (*p++)
		len++;
	p = malloc((len + 2) * sizeof(char *));
	if (!p)
		return (NULL);
	i = 0;
	while(i < len)
	{
		p[i] = env[i];
		i++;
	}
	free(env);
	p[len] = var;
	p[len + 1] = NULL;
	return (p);
}

/* @param var malloc'd string of the form VAR=value
 */
char	**set_env(char *var, char **env)
{
	const char		*pos = ft_strchr(var, '=');
	char			**p;

	if (!pos)
	{
		perror("set_env");
		return (NULL);
	}
	p = env;
	while (*p++)
		if(!ft_strncmp(var, *p, pos - var))
		{
			free(*p);
			*p = var;
			continue;
		}
	if (!*p)
		return (push_env(var, env));
	return (env);
}
