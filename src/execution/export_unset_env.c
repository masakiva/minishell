#include "execution.h"

int		check_var_name(char *var)
{
	if (!ft_isdigit(*var) && *var != '=' && *var != '\0')
	{
		while (ft_isalnum(*var) || *var == '_')
			var++;
		if (*var == '=' || *var == '\0')
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
	char	**new_env;
	char	**new_exported;
	ssize_t	equal_pos;

	nb_args = ft_arraylen(args);
	if (nb_args == 1)
		print_export(xe->env, xe->exported);
	i = 1;
	while (i < nb_args)
	{
		if (check_var_name(args[i]) == SUCCESS)
		{
			equal_pos = ft_index(args[i], '=');
			if (equal_pos != -1)
			{
//				if (variable_exists(args[i], xe->env, equal_pos) == TRUE)
//				{
//					new_env = pop_str_from_array(xe->env, args[i]);
//					if (new_env == NULL)
//						return (MALLOC_ERR);
//					xe->env = new_env;
//				}
//				if (variable_exists(args[i], xe->exported, equal_pos) == TRUE)
//				{
//					new_exported = pop_str_from_array(xe->exported, args[i]);
//					if (new_exported == NULL)
//						return (MALLOC_ERR);
//					xe->exported = new_exported;
//				}
				new_env = push_str_to_array(xe->env, args[i]);
				if (new_env == NULL)
					return (MALLOC_ERR);
				xe->env = new_env;
			}
			else
			{
				new_exported = push_str_to_array(xe->exported, args[i]);
				if (new_exported == NULL)
					return (MALLOC_ERR);
				xe->exported = new_exported;
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
	char	**new_env;
	size_t	i;

	nb_args = ft_arraylen(args);
	i = 1;
	while (i < nb_args)
	{
		//if (check_var_name(args[i]) == SUCCESS)
		//{
		var_pos = get_var_pos(xe->env, args[i]);
		if (var_pos != -1)
		{
			new_env = pop_str_from_array(xe->env, (size_t)var_pos);
			if (new_env == NULL)
				return (MALLOC_ERR);
			xe->env = new_env;
		}
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
