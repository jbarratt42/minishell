/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: chuezeri <chuezeri@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/29 12:48:31 by chuezeri          #+#    #+#             */
/*   Updated: 2025/09/29 12:48:37 by chuezeri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	builtin_env(t_token *tokens, t_context *context)
{
	char	**env;
	int		i;

	(void)tokens;
	env = context->env;
	if (!env)
		return (0);
	i = 0;
	while (env[i])
	{
		if (ft_strchr(env[i], '='))
		ft_printf("%s\n", env[i]);
		i++;
	}
	return (0);
}
