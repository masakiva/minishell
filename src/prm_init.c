/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   prm_init.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abenoit <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/10/13 12:05:20 by abenoit           #+#    #+#             */
/*   Updated: 2020/10/15 12:55:21 by abenoit          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <unistd.h>
#include "minishell.h"

static int	extract_env(t_param *prm)
{
	int		i;
	char	*buff;

	i = 0;
	prm->env = NULL;
	prm->fd[0] = STDIN_FILENO;
	prm->fd[1] = STDOUT_FILENO;
	prm->fd[2] = STDERR_FILENO;
	if (prm->current == NULL)
		return (-1);
	while ((prm->current)[i])
	{
		if ((buff = ft_strdup((prm->current)[i])) == NULL)
			return (MALLOC_ERR);
		ft_lstadd_back(&prm->env, ft_lstnew(buff));
		i++;
	}
	prm->current = NULL;
	return (SUCCESS);
}

int			prm_init(t_param *prm)
{
	return (extract_env(prm));
}
