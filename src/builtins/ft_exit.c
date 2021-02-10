/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_exit.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mvidal-a <mvidal-a@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/02/10 16:41:31 by mvidal-a          #+#    #+#             */
/*   Updated: 2021/02/10 16:41:31 by mvidal-a         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "builtins.h"

int		ft_exit(char **args, t_xe *xe)
{
	if (xe->flags & CMD_PIPE || xe->flags & CHILD)
		xe->flags += EXIT_ABORT;
	else
	{
		xe->flags -= RUN;
		xe->flags += EXIT_FLAG;
	}
	if (args[1] != NULL)
	{
		if (ft_isnumber(args[1]) == TRUE)
		{
			if (args[2] != NULL)
			{
				xe->stat_loc = 1;
				return (EXIT_ARG_ERR);
			}
			xe->stat_loc = (unsigned int)ft_atoi(args[1]) % 256;
		}
		else
		{
			xe->stat_loc = 2;
			return (EXIT_NAN);
		}
	}
	return (SUCCESS);
}
