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

	// Count exported variables
	count = 0;
	i = 0;
	while (env && env[i])
	{
		if (ft_strchr(env[i], '='))
			count++;
		i++;
	}
	
	if (count == 0)
		return;
	
	// Create sorted array
	sorted_env = malloc((count + 1) * sizeof(char *));
	if (!sorted_env)
		return;
	
	// Copy exported variables
	j = 0;
	i = 0;
	while (env && env[i])
	{
		if (ft_strchr(env[i], '='))
			sorted_env[j++] = env[i];
		i++;
	}
	sorted_env[j] = NULL;
	
	// Simple bubble sort
	for (i = 0; i < count - 1; i++)
	{
		for (j = 0; j < count - i - 1; j++)
		{
			if (ft_strncmp(sorted_env[j], sorted_env[j + 1], ft_strlen(sorted_env[j]) + 1) > 0)
			{
				char *temp = sorted_env[j];
				sorted_env[j] = sorted_env[j + 1];
				sorted_env[j + 1] = temp;
			}
		}
	}
	
	// Print in format "declare -x VAR=value"
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
	int		ret;

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
		
		// Check if it's a valid identifier
		if (!is_valid_identifier(var))
		{
			fprintf(stderr, "export: `%s': not a valid identifier\n", var);
			ret = 1;
		}
		else
		{
			// Add to environment
			context->env = set_env(ft_strdup(var), context->env);
			if (!context->env)
			{
				perror("export");
				ret = 1;
			}
		}
		
		current = current->next;
	}
	
	return (ret);
}
