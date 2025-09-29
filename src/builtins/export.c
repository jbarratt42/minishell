/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: chuezeri <chuezeri@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/29 12:48:42 by chuezeri          #+#    #+#             */
/*   Updated: 2025/09/29 15:38:53 by chuezeri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static bool	is_valid_identifier(const char *str)
{
	int	i;

	if (!str || !*str)
		return (false);
	if (!ft_isalpha(*str) && *str != '_')
		return (false);
	i = 1;
	while (str[i] && str[i] != '=')
	{
		if (!ft_isalnum(str[i]) && str[i] != '_')
			return (false);
		i++;
	}
	return (true);
}

static void	print_exported_vars(char **env)
{
	char	**sorted_env;
	int		count;
	int		i;
	int		j;
	char	*temp;

	count = 0;
	i = 0;
	while (env && env[i])
	{
		if (ft_strchr(env[i], '='))
			count++;
		i++;
	}
	if (count == 0)
		return ;
	sorted_env = malloc((count + 1) * sizeof(char *));
	if (!sorted_env)
		return ;
	j = 0;
	i = 0;
	while (env && env[i])
	{
		if (ft_strchr(env[i], '='))
			sorted_env[j++] = env[i];
		i++;
	}
	sorted_env[j] = NULL;
	i = 0;
	while (i < count - 1)
	{
		j = 0;
		while (j < count - i - 1)
		{
			if (ft_strncmp(sorted_env[j], sorted_env[j + 1],
					ft_strlen(sorted_env[j]) + 1) > 0)
			{
				temp = sorted_env[j];
				sorted_env[j] = sorted_env[j + 1];
				sorted_env[j + 1] = temp;
			}
			j++;
		}
		i++;
	}
	i = 0;
	while (sorted_env[i])
	{
		printf("declare -x %s\n", sorted_env[i]);
		i++;
	}
	free(sorted_env);
}

int	builtin_export(t_token *tokens, t_context *context)
{
	t_token	*current;
	char	*var;
	char	*pos;
	int		ret;
	char	*joined;
	t_token	*to_free;
	char	*local_var;

	current = tokens;
	ret = 0;
	// Skip the command name
	if (current && current->type == WORD)
		current = current->next;
	// If no arguments, print all exported variables
	if (!current || current->type != WORD)
	{
		print_exported_vars(context->env);
		return (0);
	}
	// Process each argument
	while (current && current->type == WORD)
	{
		var = current->value;
		// Merge any following WORD tokens if current contains '=' or ends with '='
		if (var && ft_strchr(var, '='))
		{
			while (current->next && current->next->type == WORD
				&& current->next->value)
			{
				joined = ft_strjoin(current->value, current->next->value);
				if (!joined)
					break ;
				free(current->value);
				current->value = joined;
				to_free = current->next;
				current->next = to_free->next;
				if (to_free->value)
					free(to_free->value);
				free(to_free);
			}
			var = current->value;
		}
		// Validate only key before '='
		pos = ft_strchr(var, '=');
		if (pos)
			*pos = '\0';
		if (!is_valid_identifier(var))
		{
			if (pos)
				*pos = '=';
			fprintf(stderr, "export: `%s': not a valid identifier\n",
				current->value);
			ret = 1;
			current = current->next;
			continue ;
		}
		if (pos)
			*pos = '=';
		// Move from local if bare name exists there, else set env
		pos = ft_strchr(current->value, '=');
		if (!pos)
		{
			local_var = get_var(current->value, context->local);
			if (local_var)
				context->env = set_env(ft_strdup(local_var), context->env);
			else
				context->env = set_env(ft_strdup(current->value), context->env);
		}
		else
			context->env = set_env(ft_strdup(current->value), context->env);
		if (!context->env)
		{
			perror("export");
			ret = 1;
		}
		current = current->next;
	}
	return (ret);
}
