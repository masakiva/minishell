#include "builtins.h"

int		ft_cd(char **args, t_xe *xe)
{
	int		ret;
	char	*path;
	char	*oldpwd;

	if (args[1] == NULL)
	{
		ret = get_var_pos(xe->env, "HOME", 4);
		if (ret == NOT_FOUND)
			return (HOME_NOT_SET);
		path = get_var_value(xe->env, "HOME", 4);
	}
	else if (args[2] != NULL)
	{
		printf("trop d'arguments\n"); // temp
		return (FAILURE); // temp
	}
	else
		path = ft_strdup(args[1]);
	if (path == NULL)
		return (MALLOC_ERR); // err
	oldpwd = getcwd(NULL, 0); // error
	if (chdir(path) == ERROR)
	{
		free(path);
		free(oldpwd);
		xe->stat_loc = 1;
		return (INVALID_CD_PATH); //notify failure anyway ?
	}
	free(path);
	env_replace_var("OLDPWD", oldpwd, xe);
	free(oldpwd);
	path = getcwd(NULL, 0);
	env_replace_var("PWD", path, xe);
	free(path);
	xe->stat_loc = 0;
	return (SUCCESS);
}
