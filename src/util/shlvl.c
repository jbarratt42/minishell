#include "minishell.h"

static bool	increment_shlvl(char *name, t_context *context)
{
	int		shlvl;
	char	*shlvl_str;
	char	*assign_str;
	int		buf_size;
	
	if(ft_getenv(name, context->env))
		shlvl = ft_atoi(ft_getenv(name, context->env));
	else
		shlvl = 0;
	shlvl++;
	shlvl_str = ft_itoa(shlvl);
	if (!shlvl_str)
		return (false);
	buf_size = strlen(name) + 1 + ft_strlen(shlvl_str) + 1; 
	assign_str = malloc(buf_size);
	if (!assign_str)
		return (free(shlvl_str), false);
	assign_str = ft_memcpy(assign_str, name, ft_strlen(name) + 1);
	ft_strlcat(assign_str, "=", buf_size);
	ft_strlcat(assign_str, shlvl_str, buf_size);
	context->env = set_env(assign_str, context->env);
	free(shlvl_str);
	return (true);
}

bool	set_shell_levels(t_context *context)
{
	if (!increment_shlvl("SHLVL", context))
		return (false);
	if (ft_getenv("MINISHLVL", context->env))
		context->sigint_handler = nested_handler;
	else
		context->sigint_handler = signal_handler;
	if (!increment_shlvl("MINISHLVL", context))
		return (false);
	return (true);
}
