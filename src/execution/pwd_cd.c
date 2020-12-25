#include "execution.h"

#include <stdio.h> // perror

int		ft_cd(char **args, t_xe *xe)
{
	char	*path;

	if (args[1] == NULL)
		path = get_var_value(xe->env, "HOME");
	else
		path = ft_strdup(args[1]);
	if (path == NULL)
		; // err
	errno = 0;
	if (chdir(path) == ERROR)
	{
		perror("cd");
		free(path);
		return (FAILURE);
	}
	free(path);
	return (SUCCESS);
}

int		ft_pwd(char **args, t_xe *xe)
{
	char	*buf;

	(void)args;
	(void)xe;
	buf = getcwd(NULL, 0);
	if (buf == NULL)
		return (FAILURE);
	ft_putstr_fd(buf, STDOUT_FILENO);
	ft_putchar_fd('\n', STDOUT_FILENO);
	free(buf);
	return (SUCCESS);
}
