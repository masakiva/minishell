#include "execution.h"

char	**sort_variables(char **env, char **exported)
{
	char		**sorted_array;
	size_t		env_len;
	size_t		exported_len;

	env_len = ft_arraylen(env);
	exported_len = ft_arraylen(exported);
	sorted_array = (char **)malloc(sizeof(char *) * (env_len + exported_len + 1));
	if (sorted_array == NULL)
		return (NULL);
	ft_memcpy(sorted_array, env, sizeof(char *) * env_len);
	ft_memcpy(sorted_array + env_len, exported, sizeof(char *) * exported_len);
	sorted_array[env_len + exported_len] = NULL;
	sort_str_array(sorted_array);
	return (sorted_array);
}

int		print_export_var_value(char *value)
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

int		print_export_one_var(char *variable, ssize_t equalsign_pos)
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

int		print_export(char **env, char **exported)
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
		if (ret == SUCCESS || ret == WRITE_SUCCESS)
			ret = ft_putstr_fd(variables[i], STDOUT_FILENO);
		if (ret == WRITE_SUCCESS)
			ret = ft_putchar_fd('\n', STDOUT_FILENO);
		if (ret != SUCCESS)
			break ;
		i++;
	}
	free(variables);
	return (ret);
}

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
		new_var = ft_strjoin(xe->env[var_pos], var + equalsign_pos + 1); // ft_strjoin_free?
		if (new_var == NULL)
			return (MALLOC_ERR);
		free(xe->env[var_pos]);
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

static int	ft_env_export(char *var, t_xe *xe, ssize_t equalsign_pos)
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
			else
			{
				if (ft_exported_export(*args, xe) != SUCCESS)
					return (MALLOC_ERR);
			}
		}
		else
			printf("export: Variable identifier (name) invalid\n"); // ERROR CODE
		args++;
	}
	return (SUCCESS);
}

int		ft_export(char **args, t_xe *xe)
{
	int		ret;

	if (ft_arraylen(args) == 1)
		ret = print_export(xe->env, xe->exported);
	else
		ret = ft_export_checks(args + 1, xe);
	xe->stat_loc = 0;
	return (ret);
}
