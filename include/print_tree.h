/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   print_tree.h                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: chuezeri <chuezeri@student.42.de>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/06 13:14:31 by chuezeri          #+#    #+#             */
/*   Updated: 2025/09/06 13:26:47 by chuezeri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

void print_indentation(int depth);
const char *get_toktype_str(enum e_token_type type);
void print_tree_structure(t_node *node, int depth);
