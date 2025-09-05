#include "minishell.h"

void	init_context(t_context *context, int argc, char **argv, char **env)
{
	context->argc = argc;
	context->argv = argv;
	context->env = copy_env(env);
	context->open[0] = 0;
	context->open[1] = 1;
	context->open[2] = -1;
}
