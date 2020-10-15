/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abenoit <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/09/30 14:53:56 by abenoit           #+#    #+#             */
/*   Updated: 2020/10/15 15:13:54 by abenoit          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include <unistd.h>
#include <stdlib.h>

/*static*/ enum e_command	get_command_code(char *arg)
{
	char	**command;
	int		i;

	i = 0;
	command = ft_split(BUILTINS, "/");
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

static int			get_input(t_all *all)
{
	char	*line;

	get_next_line(all->fd[0], &line);
	if (line == NULL)
		return (MALLOC_ERR);
//	all->current = split_command(line);;
	all->current = ft_split(line, ISSPACE_3);
	free(line);
	//if (all->current == NULL)
	//	return (MALLOC_ERR);
	all->command = get_command_code(all->current[0]);
	return (SUCCESS);
}

static int			main_loop(t_all *all)
{
	int	ret;

	ft_putstr_fd(PROMPT, all->fd[1]); // err
	if ((ret = get_input(all)) != SUCCESS)
		return (ret);
	if ((ret = launch_command(all)) != SUCCESS)
		return (ret);
	return (ret);
}

int					main(int argc, char **argv, char **env)
{
	t_all			all;
	int				ret;

	(void)argv;
	if (argc != 1)
	{
		all.current = NULL;
		all.env = NULL;
		return (ft_exit(ARG_ERR, &all));
	}
	else
	{
		all.current = env;
		if ((ret = all_init(&all)) != SUCCESS)
			return (ft_exit(ret, &all));
	}
	ret = SUCCESS;
	while (ret == SUCCESS)
		ret = main_loop(&all);
	return (ft_exit(ret, &all));
}
