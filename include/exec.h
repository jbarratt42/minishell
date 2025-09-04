/* src/exec/exec.c */
bool try_dup2(int open[3]);
char **get_args(t_token *tokens);
char *get_path(t_token *tokens, char **env);
bool is_builtin(t_token *token);
void exec_builtin(t_token *token, t_context *context);
pid_t exec_terminal(t_token *tokens, t_context *context);
void exec_sequential(t_node *node, t_context *context);
pid_t traverse(t_node *node, t_context *context);
