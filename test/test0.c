#include "minishell.h"
//#include "print_tree.c"

int	main(int argc, char **argv, char **env)
{
	t_context	context;
	t_token		*tokens;

	if (argc != 2)
		return (1);
	if(!init_context(argc, argv, env, &context))
		return (1);
	context.line = ft_strdup(argv[1]);
	expand_all(&context);
	context.tokens = lex(&context);
	print_tokens(context.tokens);
	tokens = context.tokens;
	context.tree = parse(&tokens, 0);
	print_tree_structure(context.tree, 0);
	traverse(context.tree);
	free_context(&context);
	return (0);
}
