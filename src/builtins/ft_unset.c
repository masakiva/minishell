#include "builtins.h"

static int	unset_one_var(char *var, t_xe *xe)
{
	ssize_t	var_pos;
	char	**new_array;

	var_pos = get_var_pos(xe->env, var, ft_strlen(var));
	if (var_pos != NOT_FOUND)
	{
		new_array = rm_str_from_array(xe->env, (size_t)var_pos);
		if (new_array == NULL)
			return (MALLOC_ERR);
		xe->env = new_array;
	}
	else
	{
		var_pos = get_var_pos(xe->exported, var, ft_strlen(var));
		if (var_pos != NOT_FOUND)
		{
			new_array = rm_str_from_array(xe->exported, (size_t)var_pos);
			if (new_array == NULL)
				return (MALLOC_ERR);
			xe->exported = new_array;
		}
	}
	return (SUCCESS);
}

int		ft_unset(char **args, t_xe *xe)
{
	args++;
	while (*args != NULL)
	{
		if (check_var_name(*args, ENTIRE_STR) == SUCCESS)
		{
			if (unset_one_var(*args, xe) != SUCCESS)
				return (MALLOC_ERR);
		}
		else
		{
			xe->stat_loc = 1;
			ft_error(INVALID_UNS_ID, xe);
		}
		args++;
	}
	xe->stat_loc = 0;
	return (SUCCESS);
}
