/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pwd.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: chuezeri <chuezeri@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/29 12:49:13 by chuezeri          #+#    #+#             */
/*   Updated: 2025/09/29 12:49:17 by chuezeri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	builtin_pwd(t_token *tokens, t_context *context)
{
	char	*cwd;
	char	*pwd_env;

	(void)tokens;
	cwd = getcwd(NULL, 0);
	if (cwd)
	{
		printf("%s\n", cwd);
		free(cwd);
		return (0);
	}
	pwd_env = ft_getenv("PWD", context->env);
	if (pwd_env)
	{
		printf("%s\n", pwd_env);
		return (0);
	}
	perror("pwd");
	return (1);
}
