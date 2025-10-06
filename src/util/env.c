/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: chuezeri <chuezeri@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/05 13:16:48 by jbarratt          #+#    #+#             */
/*   Updated: 2025/10/06 11:38:01 by jbarratt         ###   ########.fr       */
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
	while (*p)
	{
		len++;
		p++;
	}
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

/* @param var malloc'd string of the form VAR=value
 * @param env must be initialized with init_env or copy_env!
 */
char	**push_env(char *var, char **env)
{
	char	**p;
	size_t	len;
	size_t	i;

	len = 0;
	p = env;
	while (*p)
	{
		len++;
		p++;
	}
	p = malloc((len + 2) * sizeof(char *));
	if (!p)
		return (free(env), NULL);
	i = 0;
	while (i < len)
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
	char	*pos;
	char	**p;

	if (!env)
		return (push_env(var, env));
	pos = ft_strchr(var, '=');
	if (!pos)
		pos = var + ft_strlen(var);
	p = env;
	while (*p)
	{
		if (!ft_strncmp(var, *p, pos - var) && *pos == '=')
		{
			free(*p);
			*p = var;
			return (env);
		}
		p++;
	}
	return (push_env(var, env));
}

char	*get_var(char *name, char **env)
{
	const size_t	len = ft_strlen(name);

	if (!env)
		return (NULL);
	while (*env)
	{
		if (!ft_strncmp(name, *env, len) && (*env)[len] == '=')
			return (*env);
		env++;
	}
	return (NULL);
}

char	*ft_getenv(char *name, char **env)
{
	char	*var;

	var = get_var(name, env);
	if (!var)
		return (NULL);
	var = ft_strchr(var, '=');
	if (var == NULL)
		return (NULL);
	return (var + 1);
}
