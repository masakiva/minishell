/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strlcat.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mvidal-a <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/11/05 13:52:19 by mvidal-a          #+#    #+#             */
/*   Updated: 2020/01/29 16:26:39 by mvidal-a         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

size_t	ft_strlcat(char *dst, const char *src, size_t dstsize)
{
	size_t	orig_dstlen;
	size_t	total_len;
	size_t	i;
	size_t	j;

	total_len = ft_strlen(src);
	if (dstsize != 0)
	{
		orig_dstlen = ft_strlen(dst);
		dstsize -= 1;
		i = orig_dstlen;
		j = 0;
		while (i < dstsize && src[j])
			dst[i++] = src[j++];
		if (dstsize + 1 > orig_dstlen)
		{
			dst[i] = '\0';
			total_len += orig_dstlen;
		}
		else
			total_len += dstsize + 1;
	}
	return (total_len);
}
