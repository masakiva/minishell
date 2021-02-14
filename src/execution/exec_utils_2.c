/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_utils_2.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abenoit <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/02/14 22:03:15 by abenoit           #+#    #+#             */
/*   Updated: 2021/02/14 22:05:46 by abenoit          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parsing.h"
#include "execution.h"

int		handle_exec_return(t_xe *xe, int fd_in, int proc)
{
	if (!(xe->flags & INTERUPT))
		return (handle_execution(xe, fd_in, proc));
	else
	{
		xe->flags = RUN;
		return (SUCCESS);
	}
}
