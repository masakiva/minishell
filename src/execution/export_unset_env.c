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

	nb_args = ft_arraylen(args);
	if (nb_args == 1)
		print_export(xe->env, xe->exported);
	i = 1;
	while (i < nb_args)
	{
		if (check_var_name(args[i]) == SUCCESS)
		{
			if (ft_index(args[i], '=') != -1)
			{
				new_env = append_str_to_array(args[i], xe->env);
				if (new_env == NULL)
					return (MALLOC_ERR);
				free(xe->env);
				xe->env = new_env;
			}
			else
			{
				new_exported = append_str_to_array(args[i], xe->exported);
				if (new_exported == NULL)
					return (MALLOC_ERR);
				free(xe->exported);
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
	size_t	env_size;
	ssize_t	var_pos;
	char	**new_env;
	size_t	i;
	size_t	j;

	env_size = ft_arraylen(xe->env);
	var_pos = get_var_pos(xe->env, args[1]);
	if (var_pos == -1)
		return (SUCCESS);
	new_env = (char **)malloc(sizeof(char *) * env_size);
	if (new_env == NULL)
		return (MALLOC_ERR);
	i = 0;
	j = 0;
	while (i < env_size)
	{
		if (i != (size_t)var_pos)
			new_env[j++] = xe->env[i++];
		else
			free(xe->env[i++]);
	}
	new_env[j] = NULL;
	free(xe->env);
	xe->env = new_env;
	return (SUCCESS);
}

int		ft_env(char **args, t_xe *xe)
{
	(void)args;
	ft_printarray_fd(xe->env, STDOUT_FILENO); // change fd for pipes
	return (SUCCESS);
}
