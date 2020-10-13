/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_putnbr_fd.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mvidal-a <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/11/07 18:12:53 by mvidal-a          #+#    #+#             */
/*   Updated: 2020/06/25 13:42:24 by mvidal-a         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include <unistd.h>

ssize_t	ft_putnbr_fd(int n, int fd)
{
	char	digit;

	if (n == -2147483648)
	{
		if (ft_putstr_fd("-2147483648", fd) == 11)
			return (1);
		else
			return (-1);
	}
	else if (n < 0)
	{
		if (ft_putchar_fd('-', fd) != 1)
			return (-1);
		n *= -1;
	}
	if (n > 9)
	{
		if (ft_putnbr_fd(n / 10, fd) != 1)
			return (-1);
	}
	digit = n % 10 + '0';
	return (ft_putchar_fd(digit, fd));
}
