/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export_in_env.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mvidal-a <mvidal-a@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/02/10 14:48:58 by mvidal-a          #+#    #+#             */
/*   Updated: 2021/02/10 14:49:11 by mvidal-a         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "builtins.h"

static int	ft_env_append_from_exp(char *var, t_xe *xe, ssize_t equalsign_pos)
{
	ssize_t	var_pos;
	char	**new_array;
	char	*new_var;

	var_pos = get_var_pos(xe->exported, var, equalsign_pos - 1);
	if (var_pos != NOT_FOUND)
	{
		new_array = rm_str_from_array(xe->exported, (size_t)var_pos);
		if (new_array == NULL)
			return (MALLOC_ERR);
		xe->exported = new_array;
	}
	new_var = (char *)malloc(sizeof(char) * ft_strlen(var));
	if (new_var == NULL)
		return (MALLOC_ERR);
	ft_memcpy(new_var, var, equalsign_pos - 1);
	ft_strlcpy(new_var + equalsign_pos - 1,
				var + equalsign_pos, ft_strlen(var));
	new_array = push_str_to_array(xe->env, ft_strdup(new_var));
	if (new_array == NULL)
		return (MALLOC_ERR);
	free(new_var);
	xe->env = new_array;
	return (SUCCESS);
}

static int	ft_env_append(char *var, t_xe *xe, ssize_t equalsign_pos)
{
	ssize_t	var_pos;
	char	*new_var;

	var_pos = get_var_pos(xe->env, var, equalsign_pos - 1);
	if (var_pos != NOT_FOUND)
	{
		new_var = strjoin_free(xe->env[var_pos], var + equalsign_pos + 1);
		if (new_var == NULL)
			return (MALLOC_ERR);
		xe->env[var_pos] = new_var;
	}
	else
	{
		if (ft_env_append_from_exp(var, xe, equalsign_pos) != SUCCESS)
			return (MALLOC_ERR);
	}
	return (SUCCESS);
}

static int	ft_env_set_from_exp(char *var, t_xe *xe, ssize_t equalsign_pos)
{
	char	**new_array;
	ssize_t	var_pos;

	var_pos = get_var_pos(xe->exported, var, equalsign_pos);
	if (var_pos != NOT_FOUND)
	{
		new_array = rm_str_from_array(xe->exported, (size_t)var_pos);
		if (new_array == NULL)
			return (MALLOC_ERR);
		xe->exported = new_array;
	}
	new_array = push_str_to_array(xe->env, ft_strdup(var));
	if (new_array == NULL)
		return (MALLOC_ERR);
	xe->env = new_array;
	return (SUCCESS);
}

static int	ft_env_set(char *var, t_xe *xe, ssize_t equalsign_pos)
{
	ssize_t	var_pos;

	var_pos = get_var_pos(xe->env, var, equalsign_pos);
	if (var_pos != NOT_FOUND)
	{
		free(xe->env[var_pos]);
		xe->env[var_pos] = ft_strdup(var);
		if (xe->env[var_pos] == NULL)
			return (MALLOC_ERR);
	}
	else
	{
		if (ft_env_set_from_exp(var, xe, equalsign_pos) != SUCCESS)
			return (MALLOC_ERR);
	}
	return (SUCCESS);
}

int			ft_env_export(char *var, t_xe *xe, ssize_t equalsign_pos)
{
	if (var[equalsign_pos - 1] == '+')
	{
		if (ft_env_append(var, xe, equalsign_pos) != SUCCESS)
			return (MALLOC_ERR);
	}
	else
	{
		if (ft_env_set(var, xe, equalsign_pos) != SUCCESS)
			return (MALLOC_ERR);
	}
	return (SUCCESS);
}
