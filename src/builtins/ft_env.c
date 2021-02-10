/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_env.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mvidal-a <mvidal-a@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/02/10 14:40:51 by mvidal-a          #+#    #+#             */
/*   Updated: 2021/02/10 14:40:52 by mvidal-a         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "builtins.h"

int		ft_env(char **args, t_xe *xe)
{
	(void)args;
	if (ft_printarray_fd(xe->env, STDOUT_FILENO) != SUCCESS)
		return (WRITE_ERR);
	return (SUCCESS);
}
