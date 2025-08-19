/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jbarratt <jbarratt@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/14 12:02:08 by jbarratt          #+#    #+#             */
/*   Updated: 2025/08/14 12:59:33 by jbarratt         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

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
	{
		new[i] = ft_strdup(env[i]);
		if (!new[i])
		{
			perror(ft_printf("init_context: new[%d]", i));
			free(new);
			return (NULL);
		}
	}
	return (new);
}

void	init_context(int argc, char **argv, char **env, t_context *context)
{
	context->argc = argc;
	context->argv = argv;
	context->env = envdup(env);
	if (!context->env)
		exit(1);
	context->status = 0;
	context->tokens = NULL;
	context->tree = NULL;
}
