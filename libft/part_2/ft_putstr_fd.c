/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_putstr_fd.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mvidal-a <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/11/07 18:06:07 by mvidal-a          #+#    #+#             */
/*   Updated: 2021/01/28 17:14:12 by mvidal-a         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include <unistd.h>

/*
** return value:
** 		-2 on success (the total nb of bytes requested have been written)
**		nonnegative if the nb of bytes written is smaller than requested
** 		-1 on error
*/

ssize_t		ft_putstr_fd(const char *s, int fd)
{
	size_t	len;
	ssize_t	ret;
	
	len = ft_strlen(s);
	ret = write(fd, s, len);
	if (ret == (ssize_t)len)
		return (WRITE_SUCCESS);
	else
		return (ret);
}
