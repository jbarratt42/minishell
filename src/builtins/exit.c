#include "minishell.h"

static bool is_numeric(const char *str)
{
	if (!str || !*str)
		return (false);

	// Optional sign
	if (*str == '-' || *str == '+')
		str++;

	while (*str)
	{
		if (!ft_isdigit(*str))
			return (false);
		str++;
	}

	return (true);
}

int builtin_exit(t_token *tokens, t_context *context)
{
	t_token *current;
	int exit_code;

	current = tokens;

	// Skip the command name
	if (current && current->type == WORD)
		current = current->next;

	// Default exit code is 0
	exit_code = 0;

	// If there's an argument, parse it
	if (current && current->type == WORD)
	{
		// too many arguments with numeric first argument → do not exit; return 2
		if (current->next && current->next->type == WORD && is_numeric(current->value))
		{
			fprintf(stderr, "exit: too many arguments\n");
			return (2);
		}
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
	printf("exit\n");
	// Clean up and exit
	free_context(context);
	exit(exit_code);
}
