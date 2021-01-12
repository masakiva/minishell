/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_var_value.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abenoit <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/10/19 13:42:49 by abenoit           #+#    #+#             */
/*   Updated: 2021/01/12 10:54:08 by abenoit          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*get_var_value(char **vars, char *name, size_t name_len)
{
	if (vars != NULL && name_len != 0)
	{
		while (*vars != NULL)
		{
			if ((size_t)ft_index(*vars, '=') == name_len
					&& ft_strncmp(*vars, name, name_len) == 0)
				return (ft_strdup(*vars + name_len + 1));
			vars++;
		}
	}
	return (NULL);
}
