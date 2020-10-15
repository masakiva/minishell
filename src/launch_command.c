/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   launch_command.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abenoit <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/10/12 16:58:22 by abenoit           #+#    #+#             */
/*   Updated: 2020/10/15 12:57:29 by abenoit          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

#include <stdlib.h>
#include <unistd.h>
#define DISABLED	1
#define ENABLED		0

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
			ft_putstr_fd(prm->current[1], prm->fd[1]);
			if (prm->current[2] != NULL)
				ft_putchar_fd(' ', prm->fd[1]);
		}
		if (prm->current[2] != NULL)
			ft_putstr_fd(prm->current[2], prm->fd[1]);
		i = 3;
		while (prm->current[i] != NULL)
		{
			ft_putchar_fd(' ', prm->fd[1]);
			ft_putstr_fd(prm->current[i], prm->fd[1]);
			i++;
		}
	}
	if (n_option != ENABLED || prm->current[1] == NULL)
		ft_putchar_fd('\n', prm->fd[1]);
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
	ft_putstr_fd("command: cd\n", prm->fd[1]);
	return (SUCCESS);
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
	ft_putstr_fd(buf, prm->fd[1]);
	ft_putchar_fd('\n', prm->fd[1]);
	free(buf);
	return (SUCCESS);
}

static int	launch_export(t_param *prm)
{
	(void)prm;
	ft_putstr_fd("command: export\n", prm->fd[1]);
	return (SUCCESS);
}

static int	launch_unset(t_param *prm)
{
	(void)prm;
	ft_putstr_fd("command: unset\n", prm->fd[1]);
	return (SUCCESS);
}

static int	launch_env(t_param *prm)
{
	ft_lstiter(prm->env, &ft_lstprint);
	return (SUCCESS);
}

static int	launch_exit(t_param *prm)
{
	(void)prm;
	return (CLEAN_EXIT);
}

static int	launch_ext(t_param *prm)
{
	(void)prm;
	ft_putstr_fd("command: ext\n", prm->fd[1]);
	return (SUCCESS);
}

int		launch_command(t_param *prm)
{
	int				i;
	int				ret;
	const t_func	command[8] = {launch_echo, launch_cd, launch_pwd,
								launch_export, launch_unset, launch_env,
								launch_exit, launch_ext};

	i = 0;
	if ((ret = command[prm->command](prm)) == FAILURE)
		ft_putstr_fd("ERROR\n", prm->fd[1]);
	free_str_array(&prm->current);
	return (ret);
}
