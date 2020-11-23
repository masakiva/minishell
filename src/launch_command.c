#include "execution.h"

#include <stdio.h> // perror
#include <sys/types.h> // waitpid
#include <sys/wait.h> // waitpid

t_byte		check_arg_for_options(char *arg, char *op_chars)
{
	size_t	i;
	ssize_t	j;
	t_byte	options;

	options = 0;
	if (arg[0] == '-')
	{
		i = 1;
		while (arg[i] != '\0')
		{
			j = ft_isset(arg[i], op_chars);
			if (j != -1 && j < 8)
				options |= 0b00000001 << j;
			else
				return (0);
			i++;
		}
	}
	return (options);
}

t_byte		pop_options(char ***args, char *op_chars)
{
	t_byte	cur_options;
	t_byte	options;

	options = 0;
	(*args)++;
	while (**args != NULL)
	{
		cur_options = check_arg_for_options(**args, op_chars);
		if (cur_options == 0)
			break ;
		else
			options |= cur_options;
		(*args)++;
	}
	return (options);
}

// >>> echo
// error management: stop quand un write fail, ou alors on fait un seul write pour tout
// no nl: "This may also be achieved by appending `\c' to the end of the string" (man echo sur mac mais pas linux)
static int	ft_echo(char **args, char **env)
{
	t_byte	options;

	(void)env;
	options = pop_options(&args, ECHO_OPTIONS);
	if (*args != NULL)
		ft_putstr_fd(*args++, STDOUT_FILENO);
	while (*args != NULL)
	{
		ft_putchar_fd(' ', STDOUT_FILENO);
		ft_putstr_fd(*args, STDOUT_FILENO);
		args++;
	}
	if (!(options & ECHO_N_OPTION))
		ft_putchar_fd('\n', STDOUT_FILENO);
	return (SUCCESS);
}

static int	ft_cd(char **args, char **env)
{
	char	*path;

	if (args[1] == NULL)
		path = get_var_value(env, "HOME");
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

static int	ft_pwd(char **args, char **env)
{
	char	*buf;

	(void)args;
	(void)env;
	buf = getcwd(NULL, 0);
	if (buf == NULL)
		return (FAILURE);
	ft_putstr_fd(buf, STDOUT_FILENO);
	ft_putchar_fd('\n', STDOUT_FILENO);
	free(buf);
	return (SUCCESS);
}

static int	ft_export(char **args, char **env)
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

static int	ft_unset(char **args, char **env)
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

static int	ft_env(char **args, char **env)
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
	return (-1);
}

static int	launch_ext(char **args, char **env)
{
	pid_t	pid;
	char	*cmd;
	char	*tmp;
	char	**path;
	int		ref;
	int		ret;

	cmd = get_var_value(env, "PATH");
	path = ft_split(cmd, ':');
	free(cmd);
	//	ft_printarray_fd(path, STDOUT_FILENO);
	pid = fork();
	if (pid == 0)
	{
		if ((ret = execve(args[0], args, env)) == 0)
			return (SUCCESS);
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
		execve(cmd, args, env);
		free(cmd);
	}
	else
	{
		signal(SIGINT, SIG_IGN);
		signal(SIGQUIT, SIG_IGN);
		waitpid(pid, &stat_loc, 0);
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

int		execute_cmd(char **args, char **env)
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
	ret = command[cmd_code](args, env);
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
