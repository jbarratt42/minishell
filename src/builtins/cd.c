#include "minishell.h"
#include <sys/stat.h>

static char	*get_home_path(t_context *context)
{
	char	*home;

	home = ft_getenv("HOME", context->env);
	if (!home)
		home = ft_getenv("HOME", context->local);
	return (home);
}

static void	update_pwd_vars(t_context *context, char *old_pwd)
{
	char	*new_pwd;
	char	*pwd_var;
	char	*old_pwd_var;

	new_pwd = getcwd(NULL, 0);
	if (!new_pwd)
	{
		perror("cd: getcwd");
		return;
	}
	
	// Update PWD
	pwd_var = ft_strjoin("PWD=", new_pwd);
	if (pwd_var)
	{
		context->env = set_env(pwd_var, context->env);
	}
	
	// Update OLDPWD
	if (old_pwd)
	{
		old_pwd_var = ft_strjoin("OLDPWD=", old_pwd);
		if (old_pwd_var)
		{
			context->env = set_env(old_pwd_var, context->env);
		}
		free(old_pwd);
	}
	
	free(new_pwd);
}

int	builtin_cd(t_token *tokens, t_context *context)
{
	char	*path;
	char	*old_pwd;
	t_token	*current;

	current = tokens;
	
	// Skip the command name
	if (current && current->type == WORD)
		current = current->next;
	
	// Get current directory for OLDPWD
	old_pwd = getcwd(NULL, 0);
	
	// Determine target path
	if (!current || current->type != WORD)
	{
		path = get_home_path(context);
		if (!path)
		{
			fprintf(stderr, "cd: HOME not set\n");
			free(old_pwd);
			return (1);
		}
	}
	else
		path = current->value;
	
	// Handle special cases
	if (ft_strncmp(path, "-", 1) == 0)
	{
		path = ft_getenv("OLDPWD", context->env);
		if (!path)
		{
			fprintf(stderr, "cd: OLDPWD not set\n");
			free(old_pwd);
			return (1);
		}
	}
	
	// Change directory
	if (chdir(path) == -1)
	{
		perror("cd");
		free(old_pwd);
		return (1);
	}
	
	// Update environment variables
	update_pwd_vars(context, old_pwd);
	
	// Print new directory if cd -
	if (current && current->type == WORD && ft_strncmp(current->value, "-", 1) == 0)
	{
		char *new_pwd = getcwd(NULL, 0);
		if (new_pwd)
		{
			printf("%s\n", new_pwd);
			free(new_pwd);
		}
	}
	
	return (0);
}
