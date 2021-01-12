#include "parsing.h"

int		parsing_error(void)
{
	const char		*err_msg[NB_PARSING_ERRORS] = {
		"=== no error (here for padding) ===",
		"No matching single quote",
		"No matching double quote",
		"Multiline inputs are currently not supported",
		"Redirection path invalid",
		"No redirection path specified",
		"Empty command before ; or |"};

	(void)err_msg;
	ft_putstr_fd("minishell: syntax error: ", STDERR_FILENO);
//	ft_putstr_fd(err_msg[machine->error], STDERR_FILENO);
	ft_putchar_fd('\n', STDERR_FILENO);
	return (PARSING_ERR);
}

int		check_syntax(char *line)
{
	int	i;

	i = 0;
	while (line[i])
	{
		i++;
	}
	return (SUCCESS);
}
