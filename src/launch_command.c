#include "minishell.h"
#include <pthread.h>

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
//	if (all->current[1] == NULL)
//	{
//		errno = 0;
//		if (chdir("/HOME_DIR/USER") != SUCCESS)
//		{
//			perror("chdir");
//			return (ERROR);
//		}
//	}
	(void)all;
	ft_putstr_fd("command: cd\n", all->fd[1]);
	return (SUCCESS);
}

static int	launch_pwd(t_all *all)
{
	char	*buf;

	(void)all;
	buf = getcwd(NULL, 0);
	if (buf == NULL)
	{
		//free(buf);
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

	size = ft_arraylen(all->env);
	if (!(new = malloc((size + 2) * sizeof(char*))))
		return (MALLOC_ERR);
	i = 0;
	while (i <= size)
	{
		new[i] = all->env[i];
		i++;
	}
	new[i] = all->current[1];
	ft_putstr_fd(all->current[1], all->fd[1]);
	new[i+1] = NULL;
	free_str_array(&(all->env));
	all->env = new;
	return (SUCCESS);
}

static int	launch_unset(t_all *all)
{
	(void)all;
	ft_putstr_fd("command: unset\n", all->fd[1]);
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
	(void)all;
	execve("/bin/ls", &all->current[1], all->env);
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
