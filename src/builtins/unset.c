/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   unset.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: chuezeri <chuezeri@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/29 12:49:21 by chuezeri          #+#    #+#             */
/*   Updated: 2025/09/29 16:25:21 by chuezeri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	unset_from_env(char **env, const char *name)
{
	char	**p;
	char	*equal_pos;
	int		name_len;

	if (!env || !name)
		return ;
	name_len = ft_strlen(name);
	p = env;
	while (*p)
	{
		equal_pos = ft_strchr(*p, '=');
		if (equal_pos && (equal_pos - *p) == name_len)
		{
			if (ft_strcmp(*p, name) == 0)
				return (free(*p));
		}
		p++;
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
			err_printf("unset: `%s': not a valid identifier\n",
				current->value);
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
