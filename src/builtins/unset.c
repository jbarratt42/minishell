#include "minishell.h"

static bool	is_valid_identifier(const char *str)
{
	int	i;

	if (!str || !*str)
		return (false);
	
	// First character must be letter or underscore
	if (!ft_isalpha(*str) && *str != '_')
		return (false);
	
	i = 1;
	while (str[i])
	{
		if (!ft_isalnum(str[i]) && str[i] != '_')
			return (false);
		i++;
	}
	
	return (true);
}

static void	unset_from_env(char **env, const char *name)
{
	char	**p;
	char	*equal_pos;
	int	name_len;

	if (!env || !name)
		return;
	
	name_len = ft_strlen(name);
	p = env;
	
	while (*p)
	{
		equal_pos = ft_strchr(*p, '=');
		if (equal_pos && (equal_pos - *p) == name_len)
		{
			if (ft_strcmp(*p, name) == 0)
			{
				// Found the variable, remove it
				free(*p);
				// Shift remaining elements
				while (*p)
				{
					*p = *(p + 1);
					p++;
				}
				return;
			}
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
	
	// Skip the command name
	if (current && current->type == WORD)
		current = current->next;
	
	// If no arguments, do nothing
	if (!current || current->type != WORD)
		return (0);
	
	// Process each argument
	while (current && current->type == WORD)
	{
		if (!is_valid_identifier(current->value))
		{
			fprintf(stderr, "unset: `%s': not a valid identifier\n", current->value);
			ret = 1;
		}
		else
		{
			// Remove from both env and local
			unset_from_env(context->env, current->value);
			unset_from_env(context->local, current->value);
		}
		
		current = current->next;
	}
	
	return (ret);
}
