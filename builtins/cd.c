/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abenoit <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/10/19 12:17:28 by abenoit           #+#    #+#             */
/*   Updated: 2020/10/19 14:11:27 by abenoit          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>

static int	launch_pwd(void)
{
	char	*buf;

	buf = getcwd(NULL, 0);
	if (buf == NULL)
	{
		free(buf);
		return (ERROR);
	}
	ft_putstr_fd(buf, 1);
	ft_putchar_fd('\n', 1);
	free(buf);
	return (SUCCESS);
}

int		main(int ac, char **av, char **env)
{
	char	*path;

	if (ac == 1)
		path = get_var_content(env, "HOME");
	else
		path = av[1];
	if (chdir(path) != SUCCESS)
	{
		perror("cd: ");
		return (-1);
	}
	launch_pwd();
	return (0);
}
