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
static int	launch_echo(t_all *all)
{
	int		i;
	int		n_option;

	n_option = DISABLED;
	if (all->current[1] != NULL)
	{
		n_option = ft_strcmp(all->current[1], "-n");
		if (n_option != ENABLED)
		{
			ft_putstr_fd(all->current[1], all->fd[1]);
			if (all->current[2] != NULL)
				ft_putchar_fd(' ', all->fd[1]);
		}
		if (all->current[2] != NULL)
			ft_putstr_fd(all->current[2], all->fd[1]);
		i = 3;
		while (all->current[i] != NULL)
		{
			ft_putchar_fd(' ', all->fd[1]);
			ft_putstr_fd(all->current[i], all->fd[1]);
			i++;
		}
	}
	if (n_option != ENABLED || all->current[1] == NULL)
		ft_putchar_fd('\n', all->fd[1]);
	return (SUCCESS);
}

//# define HOME_DIR	"Users"
//# define USER		"mvidal-a" // "abenoit"
static int	launch_cd(t_all *all)
{
	char	*path;

	if (all->current[1] == NULL)
		path = get_var_content(all->env, "HOME");
	else
		path = all->current[1];
	if (chdir(path) != SUCCESS)
	{
		perror("cd");
		free(path);
		return (ERROR);
	}
	free(path);
	return (SUCCESS);
}

static int	launch_pwd(t_all *all)
{
	char	*buf;

	(void)all;
	buf = getcwd(NULL, 0);
	if (buf == NULL)
	{
		free(buf);
		return (ERROR);
	}
	ft_putstr_fd(buf, all->fd[1]);
	ft_putchar_fd('\n', all->fd[1]);
	free(buf);
	return (SUCCESS);
}

static int	launch_export(t_all *all)
{
	size_t	i;
	size_t	size;
	char	**new;
	
	if (ft_strchr(all->current[1], '=') == NULL)
		return (SUCCESS);
	size = ft_arraylen(all->env);
	if (!(new = malloc((size + 2) * sizeof(char*))))
		return (MALLOC_ERR);
	i = 0;
	while (i < size)
	{
		new[i] = all->env[i];
		i++;
	}
	new[i] = ft_strdup(all->current[1]);
	new[i + 1] = NULL;
	free(all->env);
	all->env = new;
	return (SUCCESS);
}

static int	launch_unset(t_all *all)
{
	ssize_t	i;
	ssize_t	j;
	ssize_t	size;
	ssize_t	pos;
	char	**new;

	size = ft_arraylen(all->env);
	if ((pos = get_var_pos(all->env, all->current[1])) == -1)
		return (SUCCESS);
	if (!(new = malloc((size) * sizeof(char*))))
		return (MALLOC_ERR);
	i = 0;
	j = 0;
	while (all->env[j])
	{
		if (j != pos)
		{
			new[i] = all->env[j];
		i++;
		j++;
		}
		else
		{
			free(all->env[j]);
			all->env[j] = NULL;
			j++;
		}
	}
	new[i] = NULL;
	free(all->env);
	all->env = new;
	return (SUCCESS);
}

static int	launch_env(t_all *all)
{
	ft_printarray_fd(all->env, all->fd[1]);
	return (SUCCESS);
}

static int	launch_exit(t_all *all)
{
	(void)all;
	return (CLEAN_EXIT);
}

static int	launch_ext(t_all *all)
{
	pid_t	pid;
	char	*cmd;
	char	**path;

	(void)all;
	cmd = get_var_content(all->env, "PATH");
	path = ft_split(cmd, ':');
	ft_printarray_fd(path, all->fd[1]);
	pid = fork();
	if (pid == 0)
	{
	//	cmd = ft_strjoin("/bin/", all->current[0]);
	//	execve(cmd, &all->current[1], all->env);
		printf("%s\n", cmd);
	}
	else
		wait(NULL);
	return (SUCCESS);
}

int		launch_command(t_all *all)
{
	int				i;
	int				ret;
	const t_func	command[8] = {launch_echo, launch_cd, launch_pwd,
								launch_export, launch_unset, launch_env,
								launch_exit, launch_ext};

	i = 0;
	if ((ret = command[all->command](all)) == ERROR)
		ft_putstr_fd("ERROR\n", all->fd[1]);
	free_str_array(&all->current);
	return (ret);
}
