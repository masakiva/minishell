#include "builtins.h"

int		ft_pwd(char **args, t_xe *xe)
{
	char	*buf;

	(void)args;
	(void)xe;
	buf = getcwd(NULL, 0);
	if (buf == NULL)
		return (INVALID_PWD_PATH);
	if (ft_putendl_fd(buf, STDOUT_FILENO) != WRITE_SUCCESS)
	{
		free(buf);
		return (WRITE_ERR);
	}
	free(buf);
	return (SUCCESS);
}
