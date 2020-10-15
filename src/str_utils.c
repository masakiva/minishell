#include <unistd.h>
#include <stdlib.h>
#define ISNUM "0123456789"

//void	ft_putchar(char c)
//{
//	write(1, &c, 1);
//}
//
//void	ft_putstr(const char *str)
//{
//	while (*str)
//	{
//		ft_putchar(*str);
//		str++;
//	}
//}

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

char	*ft_strdup(const char *s1)
{
	size_t	i;
	char	*cpy;

	i = 0;
	while (s1[i])
		i++;
	if (!(cpy = malloc((i + 1) * sizeof(char))))
		return (NULL);
	i = 0;
	while (s1[i])
	{
		cpy[i] = s1[i];
		i++;
	}
	cpy[i] = '\0';
	return (cpy);
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
