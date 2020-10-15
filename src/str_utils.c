#include <unistd.h>
#include <stdlib.h>
#include "libft.h"
#define ISNUM "0123456789"

int	ft_strcmp(const char *s1, const char *s2)
{
	int i;

	i = 0;
	if (s1 == NULL && s2 == NULL)
		return (0);
	else if (s1 == NULL)
		return (-((unsigned char*)s2)[i]);
	else if (s2 == NULL)
		return (((unsigned char*)s1)[i]);
	while (s1[i] == s2[i] && s1[i] && s2[i])
	{
		i++;
	}
	return (((unsigned char*)s1)[i] - ((unsigned char*)s2)[i]);
}

int		ft_isset(char c, char *set)
{
	int	i;

	i = 0;
	while (set[i])
	{
		if (c == set[i])
			return (i);
		i++;
	}
	return (-1);
}

//char	*ft_substr(char const *s, size_t len)
//{
//	char	*sub;
//	size_t	i;
//
//	sub = (char *)malloc(sizeof(char) * (len + 1));
//	if (sub != NULL)
//	{
//		i = 0;
//		while (i < len)
//		{
//			sub[i] = s[i];
//			i++;
//		}
//		sub[i] = '\0';
//	}
//	return (sub);
//}
//

void	ft_printarray_fd(char **array, int fd)
{
	int	i;

	i = 0;
	while (array[i] != NULL)
	{
		ft_putstr_fd(array[i], fd);
		ft_putchar_fd('\n', fd);
		i++;
	}
}

size_t	ft_arraylen(char **array)
{
	int	i;

	i = 0;
	while (array[i])
		i++;
	return (i);
}

void	free_str_array(char ***ptr)
{
	int		i;

	i = 0;
	if (ptr != NULL)
	{
		if ((*ptr) != NULL)
		{
			while ((*ptr)[i] != NULL)
			{
				free((*ptr)[i]);
				(*ptr)[i] = NULL;
				i++;
			}
			free((*ptr));
			*ptr = NULL;
		}
	}
}
