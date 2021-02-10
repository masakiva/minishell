#include "libft.h"

int		ft_set_index(char c, char *set)
{
	int	i;

	i = 0;
	while (set[i] != '\0')
	{
		if (c == set[i])
			return (i);
		i++;
	}
	return (ERROR);
}
