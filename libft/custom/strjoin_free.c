/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   strjoin_free.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mvidal-a <mvidal-a@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/01/09 18:31:27 by mvidal-a          #+#    #+#             */
/*   Updated: 2021/01/10 00:34:08 by mvidal-a         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include "libft.h"

char	*strjoin_free(char *s1, char const *s2)
{
	char	*join;

	join = ft_strjoin(s1, s2);
	free(s1);
	return (join);
}
