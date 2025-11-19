/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   util.h                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: chuezeri <chuezeri@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/29 13:06:47 by chuezeri          #+#    #+#             */
/*   Updated: 2025/11/19 11:56:42 by jbarratt         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef UTIL_H
# define UTIL_H

/* prototypes */
/* src/util/env.c */
char	**copy_env(char **env);
char	**push_env(char *var, char **env);
char	**set_env(char *var, char **env);
char	*get_var(char *name, char **env);
char	*ft_getenv(char *name, char **env);
/* src/util/free.c */
void	free_tokens(t_token *token);
void	free_node(t_node *node);
void	free_env(char **env);
void	free_context(t_context *context);
void	cleanup_and_exit(t_context *context);
/* src/util/shlvl.c */
bool	set_shell_levels(t_context *context);
/* src/util/ft_fprintf.c */
int		err_printf(const char *format, ...);
/* src/util/ft_isspace.c */
int		ft_isspace(int c);
/* src/util/ft_strcmp.c */
int		ft_strcmp(const char *s1, const char *s2);
/* src/util/ft_strcpy.c */
char	*ft_strcpy(char *dest, const char *src);
/* src/util/ft_strndup.c */
char	*ft_strndup(const char *s, size_t n);
/* src/util/init.c */
char	*get_project_root(void);
char	*get_history_path(void);
char	**init_env(void);
void	init_context(t_context *context, int argc, char **argv, char **env);
#endif
