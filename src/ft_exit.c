#include "minishell.h"
#include <string.h>
#include <stdio.h>

void		putstr_stderr(const char *str)
{
	if (ft_putstr_fd(str, STDERR_FILENO) == ERROR)
	{
		perror("Cannot write the error message on STDERR");
		exit(EXIT_FAILURE);
	}
}

static const char	*err_msg(int err_code)
{
	const char	*msg[] = {
		"Minishell takes no argument",
		"Memory allocation failure",
		"Cannot write on standard output",
		"Cannot read standard input (GNL error)"};

	return (msg[err_code]);
}

static int			err_output(enum e_retcode err_code)
{
	putstr_stderr("Error\n");
	if (err_code <= ARG_ERR)
	{
		putstr_stderr(err_msg(err_code - 3));
		putstr_stderr("\n");
	}
	else
		perror(err_msg(err_code - 3));
	exit(EXIT_FAILURE);
}

int					ft_exit(enum e_retcode ret, char **env)
{
	(void)env;
	//free_str_array(&env); // besoin de free?
	if (ret > CLEAN_EXIT)
		err_output(ret);
	else if (ret < CLEAN_EXIT)// temp
		putstr_stderr("ERROR CODE ERROR");// temp
	exit(EXIT_SUCCESS);
}
