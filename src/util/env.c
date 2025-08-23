/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: chuezeri <chuezeri@student.42.de>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/20 12:06:22 by jbarratt          #+#    #+#             */
/*   Updated: 2025/08/23 21:41:21 by chuezeri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static char **cleanup(char **env)
{
	size_t i;

	i = 0;
	while (env[i])
		free(env[i]);
	free(env);
	return (NULL);
}

char **copy_env(char **env)
{
	size_t i;
	size_t len;
	char **new;

	len = 0;
	while (*env[len])
		len++;
	new = malloc(len * sizeof(char *));
	if (!new)
		return (NULL);
	i = 0;
	while (i < len)
	{
		new[i] = ft_strdup(env[i]);
		if (!new[i])
			return (cleanup(new));
		i++;
	}
	new[i] = NULL;
	return (new);
}

char **push_env(char **env, char *str)
{
	char **new;
	size_t len;
	size_t i;

	len = 0;
	while (env[len])
		len++;
	new = malloc((len + 1) * sizeof(char *));
	if (!new)
		return (cleanup(env));
	i = 0;
	while (i < len)
		new[i] = env[i];
	new[len] = ft_strdup(str);
	if (!new[len])
	{
		free(new);
		return (cleanup(env));
	}
	free(env);
	return (new);
}
