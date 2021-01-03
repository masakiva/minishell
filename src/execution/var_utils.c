#include "execution.h"

int		check_var_name(char *var, ssize_t name_len)
{
	size_t	i;

	if (!ft_isdigit(var[0]) && var[0] != '=' && var[0] != '+' && var[0] != '\0')
	{
		i = 0;
		while (ft_isalnum(var[i]) || var[i] == '_')
			i++;
		if ((name_len == ENTIRE_STR && var[i] == '\0')
				|| i == (size_t)name_len
				|| (i == (size_t)name_len - 1 && var[i] == '+'))
			return (SUCCESS);
	}
	return (FAILURE);
}
