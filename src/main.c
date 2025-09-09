#include "minishell.h"

int g_status = 0;

bool minishell_init(t_context *context, int argc, char **argv, char **env) {
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

int main(int argc, char **argv, char **env) {
  t_context context;
  printf("Size of void*: %zu\n", sizeof(void *));

  if (!minishell_init(&context, argc, argv, env))
    return (EXIT_FAILURE);

  handle_signals(&context);
  signal(SIGINT, signal_handler);
  signal(SIGQUIT, SIG_IGN);
  // signal(SIGTERM, signal_handler);
  // prompt readline loop +history
  context.input = readline("$ ");

  while (context.input) {
    // Tokenize input
    add_history(context.input);
    expand(&context);
    t_token *tokens = lex(context.input);
    if (!tokens)
      (free(context.input), g_status = EXIT_FAILURE);

#ifdef DEBUG
    if (tokens)
      print_tokens(tokens);
#endif

    // add_history(context.input);
    ft_write_history(MINSHELL_DIRECTORY "/history", context.input);
    if (ft_strncmp(context.input, "exit", 4) == 0) {
      free(context.input);
      exit(EXIT_SUCCESS);
    }
    if (ft_strncmp(context.input, "history", 7) == 0)
      print_history(MINSHELL_DIRECTORY "/history");

    context.input = readline("$ ");
  }

  return (EXIT_SUCCESS);
}
