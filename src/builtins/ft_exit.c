#include "builtins.h"

int	ft_exit(char **args, t_xe *xe)
{
	if (args != NULL) // necessaire?
	{
		if (args[1] != NULL)
		{
			if (ft_isnumber(args[1]) == TRUE)
			{
				if (args[2] != NULL)
				{
					xe->stat_loc = 1;
					return (EXIT_ARG_ERR);
				}
				xe->stat_loc = ft_atoi(args[1]) % 256;
			}
			else //  "exit NaN"
			{
				xe->stat_loc = 2;
				return (FT_EXIT);//another error, to print like bash
			}
		}
	}
	return (FT_EXIT);
}
