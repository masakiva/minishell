#include "execution.h"

char	*create_export_string(char *var_name, char *value)
{
	char		*new;
	int			i;
	int			j;

	new = malloc(sizeof(char) * (ft_strlen(var_name) + ft_strlen(value) + 2));
	if (new == NULL)
		return (NULL);
	i = 0;
	while (var_name[i] != '\0')
	{
		new[i] = var_name[i];
		i++;
	}
	new[i] = '=';
	i += 1;
	j = 0;
	while (value[j] != '\0')
	{
		new[i + j] = value[j];
		j++;
	}
	new[i + j] = '\0';
	return (new);
}

int		env_replace_var(char *var_name, char *value, t_xe *xe)
{
	char	**new;

	new = malloc(sizeof(char*) * 3);
	if (new == NULL)
		return (MALLOC_ERR);
	new[0] = ft_strdup("export");
	if (new[0] == NULL)
	{
		free(new);
		return (MALLOC_ERR);
	}
	new[1] = create_export_string(var_name, value);
	if (new[1] == NULL)
	{
		free(new[0]);
		free(new);
		return (MALLOC_ERR);
	}
	new[2] = NULL;
//	printf("shlvl = %d\n", tmp);
	ft_export(new, xe);
	free_str_array(new);
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
