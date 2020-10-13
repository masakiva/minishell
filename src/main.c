/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abenoit <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/09/30 14:53:56 by abenoit           #+#    #+#             */
/*   Updated: 2020/10/13 18:48:35 by mvidal-a         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include <unistd.h>
#include <stdlib.h>

static enum e_command	get_command_code(char *arg)
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

static int			get_input(t_param *prm)
{
	char	*line;

	rec_gnl(STDIN_FILENO, &line);
	if (line == NULL)
		return (MALLOC_ERR);
	//prm->current = split_command(line);;
	prm->current = ft_split(line, ISSPACE_3);
	free(line);
	if (prm->current == NULL)
		return (MALLOC_ERR);
	prm->command = get_command_code(prm->current[0]);
	return (0);
}

static int			main_loop(t_param *prm)
{
	int	ret;

	ft_putstr(PROMPT); // err
	if ((ret = get_input(prm)) != SUCCESS)
		return (ret);
	if ((ret = launch_command(prm)) != SUCCESS)
		return (ret);
	return (ret);
}

int					main(int argc, char **argv, char **env)
{
	t_param			prm;
	int				ret;

	(void)argv;
	if (argc != 1)
	{
		prm.current = NULL;
		prm.env = NULL;
		return (ft_exit(ARG_ERR, &prm));
	}
	else
	{
		prm.current = env;
		if ((ret = prm_init(&prm)) != SUCCESS)
			return (ft_exit(ret, &prm));
	}
	ret = 0;
	while (ret == SUCCESS)
		ret = main_loop(&prm);
	return (ft_exit(ret, &prm));
}
