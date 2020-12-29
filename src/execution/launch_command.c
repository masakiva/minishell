#include "execution.h"

#include <sys/types.h> // waitpid
#include <sys/wait.h> // waitpid

static int	launch_exit(char **args, t_xe *xe)
{
	(void)args;
	(void)xe;
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
	return (FAILURE);
}

int	search_exec(char **path, char *name)
{
	DIR		*dirp;
	int		i;

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
	//closedir(dirp);
	return (NOT_FOUND);
}

static int	launch_ext(char **args, t_xe *xe)
{
	pid_t	pid;
	char	*cmd;
	char	*tmp;
	char	**path;
	int		ref;
	int		ret;

	cmd = get_var_value(xe->env, "PATH"); // and with PATH unset?
	path = ft_split(cmd, ':');
	free(cmd);
	pid = fork();
	if (pid == 0)
	{
		ret = 0; // because execve does not return (what does that mean?) on success...
		ret = execve(args[0], args, xe->env);
		if (ret == 0)
			return (SUCCESS);
		else
			;//what to do?
		ref = search_exec(path, args[0]);
		if (ref < 0)
		{
			free_str_array(path);
			return (FAILURE);
		}
		cmd = ft_strjoin(path[ref], "/");
		tmp = cmd;
		cmd = ft_strjoin(cmd, args[0]);
		free(tmp);
		execve(cmd, args, xe->env);
		free(cmd);
	}
	else
	{
		signal(SIGINT, SIG_IGN);
		signal(SIGQUIT, SIG_IGN);
		waitpid(pid, &xe->stat_loc, 0); // return value? error?
		signal_handler();
	}
	free_str_array(path);
	return (SUCCESS);
}

enum e_cmd_code	get_cmd_code(char *arg)
{
	char	**cmd_list;
	int		i;

	i = 0;
	cmd_list = ft_split(BUILTINS, '/');
	if (cmd_list == NULL)
		return (M_ERROR);
	while (cmd_list[i] != NULL)
	{
		if (ft_strcmp(arg, cmd_list[i]) == 0)
		{
			free_str_array(cmd_list);
			return (i);
		}
		i++;
	}
	free_str_array(cmd_list);
	return (ELSE);
}

int		execute_cmd(char **args, t_xe *xe)
{
	int				i;
	int				ret;
	enum e_cmd_code		cmd_code;
	const t_exec	command[8] = {ft_echo, ft_cd, ft_pwd,
		ft_export, ft_unset, ft_env,
		launch_exit, launch_ext};

	i = 0;
	cmd_code = get_cmd_code(args[0]);
	if (cmd_code == M_ERROR)
		return (MALLOC_ERR);
	ret = command[cmd_code](args, xe);
	if (ret == FAILURE)
		ft_putstr_fd("COMMAND ERROR\n", 1);
	free_str_array(args);
	if (gpid == 0)
	{
		close(fd_old);
		close(STDIN_FILENO);
	}
	if (fd_backup != -1)
		dup2(fd_backup, fd_old);
	return (ret);
}
