/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_echo.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mvidal-a <mvidal-a@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/02/01 11:26:53 by mvidal-a          #+#    #+#             */
/*   Updated: 2021/02/01 13:09:37 by mvidal-a         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "builtins.h"

int		ft_echo(char **args, t_xe *xe)
{
	t_byte	options;

	(void)xe;
	options = shift_options(&args, ECHO_OPTIONS);
	if (*args != NULL)
	{
		if (ft_putstr_fd(*args++, STDOUT_FILENO) != WRITE_SUCCESS)
			return (WRITE_ERR);
	}
	while (*args != NULL)
	{
		if (ft_putchar_fd(' ', STDOUT_FILENO) != SUCCESS)
			return (WRITE_ERR);
		if (ft_putstr_fd(*args, STDOUT_FILENO) != WRITE_SUCCESS)
			return (WRITE_ERR);
		args++;
	}
	if (!(options & ECHO_N_OPTION))
		if (ft_putchar_fd('\n', STDOUT_FILENO) != SUCCESS)
			return (WRITE_ERR);
	return (SUCCESS);
}
