#include "execution.h"

int		check_var_name(char *var, ssize_t name_len)
{
	size_t	i;

	if (!ft_isdigit(var[0]) && var[0] != '=' && var[0] != '+' && var[0] != '\0')
	{
		i = 0;
		while (ft_isalnum(var[i]) || var[i] == '_')
			i++;
		if ((name_len == ENTIRE_STR && var[i] == '\0')
				|| i == (size_t)name_len
				|| (i == (size_t)name_len - 1 && var[i] == '+'))
			return (SUCCESS);
	}
	return (FAILURE);
}

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

static int	ft_env_move_from_exp(char **args, t_xe *xe, ssize_t equalsign_pos, size_t i)
{
	ssize_t	var_pos;
	char	**new_array;
	char	*new_var;

	var_pos = get_var_pos(xe->exported, args[i], equalsign_pos - 1);
	if (var_pos != NOT_FOUND)
	{
		new_array = rm_str_from_array(xe->exported, (size_t)var_pos);
		if (new_array == NULL)
			return (MALLOC_ERR);
		xe->exported = new_array;
	}
	new_var = (char *)malloc(sizeof(char) * ft_strlen(args[i]));
	if (new_var == NULL)
		return (MALLOC_ERR);
	ft_memcpy(new_var, args[i], equalsign_pos - 1);
	ft_strlcpy(new_var + equalsign_pos - 1, args[i] + equalsign_pos, ft_strlen(args[i]));
	new_array = push_str_to_array(xe->env, new_var);
	if (new_array == NULL)
		return (MALLOC_ERR);
	free(new_var);
	xe->env = new_array;
	return (SUCCESS);
}

static int	ft_env_append(char **args, t_xe *xe, ssize_t equalsign_pos, size_t i)
{
	ssize_t	var_pos;
	char	*new_var;

	var_pos = get_var_pos(xe->env, args[i], equalsign_pos - 1);
	if (var_pos != NOT_FOUND)
	{
		new_var = ft_strjoin(xe->env[var_pos], args[i] + equalsign_pos + 1); // ft_strjoin_free?
		if (new_var == NULL)
			return (MALLOC_ERR);
		free(xe->env[var_pos]);
		xe->env[var_pos] = new_var;
	}
	else
	{
		ft_env_move_from_exp(args, xe, equalsign_pos, i);
	}
	return (SUCCESS);
}

static int	ft_env_set(char **args, t_xe *xe, ssize_t equalsign_pos, size_t i)
{
	char	**new_array;
	ssize_t	var_pos;

	var_pos = get_var_pos(xe->env, args[i], equalsign_pos);
	if (var_pos != NOT_FOUND)
	{
		free(xe->env[var_pos]);
		xe->env[var_pos] = ft_strdup(args[i]);
		if (xe->env[var_pos] == NULL)
			return (MALLOC_ERR);
	}
	else
	{
		var_pos = get_var_pos(xe->exported, args[i], equalsign_pos);
		if (var_pos != NOT_FOUND)
		{
			new_array = rm_str_from_array(xe->exported, (size_t)var_pos);
			if (new_array == NULL)
				return (MALLOC_ERR);
			xe->exported = new_array;
		}
		new_array = push_str_to_array(xe->env, args[i]);
		if (new_array == NULL)
			return (MALLOC_ERR);
		xe->env = new_array;
	}
	return (SUCCESS);
}

static int	ft_env_export(char **args, t_xe *xe, ssize_t equalsign_pos, size_t i)
{
	if (args[i][equalsign_pos - 1] == '+')
		ft_env_append(args, xe, equalsign_pos, i);
	else
		ft_env_set(args, xe, equalsign_pos, i);
	return (SUCCESS);
}

static int	ft_exported_export(char **args, t_xe *xe, size_t i)
{
	char	**new_array;

	if (get_var_pos(xe->env, args[i], ft_strlen(args[i])) == NOT_FOUND
			&& get_var_pos(xe->exported, args[i], ft_strlen(args[i])) == NOT_FOUND)
	{
		new_array = push_str_to_array(xe->exported, args[i]);
		if (new_array == NULL)
			return (MALLOC_ERR);
		xe->exported = new_array;
	}
	return (SUCCESS);
}

int		ft_export(char **args, t_xe *xe)
{
	size_t	nb_args;
	size_t	i;
	ssize_t	equalsign_pos;

	nb_args = ft_arraylen(args);
	if (nb_args == 1)
		print_export(xe->env, xe->exported);
	i = 1;
	while (i < nb_args)
	{
		equalsign_pos = ft_index(args[i], '=');
		if (check_var_name(args[i], equalsign_pos) == SUCCESS)
		{
			if (equalsign_pos != NOT_FOUND)
				ft_env_export(args, xe, equalsign_pos, i);
			else
				ft_exported_export(args, xe, i);
		}
		else
			putstr_stderr("export: Variable identifier (name) invalid\n"); // leaks si le write marche pas
		i++;
	}
	return (SUCCESS);
}

int		ft_unset(char **args, t_xe *xe)
{
	size_t	nb_args;
	ssize_t	var_pos;
	char	**new_array;
	size_t	i;

	nb_args = ft_arraylen(args);
	i = 1;
	while (i < nb_args)
	{
		if (check_var_name(args[i], ENTIRE_STR) == SUCCESS)
		{
			var_pos = get_var_pos(xe->env, args[i], ft_strlen(args[i]));
			if (var_pos != NOT_FOUND)
			{
				new_array = rm_str_from_array(xe->env, (size_t)var_pos);
				if (new_array == NULL)
					return (MALLOC_ERR);
				xe->env = new_array;
			}
			else
			{
				var_pos = get_var_pos(xe->exported, args[i], ft_strlen(args[i]));
				if (var_pos != NOT_FOUND)
				{
					new_array = rm_str_from_array(xe->exported, (size_t)var_pos);
					if (new_array == NULL)
						return (MALLOC_ERR);
					xe->exported = new_array;
				}
			}
		}
		else
			putstr_stderr("unset: Variable identifier (name) invalid\n"); // leaks si le write marche pas
		i++;
	}
	return (SUCCESS);
}

int		ft_env(char **args, t_xe *xe)
{
	(void)args;
	ft_printarray_fd(xe->env, STDOUT_FILENO); // change fd for pipes
	return (SUCCESS);
}
