#include "execution.h"

int		ft_echo(char **args, t_xe *xe)
{
	t_byte	options;

	(void)xe;
	options = shift_options(&args, ECHO_OPTIONS);
	if (*args != NULL)
	{
		if (ft_putstr_fd(*args++, STDOUT_FILENO) != WRITE_SUCCESS)
			return (WRITE_ERR);
	}
	while (*args != NULL)
	{
		if (ft_putchar_fd(' ', STDOUT_FILENO) != SUCCESS)
			return (WRITE_ERR);
		if (ft_putstr_fd(*args, STDOUT_FILENO) != WRITE_SUCCESS)
			return (WRITE_ERR);
		args++;
	}
	if (!(options & ECHO_N_OPTION))
		if (ft_putchar_fd('\n', STDOUT_FILENO) != SUCCESS)
			return (WRITE_ERR);
	xe->stat_loc = 0;
	return (SUCCESS);
}
