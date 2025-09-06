/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   prlh.h                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: chuezeri <chuezeri@student.42.de>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/24 13:13:51 by chuezeri          #+#    #+#             */
/*   Updated: 2025/09/06 13:27:27 by chuezeri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

// prompt readline loop +history
#ifndef PRLH_H
#define PRLH_H

#define BUFFER_SIZE 1024
#define MAX_OPEN_FDS 3

#include <readline/readline.h>
#include <readline/history.h>
#include <sys/fcntl.h>

typedef struct s_token t_token;

typedef struct s_node t_node;

typedef struct s_context
{
    int argc;
    char **argv;
    char **env;
    char *input;
    t_token *tokens;
    t_node *tree;
    int open[MAX_OPEN_FDS];
    int status;
} t_context;

/**
 * @brief Reads a input from the prompt.
 * @return The input read from the prompt.
 */
bool read_prompt(t_context *context);

/*
 * @param fd The file descriptor of the file to read.
 * @brief Read the entire contents of a file into a string.
 * @return A pointer to the newly allocated string, or NULL on failure.
 */
char *read_file(int fd);

/**
 * @brief Read the entire contents of a file into a string.
 * @param filename The name of the file to read.
 * @return A pointer to the newly allocated string, or NULL on failure.
 */
char *ft_read_history(const char *filename);

/**
 * @brief Write the entire contents of a string to a file.
 * @param filename The name of the file to write to.
 * @param history The string to write to the file.
 * @return EXIT_SUCCESS on success, or -EXIT_FAILURE on failure.
 */
int ft_write_history(const char *filename, char *history);

/**
 * @brief Print the contents of the history file.
 * @param filename The name of the history file to print.
 * @return EXIT_SUCCESS on success, or -EXIT_FAILURE on failure.
 */
int print_history(const char *filename);

#endif
