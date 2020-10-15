/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   launch_command.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abenoit <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/10/12 16:58:22 by abenoit           #+#    #+#             */
/*   Updated: 2020/10/15 14:13:23 by abenoit          ###   ########.fr       */
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
static int	launch_echo(t_all *all)
{
	int		i;
	int		n_option;

	n_option = DISABLED;
	if (all->current[1] != NULL)
	{
		n_option = ft_strcmp(all->current[1], "-n");
		if (n_option != ENABLED)
		{
			ft_putstr_fd(all->current[1], all->fd[1]);
			if (all->current[2] != NULL)
				ft_putchar_fd(' ', all->fd[1]);
		}
		if (all->current[2] != NULL)
			ft_putstr_fd(all->current[2], all->fd[1]);
		i = 3;
		while (all->current[i] != NULL)
		{
			ft_putchar_fd(' ', all->fd[1]);
			ft_putstr_fd(all->current[i], all->fd[1]);
			i++;
		}
	}
	if (n_option != ENABLED || all->current[1] == NULL)
		ft_putchar_fd('\n', all->fd[1]);
	return (SUCCESS);
}

//# define HOME_DIR	"Users"
//# define USER		"mvidal-a" // "abenoit"
static int	launch_cd(t_all *all)
{
//	if (all->current[1] == NULL)
//	{
//		errno = 0;
//		if (chdir("/HOME_DIR/USER") != SUCCESS)
//		{
//			perror("chdir");
//			return (FAILURE);
//		}
//	}
	(void)all;
	ft_putstr_fd("command: cd\n", all->fd[1]);
	return (SUCCESS);
}

static int	launch_pwd(t_all *all)
{
	char	*buf;

	(void)all;
	buf = getcwd(NULL, 0);
	if (buf == NULL)
	{
		//free(buf);
		return (FAILURE);
	}
	ft_putstr_fd(buf, all->fd[1]);
	ft_putchar_fd('\n', all->fd[1]);
	free(buf);
	return (SUCCESS);
}

static int	launch_export(t_all *all)
{
	(void)all;
	ft_putstr_fd("command: export\n", all->fd[1]);
	return (SUCCESS);
}

static int	launch_unset(t_all *all)
{
	(void)all;
	ft_putstr_fd("command: unset\n", all->fd[1]);
	return (SUCCESS);
}

static int	launch_env(t_all *all)
{
	t_list	*ptr;
	char	*str;

	ptr = all->env;
	while (ptr != NULL)
	{
		str = ptr->content;
		ft_putstr_fd(str, all->fd[1]);
		ft_putstr_fd("\n", all->fd[1]);
		ptr = ptr->next;
	}
	return (SUCCESS);
}

static int	launch_exit(t_all *all)
{
	(void)all;
	return (CLEAN_EXIT);
}

static int	launch_ext(t_all *all)
{
	(void)all;
	ft_putstr_fd("command: ext\n", all->fd[1]);
	return (SUCCESS);
}

int		launch_command(t_all *all)
{
	int				i;
	int				ret;
	const t_func	command[8] = {launch_echo, launch_cd, launch_pwd,
								launch_export, launch_unset, launch_env,
								launch_exit, launch_ext};

	i = 0;
	if ((ret = command[all->command](all)) == FAILURE)
		ft_putstr_fd("ERROR\n", all->fd[1]);
	free_str_array(&all->current);
	return (ret);
}
