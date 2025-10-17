/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: chuezeri <chuezeri@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/29 12:48:42 by chuezeri          #+#    #+#             */
/*   Updated: 2025/10/17 11:13:19 by chuezeri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	export_variable_to_env(const char *value, t_context *context)
{
	char	*copy;

	copy = ft_strdup(value);
	if (!copy)
	{
		perror("export");
		return ;
	}
	context->env = set_env(copy, context->env);
	if (!context->env)
		perror("export");
}

void	process_export_var(char *value, t_context *context)
{
	char	*pos;
	char	*local_var;

	pos = ft_strchr(value, '=');
	if (!pos)
	{
		local_var = get_var(value, context->local);
		if (local_var)
			export_variable_to_env(local_var, context);
		else
			export_variable_to_env(value, context);
	}
	else
	{
		export_variable_to_env(value, context);
	}
}

void	merge_adjacent_assignments(t_token *current)
{
	char	*joined;
	t_token	*to_free;

	while (current->value && ft_strchr(current->value, '=') && current->next
		&& current->next->type == WORD && current->next->value)
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
}

int	handle_export_argument(t_token **current_ptr, t_context *context)
{
	t_token	*current;
	char	*var;
	char	*pos;

	current = *current_ptr;
	var = current->value;
	pos = ft_strchr(var, '=');
	if (pos)
		*pos = '\0';
	if (!is_valid_identifier(var))
	{
		if (pos)
			*pos = '=';
		err_printf("export: `%s': not a valid identifier\n", current->value);
		*current_ptr = current->next;
		return (1);
	}
	if (pos)
		*pos = '=';
	process_export_var(current->value, context);
	*current_ptr = current->next;
	return (0);
}

int	builtin_export(t_token *tokens, t_context *context)
{
	t_token	*current;
	int		ret;

	current = tokens;
	ret = 0;
	if (current && current->type == WORD)
		current = current->next;
	if (!current || current->type != WORD)
	{
		print_exported_vars(context->env);
		return (0);
	}
	while (current && current->type == WORD)
		ret |= handle_export_argument(&current, context);
	return (ret);
}
