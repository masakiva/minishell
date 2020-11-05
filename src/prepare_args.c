#include "execution.h"

//char	*get_var(char *str, size_t start, size_t end)
//{
//	char	*ret;
//	size_t	size;
//	size_t	i;
//	size_t	j;
//
//	size = (end - start) + 1;
//	if (!(ret = malloc((size + 1) *sizeof(char))))
//		return (0);
//	i = start;
//	j = 0;
//	while (i <= end)
//	{
//		ret[j] = str[i];
//		i++;
//		j++;
//	}
//	ret[j] = '\0';
//	return (ret);
//}
//
char	**extract_vars(char *str, t_list *lst, char **env)
{
	t_list		*ptr;
	t_variable	*var;
	char		**ret;
	char		*buf;
	size_t		size;
	size_t		i;

	ptr = lst;
	size = ft_lstsize(ptr);
	if (!(ret = malloc((size + 1) * sizeof(char*))))
		return (NULL);
	i = 0;
	while (ptr != NULL)
	{
		var = ptr->content;
		buf = ft_substr(str, var->start, var->len);
		if (*buf == '~')
			ret[i] = get_var_content(env, "HOME");
		else
			ret[i] = get_var_content(env, buf);
		if (ret[i] == NULL)
			ret[i] = "";
		i++;
		ptr = ptr->next;
	}
	ret[i] = NULL;
	return (ret);
}

size_t	resize_token(char *str, t_list *lst)
{
	size_t		ret;
	t_variable	*var;

	ret = ft_strlen(str);
	while (lst != NULL)
	{
		var = lst->content;
		ret -= var->len;
		lst = lst->next;
	}
	return (ret);
}

char	*remake_and_subs(t_token *token, char **env)
{
	char		**buf;
	char		*ret;
	size_t		size;
	t_list		*ptr;
	t_variable	*var;
	size_t		i;
	size_t		j;
	size_t		k;
	size_t		l;

	size = resize_token(token->str, token->vars);
	buf = extract_vars(token->str, token->vars, env);
	k = 0;
	while (buf[k] != NULL)
	{
		size += ft_strlen(buf[k]);
		k++;
	}
	if (!(ret = (char *)malloc((size + 2) * sizeof(char))))
		return (NULL);
	i = 0;
	j = 0;
	l = 0;
	ptr = token->vars;
	while (i < size)
	{
		if (ptr != NULL) 
			var = ptr->content;
		if (var != NULL && j == var->start)
		{
			k = 0;
			if (buf[l] != NULL)
			{
				j = var->start + var->len;
				if (ft_strlen(buf[l]) != 0)
				{
					while (buf[l][k])
					{
						ret[i] = buf[l][k];
						k++;
						i++;
					}
					j = var->start + var->len;
				}
			}
			ptr = ptr->next;
			l++;
		}
		else
		{
			ret[i] = token->str[j];
			i++;
			j++;
		}
	}
	ret[i] = '\0';
	free(buf);
	return (ret);
}

char	**prepare_args(t_command *command, char **env)
{
	t_list		*tokens;
	t_token		*cur_token;
	char		**args;
	char		*cur_arg;
	size_t		i;

	tokens = command->tokens;
	args = (char **)malloc(sizeof(char *) * (ft_lstsize(tokens) + 1));
	i = 0;
	while (tokens != NULL)
	{
		cur_token = ft_lstpop(&tokens);
  		cur_arg = remake_and_subs(cur_token, env);
//		 do checks here for pipes, redir, etc...
		args[i] = cur_arg;
		free_token(cur_token);
		i++;
	}
	args[i] = NULL;
	free(command);
//	ft_printarray_fd(args, 1);
	return (args);
}
