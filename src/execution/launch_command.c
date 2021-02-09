#include "execution.h"
#include "builtins.h"

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

static int	exec_cmd(char *cmd, char **args, t_xe *xe)
{
	pid_t	pid;

	pid = fork();
	if (pid == 0)
	{
		xe->flags += EXEC_PIPE;
		if (execve(cmd, args, xe->env) == ERROR)
		{
			xe->flags -= RUN;
			return (EXT_CMD_ERROR);
		}
		else
		{
			xe->flags -= RUN;
			return (SUCCESS);
		}
	}
	else
	{
		signal(SIGINT, SIG_IGN); // error if SIG_ERR? (check errno)
		signal(SIGQUIT, SIG_IGN); // error if SIG_ERR? (check errno)
		waitpid(pid, &xe->stat_loc, 0); // return value? error?
		if (WIFSIGNALED(xe->stat_loc))
		{
			xe->stat_loc = WTERMSIG(xe->stat_loc);
			if (xe->stat_loc == SIGQUIT)
			{
				if (ft_putstr_fd("\b\b^\\Quit (core dumped)\n", STDERR_FILENO) != WRITE_SUCCESS)
					return (WRITE_ERR);
			}
			else if (xe->stat_loc == SIGINT)
			{
				if (ft_putstr_fd("\n", STDERR_FILENO) != WRITE_SUCCESS)
					return (WRITE_ERR);
			}
			xe->stat_loc += 128;
		}
		else if(WIFEXITED(xe->stat_loc))
			xe->stat_loc = WEXITSTATUS(xe->stat_loc);
		signal_handler(); // err
		return (SUCCESS);
	}
}

static int	create_cmd(char **tmp, char **path, char **args)
{
	char	*cmd;
	char	*dir;
	int		dir_index;

	dir_index = search_exec(path, args[0]);
	if (dir_index == NOT_FOUND)
		return (NO_SUCH_FILE); // other error code
	dir = ft_strjoin(path[dir_index], "/");
	cmd = ft_strjoin(dir, args[0]);
	if (cmd == NULL)
		return (MALLOC_ERR);
	free(dir);
	*tmp = cmd;
	return (SUCCESS);
}

static int	add_path_to_localdir(char ***path)
{
	char	*tmp;

	tmp = malloc(sizeof(char) + 2);
	if (tmp == NULL)
		return (MALLOC_ERR);
	tmp[0] = '.';
	tmp[1] = '/';
	tmp[2] = '\0';
	*path = push_str_to_array(*path, tmp);
	return (SUCCESS);
}

static int	launch_ext(char **args, t_xe *xe)
{
	int		ret;
	char	*tmp;
	char	*cmd;
	char	**path;

	ret = SUCCESS;
	if (ft_strchr(args[0], '/') != NULL)
	{
		cmd = ft_strdup(args[0]);
		if (cmd == NULL)
			return (MALLOC_ERR);
	}
	else
	{
		ret = get_var_pos(xe->env, "PATH", 4); // and with PATH unset?
		if (ret == -1)
		{
			path = malloc(sizeof(char*));
			*path = NULL;
		}
		else
		{
			tmp = get_var_value(xe->env, "PATH", 4); // and with PATH unset?
			if (tmp == NULL)
				return (MALLOC_ERR);
			path = ft_split(tmp, ':');
			free(tmp);
			if (path == NULL)
				return (MALLOC_ERR);
		}
		add_path_to_localdir(&path);
		ret = create_cmd(&cmd, path, args);
		free_str_array(path);
		if (ret != SUCCESS)
			return (ret);
	}
	ret = exec_cmd(cmd, args, xe);
	free(cmd);
	return (ret);
}

enum e_cmd_code	get_cmd_code(char *arg)
{
	static char	*cmd_list[] = {
		"echo",
		"cd",
		"pwd",
		"env",
		"export",
		"unset",
		"exit"};
	int		i;

	i = 0;
	while (i < 7)
	{
		if (ft_strcmp(arg, cmd_list[i]) == 0)
			return (i);
		i++;
	}
	return (ELSE);
}

int		apply_redir(char *cur_arg, enum e_redir_op redir, t_xe *xe)
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
		mode = S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP | S_IROTH;
		if (redir == APPEND)
			flags |= O_APPEND;
	}
	redir_fd = open(cur_arg, flags, mode);
	if (redir_fd >= 0) // else error
	{
		dup2(redir_fd, src_fd); // error
	}
	else
	{
		xe->stat_loc = 1;
		return (FD_ERROR);
	}
	return (SUCCESS);
}

int		apply_redirs(char **redir_paths, enum e_redir_op *redir_types, t_xe *xe)
{
	int		i;
	int		ret;

	i = 0;
	while (redir_paths[i] != NULL)
	{
		ret = apply_redir(redir_paths[i], redir_types[i], xe);
		if (ret != SUCCESS)
			return (ret);
		i++;
	}
	return (SUCCESS);
}

int		execute_cmd(char **args, char **redir_paths, enum e_redir_op *redir_types, t_xe *xe)
{
	int				i;
	int				ret;
	enum e_cmd_code	cmd_code;
	const t_exec	command[8] = {ft_echo, ft_cd, ft_pwd, ft_env, ft_export,
		ft_unset, ft_exit, launch_ext};

	i = 0;
	if (redir_paths != NULL)
	{
		ret = apply_redirs(redir_paths, redir_types, xe);
		if (ret != SUCCESS)
			return (ret);
	}
	cmd_code = get_cmd_code(args[0]);
	ret = command[cmd_code](args, xe);
	if (cmd_code < 4)
	{
		if (ret == SUCCESS)
			xe->stat_loc = 0;
		else
			xe->stat_loc = 1;
	}
	return (ret);
}
