/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_export.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mvidal-a <mvidal-a@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/02/10 14:44:55 by mvidal-a          #+#    #+#             */
/*   Updated: 2021/02/10 14:50:15 by mvidal-a         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "builtins.h"

static int	ft_exported_export(char *var, t_xe *xe)
{
	char	**new_array;

	if (get_var_pos(xe->env, var, ft_strlen(var)) == NOT_FOUND
			&& get_var_pos(xe->exported, var, ft_strlen(var)) == NOT_FOUND)
	{
		new_array = push_str_to_array(xe->exported, ft_strdup(var));
		if (new_array == NULL)
			return (MALLOC_ERR);
		xe->exported = new_array;
	}
	return (SUCCESS);
}

static int	ft_export_checks(char **args, t_xe *xe)
{
	ssize_t	equalsign_pos;

	while (*args != NULL)
	{
		equalsign_pos = ft_index(*args, '=');
		if (check_var_name(*args, equalsign_pos) == SUCCESS)
		{
			if (equalsign_pos != NOT_FOUND)
			{
				if (ft_env_export(*args, xe, equalsign_pos) != SUCCESS)
					return (MALLOC_ERR);
			}
			else if (ft_exported_export(*args, xe) != SUCCESS)
				return (MALLOC_ERR);
		}
		else
		{
			xe->stat_loc = 1;
			ft_error(INVALID_EXP_ID, xe);
		}
		args++;
	}
	return (SUCCESS);
}

int			ft_export(char **args, t_xe *xe)
{
	int		ret;

	if (ft_arraylen(args) == 1)
	{
		ret = print_export(xe->env, xe->exported);
		if (ret != SUCCESS && ret != MALLOC_ERR)
			ret = WRITE_ERR;
	}
	else
	{
		xe->stat_loc = 0;
		ret = ft_export_checks(args + 1, xe);
		if (ret != SUCCESS)
			xe->stat_loc = 1;
	}
	return (ret);
}
