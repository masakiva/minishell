#include <stdlib.h>
#include "minishell.h"

static size_t	wordlen(char const *s, char *set)
{
	size_t	i;
	size_t	count;

	i = 0;
	count = 0;
	while (s[i] && ft_isset(s[i], set) < 0)
	{
		i++;
		count++;
	}
	return (count);
}

static size_t	wordcount(char const *s, char *set)
{
	size_t	i;
	size_t	count;

	i = 0;
	count = 0;
	while (s[i])
	{
		if (ft_isset(s[i], set) < 0)
		{
			count++;
			i += wordlen(&s[i], set);
		}
		else
			i++;
	}
	return (count);
}

static char		*wordcpy(char *dst, const char *src, char *set)
{
	int	i;

	i = 0;
	while (src[i] && ft_isset(src[i], set) < 0)
	{
		dst[i] = src[i];
		i++;
	}
	dst[i] = '\0';
	return (dst);
}

static char		**ft_free(char **ret, int j)
{
	int i;

	i = 0;
	while (i < j)
	{
		free(ret[i]);
		i++;
	}
	free(ret);
	return (NULL);
}

char			**ft_split(char const *s, char *set)
{
	int		i;
	int		j;
	char	**ret;

	if (s == NULL)
		return (NULL);
	if (!(ret = malloc((wordcount(s, set) + 2) * sizeof(char*))))
		return (NULL);
	i = 0;
	j = 0;
	while (s[i])
	{
		if (ft_isset(s[i], set) < 0)
		{
			if (!(ret[j] = malloc(((wordlen(&s[i], set) + 2)) * sizeof(char))))
				return (ft_free(ret, j));
			wordcpy(ret[j], &s[i], set);
			j++;
			i += wordlen(&s[i], set);
		}
		else
			i++;
	}
	ret[j] = NULL;
	return (ret);
}
