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
    bool is_interactive = isatty(STDIN_FILENO);

    init_context(&context, argc, argv, env);

    signal(SIGINT, signal_handler);
    // signal(SIGTERM, signal_handler);
    signal(SIGQUIT, SIG_IGN);
    // prompt readline loop +history

    while (true)
    {
        context.input = readline(MINISHELL_PROMPT);
        if (!context.input)
        {
            if (!is_interactive)
                break;
            builtin_exit(context.tokens, &context);
        }
        // Tokenize input
        if (is_interactive)
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
		if (pid == -1)
			context.status = 1;
        if (pid && pid != -1)
        {
            if (waitpid(pid, &context.status, 0) == -1 || !WIFEXITED(context.status))
                perror("main");
            context.status = WEXITSTATUS(context.status);
        }
        char *project_root = get_project_root();
        char *history_path = NULL;
        if (project_root)
        {
            history_path = malloc(ft_strlen(project_root) + ft_strlen(MINSHELL_DIRECTORY) + 10);
            if (history_path)
            {
                ft_strcpy(history_path, project_root);
                ft_strlcat(history_path, "/", ft_strlen(project_root) + ft_strlen(MINSHELL_DIRECTORY) + 10);
                ft_strlcat(history_path, MINSHELL_DIRECTORY, ft_strlen(project_root) + ft_strlen(MINSHELL_DIRECTORY) + 10);
                ft_strlcat(history_path, "/history", ft_strlen(project_root) + ft_strlen(MINSHELL_DIRECTORY) + 10);
            }
        }

        if (history_path)
        {
            ft_write_history(history_path, context.input);
            if (ft_strcmp(context.input, "exit") == 0)
            {
                free(history_path);
                free(context.input);
                free_context(&context);
                exit(EXIT_SUCCESS);
            }
            free(history_path);
        }

        free(context.input);
        if (!is_interactive)
        {
            // In non-interactive mode, exit with the status of the last command
            free_context(&context);
            exit(context.status);
        }
    }

    free_context(&context);
    return (EXIT_SUCCESS);
}
