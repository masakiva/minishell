/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_itoa.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mvidal-a <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/11/07 12:58:23 by mvidal-a          #+#    #+#             */
/*   Updated: 2020/01/30 12:28:52 by mvidal-a         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include <stdlib.h>

static size_t	n_len(int n)
{
	size_t	len;

	len = 0;
	if (n == -2147483648)
		n++;
	if (n <= 0)
	{
		n *= -1;
		len++;
	}
	while (n > 0)
	{
		len++;
		n /= 10;
	}
	return (len);
}

char			*ft_itoa(int n)
{
	char		*a;
	unsigned	un;
	size_t		len;

	len = n_len(n);
	if (n < 0)
		un = (unsigned)(n * -1);
	else
		un = (unsigned)n;
	a = (char *)malloc(sizeof(char) * (len + 1));
	if (a != NULL)
	{
		a[len--] = '\0';
		if (n < 0)
			a[0] = '-';
		else if (n == 0)
			a[0] = '0';
		while (un > 0)
		{
			a[len--] = (char)(un % 10 + '0');
			un /= 10;
		}
	}
	return (a);
}
