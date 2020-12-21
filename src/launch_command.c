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
static int	ft_echo(char **args, t_xe *xe)
{
	t_byte	options;

	(void)xe;
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

static int	ft_cd(char **args, t_xe *xe)
{
	char	*path;

	if (args[1] == NULL)
		path = get_var_value(xe->env, "HOME");
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

static int	ft_pwd(char **args, t_xe *xe)
{
	char	*buf;

	(void)args;
	(void)xe;
	buf = getcwd(NULL, 0);
	if (buf == NULL)
		return (FAILURE);
	ft_putstr_fd(buf, STDOUT_FILENO);
	ft_putchar_fd('\n', STDOUT_FILENO);
	free(buf);
	return (SUCCESS);
}

char	**append_variable_to_env(char *var, char **env)
{
	size_t	env_size;
	char	**new_env;
	size_t	i;

	env_size = ft_arraylen(env);
	new_env = (char **)malloc(sizeof(char *) * (env_size + 2));
	if (new_env == NULL)
		return (NULL);
	i = 0;
	while (i < env_size)
	{
		new_env[i] = env[i];
		i++;
	}
	new_env[i] = ft_strdup(var);
	new_env[i + 1] = NULL;
	return (new_env);
}

int		check_var_name(char *var)
{
	if (!ft_isdigit(*var))
	{
		while (ft_isalnum(*var) || *var == '_')
			var++;
		if (*var == '=' || *var == '\0')
			return (SUCCESS);
	}
	return (FAILURE);
}

static int	ft_export(char **args, t_xe *xe)
{
	size_t	nb_args;
	size_t	i;
	char	**new_env;

	nb_args = ft_arraylen(args);
	i = 1;
	while (i < nb_args)
	{
		if (check_var_name(args[i]) == SUCCESS)
		{
			if (ft_index(args[i], '=') != -1)
			{
				new_env = append_variable_to_env(args[i], xe->env);
				if (new_env == NULL)
					return (MALLOC_ERR);
				free(xe->env);
				xe->env = new_env;
			}
		}
		else
			putstr_stderr("export: Variable identifier (name) invalid\n"); // pb si le write marche pas
		i++;
	}
	return (SUCCESS);
}

static int	ft_unset(char **args, t_xe *xe)
{
	size_t	env_size;
	ssize_t	var_pos;
	char	**new_env;
	size_t	i;
	size_t	j;

	env_size = ft_arraylen(xe->env);
	var_pos = get_var_pos(xe->env, args[1]);
	if (var_pos == -1)
		return (SUCCESS);
	new_env = (char **)malloc(sizeof(char *) * env_size);
	if (new_env == NULL)
		return (MALLOC_ERR);
	i = 0;
	j = 0;
	while (i < env_size)
	{
		if (i != (size_t)var_pos)
			new_env[j++] = xe->env[i++];
		else
			free(xe->env[i++]);
	}
	new_env[j] = NULL;
	free(xe->env);
	xe->env = new_env;
	return (SUCCESS);
}

static int	ft_env(char **args, t_xe *xe)
{
	(void)args;
	ft_printarray_fd(xe->env, STDOUT_FILENO);
	return (SUCCESS);
}

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
	return (-1);
}

static int	launch_ext(char **args, t_xe *xe)
{
	pid_t	pid;
	char	*cmd;
	char	*tmp;
	char	**path;
	int		ref;
	int		ret;

	cmd = get_var_value(xe->env, "PATH");
	path = ft_split(cmd, ':');
	free(cmd);
	//	ft_printarray_fd(path, STDOUT_FILENO);
	pid = fork();
	if (pid == 0)
	{
		if ((ret = execve(args[0], args, xe->env)) == 0)
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
		execve(cmd, args, xe->env);
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
