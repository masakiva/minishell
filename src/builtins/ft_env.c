#include "builtins.h"

int		ft_env(char **args, t_xe *xe)
{
	(void)args;
	if (ft_printarray_fd(xe->env, STDOUT_FILENO) != SUCCESS)
		return (WRITE_ERR);
	return (SUCCESS);
}
