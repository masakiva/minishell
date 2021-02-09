#include "builtins.h"

int	ft_exit(char **args, t_xe *xe)
{
	if (xe->flags & CMD_PIPE || xe->flags & CHILD)
	{
		xe->flags += EXIT_ABORT;
	}
	else
	{
		xe->flags -= RUN;
		xe->flags += EXIT_FLAG;
	}
	if (args != NULL) // necessaire?
	{
		if (args[1] != NULL)
		{
			if (ft_isnumber(args[1]) == TRUE)
			{
				if (args[2] != NULL)
				{
					xe->stat_loc = 1;
					if (!(xe->flags & RUN))
						xe->flags += RUN;
					if (xe->flags & EXIT_FLAG)
						xe->flags -= EXIT_FLAG;
					return (EXIT_ARG_ERR);
				}
				xe->stat_loc = (unsigned int)ft_atoi(args[1]) % 256;
			}
			else
			{
				xe->stat_loc = 2;
				return (EXIT_NAN);
			}
		}
	}
	return (SUCCESS);
}
