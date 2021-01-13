#include "execution.h"

#include <stdio.h> // perror

int		ft_cd(char **args, t_xe *xe)
{
	char	*path;
	char	*oldpwd;

	if (args[1] == NULL)
	{
		path = get_var_value(xe->env, "HOME", 4);
		if (path == NULL)
			return (HOME_NOT_SET);
	}
	else
		path = ft_strdup(args[1]);
	if (path == NULL)
		return (SUCCESS); // err
	errno = 0;
	oldpwd = getcwd(NULL, 0);
	if (chdir(path) == ERROR)
	{
		perror("cd");
		free(path);
		free(oldpwd);
		return (SUCCESS); //notify failure anyway ?
	}
	env_replace_var("OLDPWD", oldpwd, xe);
	free(path);
	path = getcwd(NULL, 0);
	env_replace_var("PWD", path, xe);
	free(path);
	return (SUCCESS);
}
