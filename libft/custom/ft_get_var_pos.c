/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_get_var_pos.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abenoit <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/10/19 13:44:56 by abenoit           #+#    #+#             */
/*   Updated: 2020/11/23 22:59:32 by mvidal-a         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

ssize_t	get_var_pos(char **array, char *name)
{
	ssize_t		i;
	char	**tmp;

	i = 0;
	while (array[i])
	{
		tmp = ft_split(array[i], '=');
		if (ft_strcmp(tmp[0], name) == 0)
		{
			free_str_array(tmp);
			return (i);
		}
		free_str_array(tmp);
		i++;
	}
	return (-1);
}
