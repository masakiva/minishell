#include "execution.h"

// >>> echo
// error management: stop quand un write fail, ou alors on fait un seul write pour tout
// no nl: "This may also be achieved by appending `\c' to the end of the string" (man echo sur mac mais pas linux)
int		ft_echo(char **args, t_xe *xe)
{
	t_byte	options;

	(void)xe;
	options = shift_options(&args, ECHO_OPTIONS);
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
	xe->stat_loc = 0;
	return (SUCCESS);
}
