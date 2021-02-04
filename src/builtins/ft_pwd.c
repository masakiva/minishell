#include "builtins.h"

int		ft_pwd(char **args, t_xe *xe)
{
	char	*buf;
	int		ret;

	(void)args;
	(void)xe;
	ret = SUCCESS;
	buf = getcwd(NULL, 0);
	if (buf != NULL)
	{
		if (ft_putendl_fd(buf, STDOUT_FILENO) != WRITE_SUCCESS)
			ret = WRITE_ERR;
		free(buf);
	}
	else
		ret = INVALID_PWD_PATH;
	return (ret);
}
