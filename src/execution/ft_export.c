#include "execution.h"

void	print_export(char **env, char **exported)
{
	size_t	equalsign_pos;

	while (*env != NULL)
	{
		equalsign_pos = ft_index(*env, '=');
		ft_putstr_fd("export ", STDOUT_FILENO);// change fd
		write(STDOUT_FILENO, *env, equalsign_pos + 1);// idem
		ft_putchar_fd('"', STDOUT_FILENO); // idem
		ft_putstr_fd(*env + equalsign_pos + 1, STDOUT_FILENO);// id
		ft_putchar_fd('"', STDOUT_FILENO); // id
		ft_putchar_fd('\n', STDOUT_FILENO); // id
		env++;
	}
	if (exported != NULL)
		while (*exported != NULL)
		{
			ft_putstr_fd("export ", STDOUT_FILENO);// change fd
			ft_putstr_fd(*exported, STDOUT_FILENO); //idem
			ft_putchar_fd('\n', STDOUT_FILENO); // idem
			exported++;
		}
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
	new_array = push_str_to_array(xe->env, new_var);
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
	new_array = push_str_to_array(xe->env, var);
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
		new_array = push_str_to_array(xe->exported, var);
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
			putstr_stderr("export: Variable identifier (name) invalid\n"); // leaks si le write marche pas
		args++;
	}
	return (SUCCESS);
}

int		ft_export(char **args, t_xe *xe)
{
	if (ft_arraylen(args) == 1)
		print_export(xe->env, xe->exported);
	else
	{
		if (ft_export_checks(args + 1, xe) != SUCCESS)
			return (MALLOC_ERR);
	}
	return (SUCCESS);
}
