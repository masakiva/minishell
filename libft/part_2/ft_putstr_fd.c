/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_putstr_fd.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mvidal-a <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/11/07 18:06:07 by mvidal-a          #+#    #+#             */
/*   Updated: 2020/06/25 13:38:25 by mvidal-a         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include <unistd.h>

/*
** return value (inherited from write):
** 		nonnegative on success (nb of chars written)
** 		-1 + errno set on error
*/

ssize_t	ft_putstr_fd(const char *s, int fd)
{
	return (write(fd, s, ft_strlen(s)));
}
