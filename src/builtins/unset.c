/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   unset.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: chuezeri <chuezeri@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/29 12:49:21 by chuezeri          #+#    #+#             */
/*   Updated: 2025/10/17 11:24:45 by chuezeri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	unset_from_env(char **env, const char *name)
{
	int		name_len;
	int		i;
	int		j;
	char	*equal_pos;

	if (!env || !name)
		return ;
	name_len = ft_strlen(name);
	i = 0;
	while (env[i])
	{
		equal_pos = ft_strchr(env[i], '=');
		if (equal_pos && (equal_pos - env[i]) == name_len
			&& !ft_strncmp(env[i], name, name_len))
		{
			free(env[i]);
			j = i;
			while (env[j + 1])
			{
				env[j] = env[j + 1];
				j++;
			}
			env[j] = NULL;
			i--;
		}
		i++;
	}
}


int	builtin_unset(t_token *tokens, t_context *context)
{
	t_token	*current;
	int		ret;

	current = tokens;
	ret = 0;
	if (current && current->type == WORD)
		current = current->next;
	if (!current || current->type != WORD)
		return (0);
	while (current && current->type == WORD)
	{
		if (!is_valid_identifier(current->value))
		{
			err_printf("unset: `%s': not a valid identifier\n", current->value);
			ret = 1;
		}
		else
		{
			unset_from_env(context->env, current->value);
			unset_from_env(context->local, current->value);
		}
		current = current->next;
	}
	return (ret);
}
