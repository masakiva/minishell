#include "execution.h"

int		ft_pwd(char **args, t_xe *xe)
{
	char	*buf;

	(void)args;
	(void)xe;
	buf = getcwd(NULL, 0);
	if (buf == NULL)
	{
		putstr_stderr("pwd : erreur de détermination du répertoire actuel : getcwd : ne peut accéder aux répertoires parents : Aucun fichier ou dossier de ce type\n"); // leaks si le write marche pas
		xe->stat_loc = 1;
		return (SUCCESS); // other code!
	}
	ft_putstr_fd(buf, STDOUT_FILENO);
	ft_putchar_fd('\n', STDOUT_FILENO);
	free(buf);
	xe->stat_loc = 0;
	return (SUCCESS);
}
