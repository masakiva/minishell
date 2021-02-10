/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_cd.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mvidal-a <mvidal-a@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/02/10 14:40:20 by mvidal-a          #+#    #+#             */
/*   Updated: 2021/02/10 14:40:46 by mvidal-a         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "builtins.h"

int		cd_to(char *newpath, t_xe *xe)
{
	char	*oldpwd;
	char	*full_newpath;
	int		ret;

	oldpwd = get_var_value(xe->env, "PWD", 3);
	if (oldpwd == NULL)
		return (MALLOC_ERR);
	if (chdir(newpath) == ERROR)
	{
		free(oldpwd);
		return (INVALID_CD_PATH);
	}
	ret = env_replace_var("OLDPWD", oldpwd, xe);
	free(oldpwd);
	if (ret != SUCCESS)
		return (MALLOC_ERR);
	full_newpath = getcwd(NULL, 0);
	if (full_newpath == NULL)
		return (INVALID_CD_NEW_PATH);
	ret = env_replace_var("PWD", full_newpath, xe);
	free(full_newpath);
	if (ret != SUCCESS)
		return (MALLOC_ERR);
	return (SUCCESS);
}

int		ft_cd(char **args, t_xe *xe)
{
	int		ret;
	char	*newpath;

	ret = SUCCESS;
	if (args[1] == NULL)
	{
		if (get_var_pos(xe->env, "HOME", 4) == NOT_FOUND)
			ret = HOME_NOT_SET;
		else
			newpath = get_var_value(xe->env, "HOME", 4);
	}
	else if (args[2] != NULL)
		ret = CD_ARG_ERR;
	else
		newpath = ft_strdup(args[1]);
	if (ret == SUCCESS && newpath == NULL)
		ret = MALLOC_ERR;
	else if (ret == SUCCESS)
	{
		ret = cd_to(newpath, xe);
		free(newpath);
	}
	return (ret);
}
