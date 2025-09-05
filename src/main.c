#include "minishell.h"

int g_status = 0;

bool minishell_init(t_context *context, int argc, char **argv, char **env)
{
    if (!context)
		return (false);

	init_context(context, argc, argv, env);
    printf("███╗   ███╗██╗███╗   ██╗██╗███████╗██╗  ██╗███████╗██╗     ██╗\n");
    printf("████╗ ████║██║████╗  ██║██║██╔════╝██║  ██║██╔════╝██║     ██║\n");
    printf("██╔████╔██║██║██╔██╗ ██║██║███████╗███████║█████╗  ██║     ██║\n");
    printf("██║╚██╔╝██║██║██║╚██╗██║██║╚════██║██╔══██║██╔══╝  ██║     ██║\n");
    printf(
        "██║ ╚═╝ ██║██║██║ ╚████║██║███████║██║  ██║███████╗███████╗███████╗\n");
    printf(
        "╚═╝     ╚═╝╚═╝╚═╝  ╚═══╝╚═╝╚══════╝╚═╝  ╚═╝╚══════╝╚══════╝╚══════╝\n");

    return (true);
}

int main(int argc, char **argv, char **env)
{
    t_context	context;
	t_token		*token;
	pid_t		pid;

    if (!minishell_init(&context, argc, argv, env))
        return (EXIT_FAILURE);

    // prompt readline loop +history
    context.input = readline("$ ");

    while (context.input)
    {
        // Tokenize input
        add_history(context.input);
		// maybe do expand at the token level
		//expand(&context);
        context.tokens = lex(context.input);
        if (!context.tokens)
            (free(context.input), g_status = EXIT_FAILURE);
		token = context.tokens;
		context.tree = parse(&token, 0);
		pid = traverse(context.tree, &context);
		if (pid)
		{
			if(waitpid(pid, &context.status, 0) == -1
					|| !WIFEXITED(context.status))
				perror("main");
			context.status = WEXITSTATUS(pid);
		}

#ifdef DEBUG
        if (context.tokens)
            print_tokens(context.tokens);
		printf("\nParse Tree:\n");
		print_tree_structure(context.tree, 0);
#endif

        // add_history(context.input);
        ft_write_history(MINSHELL_DIRECTORY "/history", context.input);
        if (ft_strncmp(context.input, "exit", 4) == 0)
        {
            free(context.input);
            exit(EXIT_SUCCESS);
        }
        if (ft_strncmp(context.input, "history", 7) == 0)
            print_history(MINSHELL_DIRECTORY "/history");

        context.input = readline("$ ");
    }

    return (EXIT_SUCCESS);
}
