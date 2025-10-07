/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   print_env.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: chuezeri <chuezeri@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/07 12:50:11 by chuezeri          #+#    #+#             */
/*   Updated: 2025/10/07 13:01:29 by chuezeri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static char	**copy_env_with_equals(char **env, int count)
{
	char	**result;
	int		i;
	int		j;

	i = 0;
	j = 0;
	result = malloc((count + 1) * sizeof(char *));
	if (!result)
		return (NULL);
	while (env && env[i])
	{
		if (ft_strchr(env[i], '='))
			result[j++] = env[i];
		i++;
	}
	result[j] = NULL;
	return (result);
}

static int	count_env_with_equals(char **env)
{
	int	count;
	int	i;

	count = 0;
	i = 0;
	while (env && env[i])
	{
		if (ft_strchr(env[i], '='))
			count++;
		i++;
	}
	return (count);
}

static char	**filter_env_with_equals(char **env, int *out_count)
{
	char	**result;
	int		count;

	count = count_env_with_equals(env);
	*out_count = count;
	if (count == 0)
		return (NULL);
	result = copy_env_with_equals(env, count);
	return (result);
}

static void	print_sorted_env_vars(char **env)
{
	int	i;

	i = 0;
	while (env && env[i])
	{
		ft_printf("declare -x %s\n", env[i]);
		i++;
	}
}

void	print_exported_vars(char **env)
{
	char	**filtered_env;
	int		count;

	filtered_env = filter_env_with_equals(env, &count);
	if (!filtered_env || count == 0)
	{
		free(filtered_env);
		return ;
	}
	sort_env_vars(filtered_env, count);
	print_sorted_env_vars(filtered_env);
	free(filtered_env);
}
