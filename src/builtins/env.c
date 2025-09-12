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
		// Only print variables that have values (contain '=')
		if (ft_strchr(env[i], '='))
			printf("%s\n", env[i]);
		i++;
	}
	
	return (0);
}
