/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   launch_command.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abenoit <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/10/12 16:58:22 by abenoit           #+#    #+#             */
/*   Updated: 2020/10/13 00:44:53 by mvidal-a         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

#include <stdlib.h>
#include <unistd.h>
#define DISABLED	1
#define ENABLED		0

#define SUCCESS		0 // or 1 ?
#define FAILURE		-1 // or 0 ?

// >>> echo
// error management: stop quand un write fail, ou alors on fait un seul write pour tout
// no nl: "This may also be achieved by appending `\c' to the end of the string" (man echo sur mac mais pas linux)

static int	launch_echo(t_param *prm)
{
	int		i;
	int		n_option;

	n_option = DISABLED;
	if (prm->current[1] != NULL)
	{
		n_option = ft_strcmp(prm->current[1], "-n");
		if (n_option != ENABLED)
		{
			ft_putstr(prm->current[1]);
			if (prm->current[2] != NULL)
				ft_putchar(' ');
		}
		if (prm->current[2] != NULL)
			ft_putstr(prm->current[2]);
		i = 3;
		while (prm->current[i] != NULL)
		{
			ft_putchar(' ');
			ft_putstr(prm->current[i]);
			i++;
		}
	}
	if (n_option != ENABLED || prm->current[1] == NULL)
		ft_putchar('\n');
	return (SUCCESS);
}

//# define HOME_DIR	"Users"
//# define USER		"mvidal-a" // "abenoit"
static int	launch_cd(t_param *prm)
{
//	if (prm->current[1] == NULL)
//	{
//		errno = 0;
//		if (chdir("/HOME_DIR/USER") != SUCCESS)
//		{
//			perror("chdir");
//			return (FAILURE);
//		}
//	}
	(void)prm;
	ft_putstr("cd");
	return (0);
}

static int	launch_pwd(t_param *prm)
{
	char	*buf;

	(void)prm;
	buf = getcwd(NULL, 0);
	if (buf == NULL)
	{
		//free(buf);
		return (FAILURE);
	}
	ft_putstr(buf);
	ft_putchar('\n');
	free(buf);
	return (SUCCESS);
}

static int	launch_export(t_param *prm)
{
	(void)prm;
	ft_putstr("export");
	return (0);
}

static int	launch_unset(t_param *prm)
{
	(void)prm;
	ft_putstr("unset");
	return (0);
}

static int	launch_env(t_param *prm)
{
	(void)prm;
	ft_putstr("env");
	return (0);
}

static int	launch_exit(t_param *prm)
{
	(void)prm;
	ft_putstr("exit");
	return (0);
}

static int	launch_else(t_param *prm)
{
	(void)prm;
	ft_putstr("else");
	return (0);
}

int		launch_command(t_param *prm)
{
	const t_func	command[8] = {launch_echo, launch_cd, launch_pwd,
								launch_export, launch_unset, launch_env,
								launch_exit, launch_else};

	if (prm->command != ECHO && prm->command != PWD)
		ft_putstr("Command = ");
	if (command[prm->command](prm) == FAILURE)
		ft_putstr("ERROR\n");;
	if (prm->command != ECHO && prm->command != PWD)
		ft_putstr("\n");
	prm->state = GET_INPUT;
	return (0);
}
