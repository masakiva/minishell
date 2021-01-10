/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_varnamelen.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mvidal-a <mvidal-a@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/01/10 14:40:52 by mvidal-a          #+#    #+#             */
/*   Updated: 2021/01/10 15:20:16 by mvidal-a         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

size_t	ft_varnamelen(char *str)
{
	size_t		len;

	if (ft_isdigit(*str))
		len = 1;
	else
	{
		len = 0;
		while (ft_isalnum(*str) || *str == '_')
		{
			len++;
			str++;
		}
	}
	return (len);
}
