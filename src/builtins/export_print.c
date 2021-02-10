/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export_print.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mvidal-a <mvidal-a@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/02/10 14:45:16 by mvidal-a          #+#    #+#             */
/*   Updated: 2021/02/10 14:46:59 by mvidal-a         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "builtins.h"

static char	**sort_variables(char **env, char **exported)
{
	char		**sorted_array;
	size_t		env_len;
	size_t		exported_len;

	env_len = ft_arraylen(env);
	exported_len = ft_arraylen(exported);
	sorted_array =
		(char **)malloc(sizeof(char *) * (env_len + exported_len + 1));
	if (sorted_array == NULL)
		return (NULL);
	ft_memcpy(sorted_array, env, sizeof(char *) * env_len);
	ft_memcpy(sorted_array + env_len, exported, sizeof(char *) * exported_len);
	sorted_array[env_len + exported_len] = NULL;
	sort_str_array(sorted_array);
	return (sorted_array);
}

static int	print_export_var_value(char *value)
{
	while (*value != '\0')
	{
		if (ft_isset(*value, "$\"\\`") == TRUE)
			if (ft_putchar_fd('\\', STDOUT_FILENO) != SUCCESS)
				return (WRITE_ERR);
		if (ft_putchar_fd(*value, STDOUT_FILENO) != SUCCESS)
			return (WRITE_ERR);
		value++;
	}
	return (SUCCESS);
}

static int	print_export_one_var(char *variable, ssize_t equalsign_pos)
{
	if (write(STDOUT_FILENO, variable, equalsign_pos + 1) != equalsign_pos + 1)
		return (WRITE_ERR);
	if (ft_putchar_fd('"', STDOUT_FILENO) != SUCCESS)
		return (WRITE_ERR);
	if (print_export_var_value(variable + equalsign_pos + 1) != SUCCESS)
		return (WRITE_ERR);
	if (ft_putchar_fd('"', STDOUT_FILENO) != SUCCESS)
		return (WRITE_ERR);
	return (SUCCESS);
}

int			print_export(char **env, char **exported)
{
	char	**variables;
	size_t	i;
	ssize_t	equalsign_pos;
	int		ret;

	variables = sort_variables(env, exported);
	if (variables == NULL)
		return (MALLOC_ERR);
	i = 0;
	while (variables[i] != NULL)
	{
		ret = ft_putstr_fd("declare -x ", STDOUT_FILENO);
		equalsign_pos = ft_index(variables[i], '=');
		if (equalsign_pos != NOT_FOUND && ret == WRITE_SUCCESS)
			ret = print_export_one_var(variables[i], equalsign_pos);
		else if (ret == WRITE_SUCCESS)
			ret = ft_putstr_fd(variables[i], STDOUT_FILENO);
		if (ret == SUCCESS || ret == WRITE_SUCCESS)
			ret = ft_putchar_fd('\n', STDOUT_FILENO);
		if (ret != SUCCESS)
			break ;
		i++;
	}
	free(variables);
	return (ret);
}
