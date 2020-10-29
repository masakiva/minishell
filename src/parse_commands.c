#include "minishell.h"
#include <stdlib.h>

/*static*/ t_cmd_code	get_cmd_code(char *arg)
{
	char	**cmd_list;
	int		i;

	i = 0;
	cmd_list = ft_split(BUILTINS, '/');
	if (cmd_list == NULL)
		return (MALLOC_ERR);
	while (cmd_list[i] != NULL)
	{
		if (ft_strcmp(arg, cmd_list[i]) == 0)
		{
			free_str_array(&cmd_list);
			return (i);
		}
		i++;
	}
	free_str_array(&cmd_list);
	return (ELSE);
}

char	*get_var(char *str, size_t start, size_t end)
{
	char	*ret;
	size_t	size;
	size_t	i;
	size_t	j;

	size = (end - start) + 1;
	if (!(ret = malloc((size + 1) *sizeof(char))))
		return (0);
	i = start;
	j = 0;
	while (i <= end)
	{
		ret[j] = str[i];
		i++;
		j++;
	}
	ret[j] = '\0';
	return (ret);
}

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
		buf = get_var(str, var->start, var->end);
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
		ret -= (var->end - var->start) + 1;
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
				if (ft_strlen(buf[l]) == 0)
					j = var->end + 1;
				else
				{
					while (buf[l][k])
					{
						ret[i] = buf[l][k];
						k++;
						i++;
					}
					j = var->end + 1;
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
	return (ret);
}

int		parse_commands(t_list **commands, char ***env)
{
	t_command	*cmd;
	t_list		*ptr;
	t_token		*token;
	t_exe		exe;
	char		*str;
	int			ret;
	size_t		size;// only used once
	size_t		i;

	cmd = ft_lstpop(commands);
	ptr = cmd->tokens;
	size = ft_lstsize(ptr);
	exe.args = (char **)malloc(sizeof(char *) * (size + 1));
	i = 0;
	while (ptr != NULL)
	{
		token = ft_lstpop(&ptr);
  		str = remake_and_subs(token, *env);
//		 do checks here for pipes, redir, etc...
		free(token->str);
		exe.args[i] = str;
		free(token);
		i++;
	}
	exe.args[i] = NULL;
//	ft_printarray_fd(exe.args, 1);
//	print_tokens(*commands);
	exe.cmd_code = get_cmd_code(exe.args[0]);
	exe.env = &(*env);
	if ((ret = launch_command(&exe)) != SUCCESS)
		return (ret);
//	ft_printarray_fd(exe.args, 1);
	return (0);
}
