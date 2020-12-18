/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_var_pos.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abenoit <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/10/19 13:44:56 by abenoit           #+#    #+#             */
/*   Updated: 2020/12/18 16:29:00 by mvidal-a         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

ssize_t		get_var_pos(char **env, char *name)
{
	size_t		i;
	size_t		name_len;

	i = 0;
	while (env[i] != NULL)
	{
		name_len = ft_strlen(name);
		if (ft_strncmp(env[i], name, name_len) == 0
				&& env[i][name_len] == '=')
			return (i);
		i++;
	}
	return (-1);
}
