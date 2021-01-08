#include "execution.h"

int		env_replace_var(char *var_name, char *value, t_xe *xe)
{
	char	**new;
	int			i;
	int			j;

	new = malloc(sizeof(char*) * 3);
	new[0] = ft_strdup("export");
	new[1] = malloc(sizeof(char) * (ft_strlen(var_name) + ft_strlen(value) + 2));
	new[2] = NULL;
	if (new == NULL)
		return (MALLOC_ERR);
	i = 0;
	while (var_name[i] != '\0')
	{
		new[1][i] = var_name[i];
		i++;
	}
	new[1][i] = '=';
	i += 1;
	j = 0;
	while (value[j] != '\0')
	{
		new[1][i + j] = value[j];
		j++;
	}
	new[1][i + j] = '\0';
//	printf("shlvl = %d\n", tmp);
	ft_export(new, xe);
	return (0);
}

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
