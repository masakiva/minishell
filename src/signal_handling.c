/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signal_handling.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abenoit <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/02/11 18:16:50 by abenoit           #+#    #+#             */
/*   Updated: 2021/02/11 18:17:04 by abenoit          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	sig_int(int signum)
{
	(void)signum;
	ft_putstr_fd("\n", STDIN_FILENO);
	if (ft_putstr_fd(FT_PS1, STDOUT_FILENO) == ERROR)
		return ;
}

void	sig_kill(int signum)
{
	(void)signum;
	ft_putstr_fd("\b\b  \b\b", STDIN_FILENO);
}

void	signal_handler(void)
{
	signal(SIGINT, sig_int);
	signal(SIGQUIT, sig_kill);
}
