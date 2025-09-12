#include "minishell.h"

int g_status = 0;

bool minishell_init(t_context *context, int argc, char **argv, char **env)
{
    if (!context)
        return (false);

    context->argc = argc;
    context->argv = argv;
    context->env = env;
    context->status = EXIT_SUCCESS;

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
    pid_t pid;
    t_context context;

    init_context(&context, argc, argv, env);

    signal(SIGINT, signal_handler);
    signal(SIGQUIT, SIG_IGN);
    // signal(SIGTERM, signal_handler);
    // prompt readline loop +history
    context.input = readline(MINISHELL_PROMPT);

    while (context.input)
    {
        // Tokenize input
        add_history(context.input);
        // maybe do expand at the token level
        // expand(&context);
        context.tokens = lex(context.input);
        if (!context.tokens)
            (free(context.input), g_status = EXIT_FAILURE);
#ifdef DEBUG
        if (context.tokens)
            print_tokens(context.tokens);
#endif
		context.tree = parse(&context.tokens, 0);
#ifdef DEBUG
        printf("\nParse Tree:\n");
        print_tree_structure(context.tree, 0);
#endif
        pid = traverse(context.tree, &context);
        if (pid)
        {
            if (waitpid(pid, &context.status, 0) == -1
					|| !WIFEXITED(context.status))
                perror("main");
            context.status = WEXITSTATUS(context.status);
        }
        ft_write_history(MINSHELL_DIRECTORY "/history", context.input);
        if (ft_strncmp(context.input, "exit", 4) == 0)
        {
            free(context.input);
            free_context(&context);
            exit(EXIT_SUCCESS);
        }
        if (ft_strncmp(context.input, "history", 7) == 0)
            print_history(MINSHELL_DIRECTORY "/history");

        context.input = readline(MINISHELL_PROMPT);
    }

    free_context(&context);
    return (EXIT_SUCCESS);
}
