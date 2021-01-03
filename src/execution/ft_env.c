#include "execution.h"

int		ft_env(char **args, t_xe *xe)
{
	(void)args;
	ft_printarray_fd(xe->env, STDOUT_FILENO); // change fd for pipes
	return (SUCCESS);
}
