#include "execution.h"

#include <stdio.h> // perror

int		ft_cd(char **args, t_xe *xe)
{
	int		ret;
	char	*path;
	char	*oldpwd;

	if (args[1] == NULL)
	{
		ret = get_var_pos(xe->env, "HOME", 4);
		if (ret == -1)
			return (HOME_NOT_SET);
		path = get_var_value(xe->env, "HOME", 4);
	}
	else
		path = ft_strdup(args[1]);
	if (path == NULL)
		return (SUCCESS); // err
	oldpwd = getcwd(NULL, 0); // error
	if (chdir(path) == ERROR)
	{
		perror("cd");
		free(path);
		free(oldpwd);
		xe->stat_loc = 1;
		return (SUCCESS); //notify failure anyway ?
	}
	env_replace_var("OLDPWD", oldpwd, xe);
	free(path);
	path = getcwd(NULL, 0);
	env_replace_var("PWD", path, xe);
	free(path);
	xe->stat_loc = 0;
	return (SUCCESS);
}
