#include "minishell.h"

bool	is_builtin_command(const char *cmd)
{
	const char	*builtins[] = {
		"echo", "cd", "pwd", "export", "unset", "env", "exit", "history", NULL
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
	else if (ft_strncmp(cmd, "history", 7) == 0)
		return (builtin_history(tokens, context));
	
	return (1); // Command not found
}

int	builtin_history(t_token *tokens, t_context *context)
{
	char	*project_root;
	char	*history_path;
	int		result;

	(void)tokens;
	(void)context;
	
	project_root = get_project_root();
	if (!project_root)
	{
		fprintf(stderr, "history: unable to determine project root\n");
		return (1);
	}
	
	history_path = malloc(ft_strlen(project_root) + ft_strlen(MINSHELL_DIRECTORY) + 10);
	if (!history_path)
	{
		fprintf(stderr, "history: memory allocation failed\n");
		return (1);
	}
	
	ft_strcpy(history_path, project_root);
	ft_strlcat(history_path, "/", ft_strlen(project_root) + ft_strlen(MINSHELL_DIRECTORY) + 10);
	ft_strlcat(history_path, MINSHELL_DIRECTORY, ft_strlen(project_root) + ft_strlen(MINSHELL_DIRECTORY) + 10);
	ft_strlcat(history_path, "/history", ft_strlen(project_root) + ft_strlen(MINSHELL_DIRECTORY) + 10);
	
	result = print_history(history_path);
	free(history_path);
	return (result);
}
