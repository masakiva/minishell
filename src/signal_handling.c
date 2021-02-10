#include "minishell.h"

void	sig_int(int signum)
{
	(void)signum;
	ft_putstr_fd("\n", STDIN_FILENO);
	if (ft_putstr_fd(FT_PS1, STDOUT_FILENO) == ERROR)
		return ;
}

void	sig_kill(int signum)
{
	(void)signum;
	ft_putstr_fd("\b\b  \b\b", STDIN_FILENO);
}

void				signal_handler(void)
{
	signal(SIGINT, sig_int);
	signal(SIGQUIT, sig_kill);
}
