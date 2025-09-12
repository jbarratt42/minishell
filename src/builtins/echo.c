#include "minishell.h"

int	builtin_echo(t_token *tokens, t_context *context)
{
	bool	newline;
	t_token	*current;

	(void)context;
	newline = true;
	current = tokens;
	
	// Skip the command name
	if (current && current->type == WORD)
		current = current->next;
	
	// Check for -n flag
	while (current && current->type == WORD && current->value[0] == '-')
	{
		int i = 1;
		while (current->value[i] == 'n')
			i++;
		if (current->value[i] == '\0' && i > 1)
		{
			newline = false;
			current = current->next;
		}
		else
			break;
	}
	
	// Print arguments
	while (current && current->type == WORD)
	{
		printf("%s", current->value);
		current = current->next;
		if (current && current->type == WORD)
			printf(" ");
	}
	
	if (newline)
		printf("\n");
	
	return (0);
}
