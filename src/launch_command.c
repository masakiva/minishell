/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   launch_command.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abenoit <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/10/12 16:58:22 by abenoit           #+#    #+#             */
/*   Updated: 2020/10/12 18:47:06 by abenoit          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	launch_echo(t_param *prm)
{
	(void)prm;
	ft_putstr("echo");
	return (0);
}

static int	launch_cd(t_param *prm)
{
	(void)prm;
	ft_putstr("cd");
	return (0);
}

static int	launch_pwd(t_param *prm)
{
	(void)prm;
	ft_putstr("pwd");
	return (0);
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

	ft_putstr("Command = ");
	command[prm->command](prm);
	ft_putstr("\n");
	prm->state = GET_INPUT;
	return (0);
}
