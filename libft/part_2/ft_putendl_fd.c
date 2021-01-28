/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_putendl_fd.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mvidal-a <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/11/07 18:10:47 by mvidal-a          #+#    #+#             */
/*   Updated: 2021/01/28 17:22:24 by mvidal-a         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

/*
** return value:
** 		-2 on success (the total nb of bytes requested have been written)
**		nonnegative if the nb of bytes written is smaller than requested
** 		-1 on error
*/

ssize_t	ft_putendl_fd(const char *s, int fd)
{
	ssize_t	ret_str;
	ssize_t	ret_nl;

	ret_str = ft_putstr_fd(s, fd);
	if (ret_str == WRITE_SUCCESS)
	{
		ret_nl = ft_putchar_fd('\n', fd);
		if (ret_nl == -1)
			ret_str = ERROR;
		else if (ret_nl == 0)
			ret_str = ft_strlen(s);
	}
	return (ret_str);
}
