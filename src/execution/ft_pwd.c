#include "execution.h"

int		ft_pwd(char **args, t_xe *xe)
{
	char	*buf;

	(void)args;
	(void)xe;
	buf = getcwd(NULL, 0);
	if (buf == NULL)
		return (FAILURE); // other code!
	ft_putstr_fd(buf, STDOUT_FILENO);
	ft_putchar_fd('\n', STDOUT_FILENO);
	free(buf);
	return (SUCCESS);
}
