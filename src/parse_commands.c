/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_commands.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abenoit <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/10/21 17:10:03 by abenoit           #+#    #+#             */
/*   Updated: 2020/10/21 18:00:48 by abenoit          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include <stdlib.h>

/*static*/ t_cmd_code	get_command_code(char *arg)
{
	char	**command;
	int		i;

	i = 0;
	command = ft_split(BUILTINS, '/');
	if (command == NULL)
		return (MALLOC_ERR);
	while (command[i] != NULL)
	{
		if (ft_strcmp(arg, command[i]) == 0)
		{
			free_str_array(&command);
			return (i);
		}
		i++;
	}
	free_str_array(&command);
	return (ELSE);
}

int		parse_commands(t_list **commands)
{
	t_command	*cmd;
	t_list		*ptr;
//	t_token		*token;
	t_exe		exe;
	size_t		size;
	size_t		i;

	cmd = ft_lstpop(commands);
	ptr = cmd->tokens;
	size = ft_lstsize(ptr);
	exe.args = malloc((size + 1) * sizeof(char*));
	i = 0;
	while (ptr != NULL)
	{
		exe.args[i] = ft_lstpop(&ptr);
		i++;
	}
	exe.args[i] = NULL;
//	print_tokens(*commands);
	ft_printarray_fd(exe.args, 1);
//	all->command = get_command_code(all->current[0]);
//	if ((ret = launch_command(all)) != SUCCESS)
//		return (ret);
	return (0);
}
