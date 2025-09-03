static bool	try_close2(int open[2])
{
	int	i;

	i = 0;
	while (i < 2)
		if (open[i] > 2)
			if (close(open[i]) == -1)
			{
				perror("try_close");
				return (false);
			}
	return (true);
}

static bool	try_pipe(int fds[2])
{
	if (pipe(fds) == -1)
	{
		perror("try_pipe");
		return (false);
	}
	return (true);
}

static int	collect(int pids[2])
{
	int	wstatus[2];
	int	i;

	i = 1
	while (i >= 0)
	{
		wstatus[i] = 0;
		if (pids[i] 
				&& waitpid(pids[i], &wstatus[i], 0) == -1
				|| !WIFEXITED(wstatus[i]))
		{
			perror("collect");
			return (-1);
		}
		i--;
	}
	return(WEXITSTATUS(wstatus[1]));
}

bool	try_dup2(int open[3])
{
	int	i;

	i = 0;
	while (i < 2)
	{
		if(dup2(open[i], i) == -1)
			return (false);
		i++;
	}
	i = 0;
	while (i < 3)
		if (open[i] > 2)
			if (close(open[i]) == -1)
				return (false);
	return (true);
}

char	**get_args(t_token tokens)
{
	int 	i;
	char	**args;
	t_token	*pos;

	i = 0;
	pos = tokens->next;
	while (pos)
	{
		pos = pos->next;
		i++;
	}
	args = malloc(i * sizeof(char *));
	if (!args)
		return (NULL);
	i = 0;
	pos = tokens->next;
	while (pos)
	{
		args[i++] = pos->value;
		pos = pos->next;
	}
	return (args);
}

char	*get_path(t_token *tokens)
{
	return (tokens->value);
}

pid_t	exec_terminal(t_token *tokens, t_context *context)
{
	char	**args;
	pid_t	pid;

	pid = fork();
	if (pid)
		return (pid);
	try_dup2(context->open);
	if (is_builtin(tokens))
		exec_builtin(tokens);
	execve(get_path(tokens), get_args(tokens), context->env);
	perror("exec_terminal");
	exit(1);
}
		
pid_t	traverse(t_node *node, t_context *context)
{
	int 	ret;
	pid_t	pids[2];
	if (node->is_terminal)
		return (exec_terminal(node->data.tokens, context));
	if (node->data.op.type != PIPE)
	{
		ret = collect(traverse(node->data.op.left, context), context);
		if ((node->data.op.type == AND && ret != 0)
				|| (node->data.op.type == OR && ret == 0))
			context->status = ret;
		else
			context->status = collect(traverse(node->data.op.right));
			return (0);
		return (collect(traverse(node->data.op.right)));
	}
	if (!try_pipe(&context->open[1]));
		return (-1);
	pid[0] = traverse(node->data.op.left, context);
	if (!try_close2(context->open))
		return (-1);
	context->open[0] = context->open[2];
	context->open[1] = 1;
	context->open[2] = -1;
	pid[1] = traverse(node->data.op.right, context);
	context->status = collect(pids);
	return (0);
}
