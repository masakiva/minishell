#include "minishell.h"
#include <string.h>

static int			err_output(const char *str)
{
	ft_putstr_fd("Error: ", STDERR_FILENO);
	ft_putstr_fd(str, STDERR_FILENO);
	ft_putstr_fd("\n", STDERR_FILENO);
	exit(EXIT_FAILURE);
}

static const char	*err_msg(int err_code)
{
	const char	*msg[] = {"",
		"Minishell takes no argument",
		"Memory allocation failure"};

	return (msg[err_code]);
}

int					ft_exit(int err_code, char **env)
{
	(void)env;
	//free_str_array(&env); // besoin de free?
	if (err_code != CLEAN_EXIT && err_code > 0)
		return (err_output(err_msg(err_code)));
	exit(EXIT_SUCCESS);
}
