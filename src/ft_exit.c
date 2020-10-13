/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_exit.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abenoit <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/10/12 15:28:37 by abenoit           #+#    #+#             */
/*   Updated: 2020/10/13 13:24:26 by abenoit          ###   ########.fr       */
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

static const char	*err_msg(t_error err_code)
{
	const char	*msg[] = {"",
		"Minishell takes no argument",
		"Memory allocation failure"};

	return (msg[err_code]);
}

int					ft_exit(t_param *prm)
{
	int	i;

	i = 0;
	if (prm->current != NULL)
	{
		while (prm->current[i] != NULL)
		{
			free(prm->current[i]);
			prm->current[i] = NULL;
			i++;
		}
		free(prm->current);
		prm->current = NULL;
	}
	ft_lstclear(&prm->env, &free);
	if (prm->err_code != FALSE)
		return (err_output(err_msg(prm->err_code)));
	exit(0);
}
