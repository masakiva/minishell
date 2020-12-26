#include "execution.h"

int		check_var_name(char *var, ssize_t name_len)
{
	size_t	i;

	if (!ft_isdigit(var[0]) && var[0] != '=' && var[0] != '\0')
	{
		i = 0;
		while (ft_isalnum(var[i]) || var[i] == '_')
			i++;
		if ((name_len == ENTIRE_STR && var[i] == '\0') || i == (size_t)name_len)
			return (SUCCESS);
	}
	return (FAILURE);
}

void	print_export(char **env, char **exported)
{
	size_t	equal_pos;

	while (*env != NULL)
	{
		equal_pos = ft_index(*env, '=');
		ft_putstr_fd("export ", STDOUT_FILENO);// change fd
		write(STDOUT_FILENO, *env, equal_pos + 1);// idem
		ft_putchar_fd('"', STDOUT_FILENO); // idem
		ft_putstr_fd(*env + equal_pos + 1, STDOUT_FILENO);// id
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

int		ft_export(char **args, t_xe *xe)
{
	size_t	nb_args;
	size_t	i;
	char	**new_array;
	ssize_t	equal_pos;
	ssize_t	var_pos;

	nb_args = ft_arraylen(args);
	if (nb_args == 1)
		print_export(xe->env, xe->exported);
	i = 1;
	while (i < nb_args)
	{
		equal_pos = ft_index(args[i], '=');
		if (check_var_name(args[i], equal_pos) == SUCCESS)
		{
			if (equal_pos != NOT_FOUND)
			{
				var_pos = get_var_pos(xe->env, args[i]);
				if (var_pos != NOT_FOUND)
				{
					free(xe->env[var_pos]);
					xe->env[var_pos] = ft_strdup(args[i]);
					if (xe->env[var_pos] == NULL)
						return (MALLOC_ERR);
				}
				else
				{
					var_pos = get_var_pos(xe->exported, args[i]);
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
			}
			else
			{
				if (get_var_pos(xe->env, args[i]) == NOT_FOUND
							&& get_var_pos(xe->exported, args[i]) == NOT_FOUND)
				{
					new_array = push_str_to_array(xe->exported, args[i]);
					if (new_array == NULL)
						return (MALLOC_ERR);
					xe->exported = new_array;
				}
			}
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
			var_pos = get_var_pos(xe->env, args[i]);
			if (var_pos != NOT_FOUND)
			{
				new_array = rm_str_from_array(xe->env, (size_t)var_pos);
				if (new_array == NULL)
					return (MALLOC_ERR);
				xe->env = new_array;
			}
			else
			{
				var_pos = get_var_pos(xe->exported, args[i]);
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
