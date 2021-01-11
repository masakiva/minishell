#include "execution.h"

#include <sys/types.h> // waitpid, opendir
#include <sys/wait.h> // waitpid
#include <stdio.h> // perror
#include <dirent.h> // readdir, opendir

#include <unistd.h>
#include <fcntl.h>
#include <stdio.h>

static int	launch_exit(char **args, t_xe *xe)
{
	(void)args;
	(void)xe;
	return (CLEAN_EXIT);
}

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

int	search_exec(char **path, char *name)
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
					;//error
				return (i);
			}
			if (closedir(dirp) == ERROR)
				;//error
		}
		i++;
	}
	return (NOT_FOUND);
}

static int	launch_ext(char **args, t_xe *xe)
{
	pid_t	pid;
	char	*tmp;
	char	*cmd;
	char	**path;
	int		dir_index;

	tmp = get_var_value(xe->env, "PATH", 4); // and with PATH unset?
	if (tmp == NULL)
		return (M_ERROR);
	path = ft_split(tmp, ':');
	free(tmp);
	if (path == NULL)
		return (M_ERROR);
	pid = fork();
	if (pid == 0)
	{
		if (ft_strchr(args[0], '/') != NULL)
			if (execve(args[0], args, xe->env) == ERROR)
				perror("External function error:");
		dir_index = search_exec(path, args[0]);
		if (dir_index == NOT_FOUND)
		{
			free_str_array(path);
			return (FAILURE); // other error code
		}
		cmd = ft_strjoin(path[dir_index], "/");
		tmp = cmd;
		cmd = ft_strjoin(cmd, args[0]);
		free(tmp);
		if (execve(cmd, args, xe->env) == ERROR)
			perror("External function error:");
		free(cmd); // does not free if execve succeeds
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

void	apply_redir(char *cur_arg, enum e_redir_op redir)
{
	int			src_fd;
	int			redir_fd;
	mode_t		mode;
	int			flags;

	mode = 0;
	if (redir == FILEIN)
	{
		src_fd = STDIN_FILENO;
		flags = O_RDONLY;
	}
	else
	{
		src_fd = STDOUT_FILENO;
		flags = O_WRONLY | O_CREAT;
		mode = S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH;
		if (redir == APPEND)
			flags |= O_APPEND;
	}
	redir_fd = open(cur_arg, flags, mode);
	if (redir_fd >= 0) // else error?
	{
		dup2(redir_fd, src_fd); // error
	}
}

void	apply_redirs(t_list *redirs)
{
	t_redir		*redir;

	(void)redirs;
	return ;
	while ((redir = ft_lstshift(&redirs)) != NULL)
	{
		apply_redir(redir->path, redir->type);
	}
}

int		execute_cmd(char **args, t_list *redirs, t_xe *xe)
{
	int				i;
	int				ret;
	enum e_cmd_code	cmd_code;
	const t_exec	command[8] = {ft_echo, ft_cd, ft_pwd,
		ft_export, ft_unset, ft_env,
		launch_exit, launch_ext};

	i = 0;
	if (redirs != NULL)
		apply_redirs(redirs);
//	printf("arg0 = %s\n", args[0]);
	cmd_code = get_cmd_code(args[0]);
	if (cmd_code == M_ERROR)
		return (MALLOC_ERR);
	ret = command[cmd_code](args, xe);
	if (ret == FAILURE)
		ft_putstr_fd("COMMAND ERROR\n", 1);
	free_str_array(args);
	return (ret);
}
