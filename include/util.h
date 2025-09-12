#ifndef UTIL_H
#define UTIL_H
/* src/util/env.c */
char **copy_env(char **env);
void free_env(char **env);
char **push_env(char *var, char **env);
char **set_env(char *var, char **env);
/* src/util/free.c */
void free_context(t_context *context);
/* src/util/ft_isspace.c */
int ft_isspace(int c);
/* src/util/ft_strcpy.c */
char *ft_strcpy(char *dest, const char *src);
int ft_strcmp(const char *s1, const char *s2);
/* src/util/ft_strndup.c */
char *ft_strndup(const char *s, size_t n);
/* src/util/init.c */
void init_context(t_context *context, int argc, char **argv, char **env);
#endif
