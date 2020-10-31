#include "minishell.h"
#include "libft.h"

#include <sys/types.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <unistd.h>
#define DISABLED	1
#define ENABLED		0

// >>> echo
// error management: stop quand un write fail, ou alors on fait un seul write pour tout
// no nl: "This may also be achieved by appending `\c' to the end of the string" (man echo sur mac mais pas linux)
static int	launch_echo(char **args, char **env)
{
	int		i;
	int		n_option;

	(void)env;
	n_option = DISABLED;
	i = 1;
	if (args[i] != NULL)
	{
		n_option = ft_strcmp(args[i], "-n");
		if (n_option != ENABLED)
		{
			ft_putstr_fd(args[i], STDOUT_FILENO);
			i++;
			if (args[i] != NULL)
			{
				ft_putchar_fd(' ', STDOUT_FILENO);
				i++;
			}
		}
		while (args[i] != NULL)
		{
			ft_putchar_fd(' ', STDOUT_FILENO);
			ft_putstr_fd(args[i], STDOUT_FILENO);
			i++;
		}
	}
	if (n_option != ENABLED || args[1] == NULL)
		ft_putchar_fd('\n', STDOUT_FILENO);
	return (SUCCESS);
}

static int	launch_cd(char **args, char **env)
{
	char	*path;

	if (args[1] == NULL)
		path = get_var_content(env, "HOME");
	else
	{
		path = ft_strdup(args[1]);
	}
	if (chdir(path) != SUCCESS)
	{
		perror("cd");
		free(path);
		return (ERROR);
	}
	free(path);
	return (SUCCESS);
}

static int	launch_pwd(char **args, char **env)
{
	char	*buf;

	(void)args;
	(void)env;
	buf = getcwd(NULL, 0);
	if (buf == NULL)
	{
		free(buf);
		return (ERROR);
	}
	ft_putstr_fd(buf, STDOUT_FILENO);
	ft_putchar_fd('\n', STDOUT_FILENO);
	free(buf);
	return (SUCCESS);
}

static int	launch_export(char **args, char **env)
{
	size_t	i;
	size_t	size;
	char	**new;
	
	if (ft_strchr(args[1], '=') == NULL)
		return (SUCCESS);
	size = ft_arraylen(env);
	if (!(new = malloc((size + 2) * sizeof(char*))))
		return (MALLOC_ERR);
	i = 0;
	while (i < size)
	{
		new[i] = (env)[i];
		i++;
	}
	new[i] = ft_strdup(args[1]);
	new[i + 1] = NULL;
	free(env);
	env = new;
	return (SUCCESS);
}

static int	launch_unset(char **args, char **env)
{
	ssize_t	i;
	ssize_t	j;
	ssize_t	size;
	ssize_t	pos;
	char	**new;

	size = ft_arraylen(env);
	if ((pos = get_var_pos(env, args[1])) == -1)
		return (SUCCESS);
	if (!(new = malloc((size) * sizeof(char*))))
		return (MALLOC_ERR);
	i = 0;
	j = 0;
	while ((env)[j])
	{
		if (j != pos)
		{
			new[i] = (env)[j];
		i++;
		j++;
		}
		else
		{
			free((env)[j]);
			(env)[j] = NULL;
			j++;
		}
	}
	new[i] = NULL;
	free(env);
	env = new;
	return (SUCCESS);
}

static int	launch_env(char **args, char **env)
{
	(void)args;
	ft_printarray_fd(env, STDOUT_FILENO);
	return (SUCCESS);
}

static int	launch_exit(char **args, char **env)
{
	(void)args;
	(void)env;
	return (CLEAN_EXIT);
}

#include <dirent.h>

static int	search_path(DIR *dirp, char *name)
{
	struct dirent	*buf;

	while ((buf = readdir(dirp)) != NULL)
	{
		if (ft_strcmp(buf->d_name, name) == 0)
			return (SUCCESS);
	}
	return (ERROR);
}

int	search_exec(char **path, char *name)
{
	DIR				*dirp;
	int				i;

	i = 0;
	while (path[i] != NULL)
	{
		if ((dirp = opendir(path[i])) != NULL)
		{
			if (search_path(dirp, name) == SUCCESS)
			{
				closedir(dirp);
				return (i);
			}
			closedir(dirp);
			i++;
		}
		else
			i++;
	}
	closedir(dirp);
	return (-1);
}

static int	launch_ext(char **args, char **env)
{
	pid_t	pid;
	char	*cmd;
	char	**path;
	int		ref;
	int		ret;

	cmd = get_var_content(env, "PATH");
	path = ft_split(cmd, ':');
//	ft_printarray_fd(path, STDOUT_FILENO);
	pid = fork();
	if (pid == 0)
	{
		if ((ret = execve(args[0], args, env)) == 0)
			return (SUCCESS);
		ref = search_exec(path, args[0]);
		if (ref < 0)
			return (ERROR);
		cmd = ft_strjoin(path[ref], "/");
		cmd = ft_strjoin(cmd, args[0]);
		execve(cmd, args, env);
	}
	else
	{
		signal(SIGINT, SIG_IGN);
		signal(SIGQUIT, SIG_IGN);
		waitpid(pid, &stat_loc, 0);
		signal_handler();
	}
	return (SUCCESS);
}

int		execute_cmd(char **args, char **env)
{
	int				i;
	int				ret;
	t_cmd_code		cmd_code;
	const t_func	command[8] = {launch_echo, launch_cd, launch_pwd,
								launch_export, launch_unset, launch_env,
								launch_exit, launch_ext};

	i = 0;
	cmd_code = get_cmd_code(args[0]);
	if ((ret = command[cmd_code](args, env)) == ERROR)
		ft_putstr_fd("ERROR\n", 1);
	free_str_array(&args);
	return (ret);
}
