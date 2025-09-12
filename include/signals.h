/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signals.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: chuezeri <chuezeri@student.42.de>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/30 15:10:38 by chuezeri          #+#    #+#             */
/*   Updated: 2025/09/12 18:29:15 by chuezeri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SIGNALS_H
#define SIGNALS_H

#include <signal.h>

#define SIG_DEFAULT 128

void signal_handler(int sig);

#endif