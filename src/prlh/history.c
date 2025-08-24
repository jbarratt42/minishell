/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   history.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: chuezeri <chuezeri@student.42.de>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/24 15:08:33 by chuezeri          #+#    #+#             */
/*   Updated: 2025/08/24 17:03:53 by chuezeri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char *read_file(int fd)
{
    char *str;
    char *new_str;
    char buffer[BUFFER_SIZE];
    ssize_t bytes_read;
    size_t total_size;

    str = NULL;
    total_size = 0;
    bytes_read = read(fd, buffer, BUFFER_SIZE);
    while (bytes_read > 0)
    {
        new_str = ft_calloc(total_size + bytes_read + 1, sizeof(char));
        if (!new_str)
            return (free(str), NULL);
        if (str)
            ft_memcpy(new_str, str, total_size);
        ft_memcpy(new_str + total_size, buffer, bytes_read);
        total_size += bytes_read;
        free(str);
        bytes_read = read(fd, buffer, BUFFER_SIZE);
        str = new_str;
    }
    return (str);
}

char *ft_read_history(const char *filename)
{
    int fd;
    char *contents;

    fd = open(filename, O_RDONLY);
    if (fd == -1)
        return (NULL);
    contents = read_file(fd);
    close(fd);
    return (contents);
}

// TODO: Make this session only
int ft_write_history(const char *filename, char *history)
{
    int fd;
    int len;

    len = ft_strlen((const char *)history);
    if (!filename)
        return (perror("Invalid filename"), -EXIT_FAILURE);

    fd = open(filename, O_WRONLY | O_CREAT | O_APPEND, 0644);
    if (fd == -1)
        return (perror("Failed to open history file"), -EXIT_FAILURE);

    while (*history && ft_isspace(*history) == EXIT_FAILURE)
        history++;

    if (*history)
    {
        write(fd, history, len);
        write(fd, "\n", 1);
    }

    close(fd);
    return (EXIT_SUCCESS);
}

int print_history(const char *filename)
{
    char **history;

    history = ft_split(ft_read_history(filename), '\n');
    if (!history)
        return (perror("Failed to read history file"), -EXIT_FAILURE);

    int i = 0;
    while (history[i])
    {
        printf("%5d %s\n", i, history[i]);
        free(history[i]);
        i++;
    }
    free(history);
    return (EXIT_SUCCESS);
}