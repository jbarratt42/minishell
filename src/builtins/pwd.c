#include "minishell.h"

int	builtin_pwd(t_token *tokens, t_context *context)
{
	char	*cwd;
	char	*pwd_env;

	(void)tokens;
	
	// Try to get current working directory
	cwd = getcwd(NULL, 0);
	if (cwd)
	{
		printf("%s\n", cwd);
		free(cwd);
		return (0);
	}
	
	// Fallback to PWD environment variable
	pwd_env = ft_getenv("PWD", context->env);
	if (pwd_env)
	{
		printf("%s\n", pwd_env);
		return (0);
	}
	
	// If all else fails
	perror("pwd");
	return (1);
}
