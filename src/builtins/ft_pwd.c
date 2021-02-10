/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_pwd.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mvidal-a <mvidal-a@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/02/10 14:41:06 by mvidal-a          #+#    #+#             */
/*   Updated: 2021/02/10 14:41:07 by mvidal-a         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "builtins.h"

int		ft_pwd(char **args, t_xe *xe)
{
	char	*buf;
	int		ret;

	(void)args;
	(void)xe;
	ret = SUCCESS;
	buf = getcwd(NULL, 0);
	if (buf != NULL)
	{
		if (ft_putendl_fd(buf, STDOUT_FILENO) != WRITE_SUCCESS)
			ret = WRITE_ERR;
		free(buf);
	}
	else
		ret = INVALID_PWD_PATH;
	return (ret);
}
