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
		return (CD_ARG_ERR);
	else
		path = ft_strdup(args[1]);
	if (path == NULL)
		return (MALLOC_ERR);
	oldpwd = get_var_value(xe->env, "PWD", 3);
	if (oldpwd == NULL)
	{
		free(path);
		return (MALLOC_ERR);
	}
	if (chdir(path) == ERROR)
	{
		free(path);
		free(oldpwd);
		xe->stat_loc = 1;
		return (INVALID_CD_PATH);
	}
	free(path);
	env_replace_var("OLDPWD", oldpwd, xe);// erreur
	free(oldpwd);
	path = getcwd(NULL, 0);
	if (path == NULL)
		return (INVALID_CD_NEW_PATH);
	env_replace_var("PWD", path, xe);// erreur
	free(path);
	xe->stat_loc = 0;
	return (SUCCESS);
}
