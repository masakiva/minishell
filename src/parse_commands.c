/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_commands.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abenoit <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/10/21 17:10:03 by abenoit           #+#    #+#             */
/*   Updated: 2020/10/22 12:00:08 by abenoit          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include <stdlib.h>

/*static*/ t_cmd_code	get_cmd_code(char *arg)
{
	char	**cmd_list;
	int		i;

	i = 0;
	cmd_list = ft_split(BUILTINS, '/');
	if (cmd_list == NULL)
		return (MALLOC_ERR);
	while (cmd_list[i] != NULL)
	{
		if (ft_strcmp(arg, cmd_list[i]) == 0)
		{
			free_str_array(&cmd_list);
			return (i);
		}
		i++;
	}
	free_str_array(&cmd_list);
	return (ELSE);
}

int		parse_commands(t_list **commands, char ***env)
{
	t_command	*cmd;
	t_list		*ptr;
	t_token		*token;
	t_exe		exe;
	size_t		size;
	int			ret;
	size_t		i;

	cmd = ft_lstpop(commands);
	ptr = cmd->tokens;
	size = ft_lstsize(ptr);
	exe.args = malloc((size + 1) * sizeof(char*));
	i = 0;
	while (ptr != NULL)
	{
		token = ft_lstpop(&ptr);
		// do checks here for pipes, redir, etc...
		exe.args[i] = token->str;
		free(token);
		i++;
	}
	exe.args[i] = NULL;
//	print_tokens(*commands);
	exe.cmd_code = get_cmd_code(exe.args[0]);
	exe.env = &(*env);
	if ((ret = launch_command(&exe)) != SUCCESS)
		return (ret);
//	ft_printarray_fd(exe.args, 1);
	return (0);
}
