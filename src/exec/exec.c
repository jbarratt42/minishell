pid_t	traverse(t_node node, t_context context)
{
	int ret;
	pid_t lpid;
	pid_t rpid;

	if (node->is_terminal)
		return (exec_terminal(node->data.tokens, context));
	if (node->data.op.type != PIPE)
	{
		ret = collect(traverse(node->data.op.left, context), context);
		if (node->data.op.type == AND && ret != 0
				|| node->data.op.type == OR && ret == 0)
			return (ret);
		return (collect(traverse(node->data.op.right, context), context));
	}
	context->open[0] = context->[2];
	try_pipe(&open[1]);
	lpid = traverse(node->data.op.left, context);
	close(context->open[0]);
	context->open[0] = context->open[1];
	context->open[1] = context->open[2];
	context->open[2] = -1;
	rpid = traverse(node->data.op.right, context);
	close(context->open[1]);
	ret = collect(rpid, context);
	collect(lpid, context);
	return (ret);
}
