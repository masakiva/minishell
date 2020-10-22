#include "minishell.h"
#include "libft.h"
#include <pthread.h>

#include <sys/types.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <unistd.h>
#define DISABLED	1
#define ENABLED		0

// >>> echo
// error management: stop quand un write fail, ou alors on fait un seul write pour tout
// no nl: "This may also be achieved by appending `\c' to the end of the string" (man echo sur mac mais pas linux)
static int	launch_echo(t_exe *exe)
{
	int		i;
	int		n_option;

	n_option = DISABLED;
	if (exe->args[1] != NULL)
	{
		n_option = ft_strcmp(exe->args[1], "-n");
		if (n_option != ENABLED)
		{
			ft_putstr_fd(exe->args[1], STDOUT_FILENO);
			if (exe->args[2] != NULL)
				ft_putchar_fd(' ', STDOUT_FILENO);
		}
		if (exe->args[2] != NULL)
			ft_putstr_fd(exe->args[2], STDOUT_FILENO);
		i = 3;
		while (exe->args[i] != NULL)
		{
			ft_putchar_fd(' ', STDOUT_FILENO);
			ft_putstr_fd(exe->args[i], STDOUT_FILENO);
			i++;
		}
	}
	if (n_option != ENABLED || exe->args[1] == NULL)
		ft_putchar_fd('\n', STDOUT_FILENO);
	return (SUCCESS);
}

static int	launch_cd(t_exe *exe)
{
	char	*path;

	if (exe->args[1] == NULL)
		path = get_var_content(*(exe->env), "HOME");
	else
	{
		path = ft_strdup(exe->args[1]);
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

static int	launch_pwd(t_exe *exe)
{
	char	*buf;

	(void)exe;
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

static int	launch_export(t_exe *exe)
{
	size_t	i;
	size_t	size;
	char	**new;
	
	if (ft_strchr(exe->args[1], '=') == NULL)
		return (SUCCESS);
	size = ft_arraylen(*(exe->env));
	if (!(new = malloc((size + 2) * sizeof(char*))))
		return (MALLOC_ERR);
	i = 0;
	while (i < size)
	{
		new[i] = (*(exe->env))[i];
		i++;
	}
	new[i] = ft_strdup(exe->args[1]);
	new[i + 1] = NULL;
	free(*(exe->env));
	*(exe->env) = new;
	return (SUCCESS);
}

static int	launch_unset(t_exe *exe)
{
	ssize_t	i;
	ssize_t	j;
	ssize_t	size;
	ssize_t	pos;
	char	**new;

	size = ft_arraylen(*(exe->env));
	if ((pos = get_var_pos(*(exe->env), exe->args[1])) == -1)
		return (SUCCESS);
	if (!(new = malloc((size) * sizeof(char*))))
		return (MALLOC_ERR);
	i = 0;
	j = 0;
	while ((*(exe->env))[j])
	{
		if (j != pos)
		{
			new[i] = (*(exe->env))[j];
		i++;
		j++;
		}
		else
		{
			free((*(exe->env))[j]);
			(*(exe->env))[j] = NULL;
			j++;
		}
	}
	new[i] = NULL;
	free(*(exe->env));
	*(exe->env) = new;
	return (SUCCESS);
}

static int	launch_env(t_exe *exe)
{
	ft_printarray_fd(*(exe->env), STDOUT_FILENO);
	return (SUCCESS);
}

static int	launch_exit(t_exe *exe)
{
	(void)exe;
	return (CLEAN_EXIT);
}

static int	launch_ext(t_exe *exe)
{
	pid_t	pid;
	char	*cmd;
	char	**path;

	(void)exe;
	cmd = get_var_content(*(exe->env), "PATH");
	path = ft_split(cmd, ':');
	ft_printarray_fd(path, STDOUT_FILENO);
	pid = fork();
	if (pid == 0)
	{
	//	cmd = ft_strjoin("/bin/", exe->args[0]);
	//	execve(cmd, &exe->args[1], *(exe->env));
		printf("%s\n", cmd);
	}
	else
		wait(NULL);
	return (SUCCESS);
}

int		launch_command(t_exe *exe)
{
	int				i;
	int				ret;
	const t_func	command[8] = {launch_echo, launch_cd, launch_pwd,
								launch_export, launch_unset, launch_env,
								launch_exit, launch_ext};

	i = 0;
	if ((ret = command[exe->cmd_code](exe)) == ERROR)
		ft_putstr_fd("ERROR\n", 1);
	free_str_array(&exe->args);
	return (ret);
}
