/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_var_pos.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abenoit <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/10/19 13:44:56 by abenoit           #+#    #+#             */
/*   Updated: 2020/12/26 22:37:11 by mvidal-a         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

ssize_t		get_var_pos(char **vars, char *name)
{
	size_t		i;
	ssize_t		name_len;

	if (vars != NULL)
	{
		i = 0;
		name_len = ft_index(name, '=');
		if (name_len == -1)
			name_len = ft_strlen(name);
		while (vars[i] != NULL)
		{
			if (ft_strncmp(vars[i], name, name_len) == 0
					&& (vars[i][name_len] == '=' || vars[i][name_len] == '\0'))
				return (i);
			i++;
		}
	}
	return (-1);
}
