/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: chuezeri <chuezeri@student.42.de>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/29 12:53:36 by chuezeri          #+#    #+#             */
/*   Updated: 2025/10/13 10:02:42 by chuezeri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char *get_project_root(void)
{
	static char project_root[PATH_MAX];
	char *cwd;
	char *minishell_pos;

	if (project_root[0] != '\0')
		return (project_root);
	cwd = getcwd(NULL, 0);
	if (!cwd)
		return (NULL);
	minishell_pos = ft_strnstr(cwd, "minishell", ft_strlen(cwd));
	if (minishell_pos)
	{
		while (*minishell_pos && *minishell_pos != '/')
			minishell_pos++;
		if (*minishell_pos == '/')
			minishell_pos++;
		*minishell_pos = '\0';
		ft_strcpy(project_root, cwd);
	}
	else
	{
		ft_strcpy(project_root, cwd);
	}
	free(cwd);
	return (project_root);
}

char *get_minishell_dir(void)
{
	char *project_root;
	char *dir_path;
	size_t len_total;

	project_root = get_project_root();
	if (!project_root)
		return (NULL);
	len_total = ft_strlen(project_root) + 1 + ft_strlen(MINSHELL_DIRECTORY) + 1;
	dir_path = malloc(len_total);
	if (!dir_path)
		return (NULL);
	ft_strcpy(dir_path, project_root);
	ft_strlcat(dir_path, "/", len_total);
	ft_strlcat(dir_path, MINSHELL_DIRECTORY, len_total);
	if (access(dir_path, F_OK) != 0)
	{
		if (mkdir(dir_path, 0700) != 0)
			return (free(dir_path), NULL);
	}
	return (dir_path);
}

char *get_history_path(void)
{
	static char *project_root;
	static char *history_path;

	project_root = get_project_root();
	history_path = NULL;
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
	return (NULL);
}

char **init_env(void)
{
	char **env;

	env = malloc(sizeof(char *));
	if (!env)
		return (NULL);
	env[0] = NULL;
	return (env);
}

void init_context(t_context *context, int argc, char **argv, char **env)
{
	context->argc = argc;
	context->argv = argv;
	context->env = copy_env(env);
	context->local = init_env();
	context->open[0] = 0;
	context->open[1] = 1;
	context->open[2] = -1;
	context->is_pipeline = false;
	context->is_heredoc = false;
	context->status = 0;
}
