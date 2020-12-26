#include "execution.h"

t_byte		check_arg_for_options(char *arg, char *op_chars)
{
	size_t	i;
	ssize_t	j;
	t_byte	options;

	options = 0;
	if (arg[0] == '-')
	{
		i = 1;
		while (arg[i] != '\0')
		{
			j = ft_index(op_chars, arg[i]);
			if (j != NOT_FOUND && j < 8)
				options |= 0b00000001 << j;
			else
				return (0);
			i++;
		}
	}
	return (options);
}

t_byte		shift_options(char ***args, char *op_chars)
{
	t_byte	cur_options;
	t_byte	options;

	options = 0;
	(*args)++;
	while (**args != NULL)
	{
		cur_options = check_arg_for_options(**args, op_chars);
		if (cur_options == 0)
			break ;
		else
			options |= cur_options;
		(*args)++;
	}
	return (options);
}

