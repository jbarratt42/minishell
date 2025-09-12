#include "minishell.h"

static bool	is_numeric(const char *str)
{
	if (!str || !*str)
		return (false);
	
	// Handle negative numbers
	if (*str == '-')
		str++;
	
	// Must have at least one digit
	if (!*str || !ft_isdigit(*str))
		return (false);
	
	// All remaining characters must be digits
	while (*str)
	{
		if (!ft_isdigit(*str))
			return (false);
		str++;
	}
	
	return (true);
}

int	builtin_exit(t_token *tokens, t_context *context)
{
	t_token	*current;
	int		exit_code;

	current = tokens;
	
	// Skip the command name
	if (current && current->type == WORD)
		current = current->next;
	
	// Default exit code is 0
	exit_code = 0;
	
	// If there's an argument, parse it
	if (current && current->type == WORD)
	{
		if (!is_numeric(current->value))
		{
			fprintf(stderr, "exit: %s: numeric argument required\n", current->value);
			exit_code = 2;
		}
		else
		{
			exit_code = ft_atoi(current->value);
			// Handle overflow - bash uses 256 modulo
			if (exit_code < 0)
				exit_code = 256 + (exit_code % 256);
			else if (exit_code > 255)
				exit_code = exit_code % 256;
		}
	}
	
	// Clean up and exit
	free_context(context);
	printf("exit\n");
	exit(exit_code);
}
