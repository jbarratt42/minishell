#include "minishell.h"
#include <limits.h>

char	*get_project_root(void)
{
	static char	project_root[PATH_MAX];
	char		*cwd;
	char		*minishell_pos;
	
	if (project_root[0] != '\0')
		return (project_root);
	
	cwd = getcwd(NULL, 0);
	if (!cwd)
		return (NULL);
	
	// Look for the project root by finding where we are relative to the executable
	// or by looking for a marker file like Makefile
	minishell_pos = ft_strnstr(cwd, "minishell", ft_strlen(cwd));
	if (minishell_pos)
	{
		// Find the end of the minishell directory name
		while (*minishell_pos && *minishell_pos != '/')
			minishell_pos++;
		if (*minishell_pos == '/')
			minishell_pos++;
		// Truncate at the end of minishell directory
		*minishell_pos = '\0';
		ft_strcpy(project_root, cwd);
	}
	else
	{
		// Fallback: use current directory
		ft_strcpy(project_root, cwd);
	}
	
	free(cwd);
	return (project_root);
}

void	init_context(t_context *context, int argc, char **argv, char **env)
{
	context->argc = argc;
	context->argv = argv;
	context->env = copy_env(env);
	context->local = NULL;
	context->open[0] = 0;
	context->open[1] = 1;
	context->open[2] = -1;
	context->is_pipeline = false;
	context->status = 0;
}
