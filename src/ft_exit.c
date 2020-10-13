/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_exit.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abenoit <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/10/12 15:28:37 by abenoit           #+#    #+#             */
/*   Updated: 2020/10/13 15:45:28 by abenoit          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include <stdlib.h>

static int			err_output(const char *str)
{
	ft_putstr("Error: ");
	ft_putstr(str);
	ft_putstr("\n");
	exit (-1);
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
	exit(0);
}
