#include "minishell.h"
#include <stdlib.h>
#include <unistd.h>

static int			err_output(t_all *all, const char *str)
{
	ft_putstr_fd("Error: ", all->fd[2]);
	ft_putstr_fd(str, all->fd[2]);
	ft_putstr_fd("\n", all->fd[2]);
	exit (EXIT_FAILURE);
}

static const char	*err_msg(int err_code)
{
	const char	*msg[] = {"",
		"Minishell takes no argument",
		"Memory allocation failure"};

	return (msg[err_code]);
}

int					ft_exit(int err_code, t_all *all)
{
	free_str_array(&all->current);
	free_str_array(&all->env);
	if (err_code != CLEAN_EXIT)
		return (err_output(all, err_msg(err_code)));
	exit(EXIT_SUCCESS);
}
