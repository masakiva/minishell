/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abenoit <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/09/30 14:53:56 by abenoit           #+#    #+#             */
/*   Updated: 2020/10/13 12:45:20 by abenoit          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include <unistd.h>
#include <stdlib.h>

static t_command	get_command_code(char *arg)
{
	char	**command;
	int		i;

	i = 0;
	command = ft_split(BUILTINS, "/");
	while (command[i] != NULL)
	{
		if (ft_strcmp(arg, command[i]) == 0)
			return (i);
		i++;
	}
	return (ELSE);
}

static int			get_input(t_param *prm)
{
	char	*line;

	ft_putstr("$ ");
	rec_gnl(0, &line);
	prm->current = ft_split(line, ISSPACE_3);
	if (prm->current == NULL)
	{
		prm->err_code = MALLOC_ERR;
		prm->state = FT_EXIT;
	}
	prm->command = get_command_code(prm->current[0]);
	prm->state = LAUNCH;
	free(line);
	return (0);
}

int					main(int ac, char **av, char **env)
{
	t_param			prm;
	const t_func	fsm[4] = {prm_init, get_input, launch_command, ft_exit};

	(void)av;
	(void)env;
	if (ac != 1)
	{
		prm.err_code = ARG_ERR;
		prm.current = NULL;
		return (ft_exit(&prm));
	}
	else
	{
		prm.current = env;
		prm.state = PRM_INIT;
	}
	while (prm.state != FT_EXIT)
		fsm[prm.state](&prm);
	return (ft_exit(&prm));
}
