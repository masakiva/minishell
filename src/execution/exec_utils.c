#include "execution.h"

static int	search_path(DIR *dirp, char *name)
{
	struct dirent	*buf;

	buf = readdir(dirp);
	while (buf != NULL)
	{
		if (ft_strcmp(buf->d_name, name) == 0)
			return (SUCCESS);
		buf = readdir(dirp);
	}
	return (FAILURE);
}

static int	search_exec(char **path, char *name)
{
	DIR		*dirp;
	size_t	i;

	i = 0;
	while (path[i] != NULL)
	{
		if ((dirp = opendir(path[i])) != NULL)
		{
			if (search_path(dirp, name) == SUCCESS)
			{
				if (closedir(dirp) == ERROR)
					return (INVALID_PATH_DIR);
				return (i);
			}
			if (closedir(dirp) == ERROR)
				return (INVALID_PATH_DIR);
		}
		i++;
	}
	return (NOT_FOUND);
}

int	create_cmd(char **tmp, char **path, char **args)
{
	char	*cmd;
	char	*dir;
	int		dir_index;

	dir_index = search_exec(path, args[0]);
	if (dir_index == NOT_FOUND)
		return (CMD_NOT_FOUND);
	dir = ft_strjoin(path[dir_index], "/");
	cmd = ft_strjoin(dir, args[0]);
	if (cmd == NULL)
		return (MALLOC_ERR);
	free(dir);
	*tmp = cmd;
	return (SUCCESS);
}

static char	*add_path_to_localdir(void)
{
	char	*tmp;

	tmp = malloc(sizeof(char) + 2);
	if (tmp == NULL)
		return (NULL);
	tmp[0] = '.';
	tmp[1] = '/';
	tmp[2] = '\0';
	return (tmp);
}

char	**create_path_array(t_xe *xe)
{
	int		ret;
	char	*tmp;
	char	**path;

	ret = get_var_pos(xe->env, "PATH", 4);
	if (ret == NOT_FOUND)
	{
		path = malloc((sizeof(char*) * 2));
		path[0] = add_path_to_localdir();
		if (path[0] == NULL)
			return (NULL);
		path[1] = NULL;
	}
	else
	{
		tmp = get_var_value(xe->env, "PATH", 4);
		if (tmp == NULL)
			return (NULL);
		path = ft_split(tmp, ':');
		free(tmp);
		if (path == NULL)
			return (NULL);
	}
	return (path);
}
