/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_exit.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abenoit <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/10/12 15:28:37 by abenoit           #+#    #+#             */
/*   Updated: 2020/10/15 12:58:54 by abenoit          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include <stdlib.h>
#include <unistd.h>

static int			err_output(const char *str)
{
	ft_putstr_fd("Error: ", prm->fd[2]);
	ft_putstr_fd(str, prm->fd[2]);
	ft_putstr_fd("\n", prm->fd[2]);
	exit (EXIT_FAILURE);
}

static const char	*err_msg(int err_code)
{
	const char	*msg[] = {"",
		"Minishell takes no argument",
		"Memory allocation failure"};

	return (msg[err_code]);
}

int					ft_exit(int err_code, t_param *prm)
{
	free_str_array(&prm->current);
	ft_lstclear(&prm->env, &free);
	if (err_code != CLEAN_EXIT)
		return (err_output(err_msg(err_code)));
	exit(EXIT_SUCCESS);
}
