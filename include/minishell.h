/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: chuezeri <chuezeri@student.42.de>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/14 10:46:56 by jbarratt          #+#    #+#             */
/*   Updated: 2025/08/24 16:32:15 by chuezeri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
#define MINISHELL_H

#define MINSHELL_DIRECTORY ".minishell"

#include <unistd.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <stdio.h>
#include <stdbool.h>

#include "prlh.h"
#include "../libft/libft.h"

typedef struct s_minishell
{
    t_context *context;
    bool interactive;
} t_minishell;

#endif
