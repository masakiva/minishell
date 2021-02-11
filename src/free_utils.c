/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mvidal-a <mvidal-a@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/02/11 16:57:40 by mvidal-a          #+#    #+#             */
/*   Updated: 2021/02/11 16:57:42 by mvidal-a         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	free_command(void *content)
{
	t_command	*command;

	command = (t_command *)content;
	free_str_array(command->args);
	free_str_array(command->redir_paths);
	free(command->redir_types);
	free(command);
}

void	free_commands(t_list **commands)
{
	ft_lstclear(commands, free_command);
}
