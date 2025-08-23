/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: chuezeri <chuezeri@student.42.de>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/14 12:02:08 by jbarratt          #+#    #+#             */
/*   Updated: 2025/08/23 21:42:35 by chuezeri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// static char *try_strdup(char *dst, char *src, char **env)
// {
// 	int i;

// 	dst = ft_strdup(dst);
// 	if (!dst)
// 	{
// 		perror("try_strdup");
// 		i = 0;
// 		while (env[i])
// 			free(env[i]);
// 		free(env);
// 		return (NULL);
// 	}
// 	return (dst);
// }

/*
static char	**envdup(char **env)
{
	size_t	i;
	size_t	len;
	char	**new;

	len = 0;
	while (env[len])
		len++;
	new = malloc(env_len * sizeof(char *));
	if (!new)
	{
		perror("envdup: new");
		return (NULL);
	}
	i = 0;
	while (i < len)
	qq	if(!try_strdup(new[i], env[i], env));
			return (NULL);
	return (new);
}
*/

/* need to status of context->env immediately after */
bool init_context(int argc, char **argv, char **env, t_context *context)
{
	int i;

	context->argc = argc;
	context->argv = argv;
	context->env = copy_env(env);
	if (!context->env)
		return (false);
	context->status = 0;
	context->tokens = NULL;
	context->tree = NULL;
	i = 0;
	while (i < 3)
		context->fds[i++] = -1;
	return (true);
}
