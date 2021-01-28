#include "execution.h"

int		ft_env(char **args, t_xe *xe)
{
	(void)args;
	ft_printarray_fd(xe->env, STDOUT_FILENO);
	xe->stat_loc = 0;
	return (SUCCESS);
}
