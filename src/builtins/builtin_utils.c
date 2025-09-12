#include "minishell.h"

bool	is_builtin_command(const char *cmd)
{
	const char	*builtins[] = {
		"echo", "cd", "pwd", "export", "unset", "env", "exit", NULL
	};
	int			i;

	if (!cmd)
		return (false);
	
	i = 0;
	while (builtins[i])
	{
		if (ft_strcmp(cmd, builtins[i]) == 0)
			return (true);
		i++;
	}
	
	return (false);
}

int	execute_builtin(const char *cmd, t_token *tokens, t_context *context)
{
	if (ft_strncmp(cmd, "echo", 4) == 0)
		return (builtin_echo(tokens, context));
	else if (ft_strncmp(cmd, "cd", 2) == 0)
		return (builtin_cd(tokens, context));
	else if (ft_strncmp(cmd, "pwd", 3) == 0)
		return (builtin_pwd(tokens, context));
	else if (ft_strncmp(cmd, "export", 6) == 0)
		return (builtin_export(tokens, context));
	else if (ft_strncmp(cmd, "unset", 5) == 0)
		return (builtin_unset(tokens, context));
	else if (ft_strncmp(cmd, "env", 3) == 0)
		return (builtin_env(tokens, context));
	else if (ft_strncmp(cmd, "exit", 4) == 0)
		return (builtin_exit(tokens, context));
	
	return (1); // Command not found
}
