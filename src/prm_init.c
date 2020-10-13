/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   prm_init.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abenoit <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/10/13 12:05:20 by abenoit           #+#    #+#             */
/*   Updated: 2020/10/13 15:17:14 by abenoit          ###   ########.fr       */
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
