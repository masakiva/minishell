#include "execution.h"
#include <unistd.h>
#include <fcntl.h>

#include <stdio.h>
char	**extract_vars(char *str, t_list *var_properties, char **env, int stat_loc)
{
	t_var_props	*cur_var_props;
	char		**ret;
	char		*var_name;
	size_t		i;

	ret = malloc(sizeof(char *) * (ft_lstsize(var_properties) + 1));
	if (ret == NULL)
		return (NULL);
	ft_bzero(ret, sizeof(char *) * (ft_lstsize(var_properties) + 1));
	i = 0;
	while (var_properties != NULL)
	{
		cur_var_props = var_properties->content;
		var_name = ft_substr(str, cur_var_props->start, cur_var_props->len);
		if (var_name == NULL)
		{
			free_str_array(ret);
			return (NULL);
		}
		if (*var_name == '~')
			ret[i] = get_var_value(env, "HOME");
		else if (*var_name == '?')
			ret[i] = ft_itoa(stat_loc);
		else
			ret[i] = get_var_value(env, var_name);
		free(var_name);
		if (ret[i] == NULL)
		{
			free_str_array(ret);
			return (NULL);
		}
		i++;
		var_properties = var_properties->next;
	}
	return (ret);
}

size_t	resize_token(char *str, t_list *var_properties, char **var_values)
{
	size_t		ret;
	t_var_props	*cur_var_props;

	ret = ft_strlen(str);
	while (var_properties != NULL)
	{
		cur_var_props = var_properties->content;
		ret -= cur_var_props->len;
		var_properties = var_properties->next;
	}
	while (*var_values != NULL)
	{
		ret += ft_strlen(*var_values);
		var_values++;
	}
	return (ret);
}

char	*expand_token_vars(t_token *token, char **env, int stat_loc)
{
	char		***ptr;
	char		**var_values;
	char		*ret;
	size_t		token_len;
	t_list		*var_properties;
	t_var_props	*var;
	size_t		i_ret;
	size_t		j_input;
	size_t		k_value;
	size_t		l_values;

	if (token->var_properties == NULL)
		return (ft_strdup(token->str));
	var_values = extract_vars(token->str, token->var_properties, env, stat_loc);
	if (var_values == NULL)
		return (NULL);
	token_len = resize_token(token->str, token->var_properties, var_values);
	ret = (char *)malloc(sizeof(char) * (token_len + 2)); // + 2?
	if (ret == NULL)
		return (NULL);
	i_ret = 0;
	j_input = 0;
	l_values = 0;
	var_properties = token->var_properties;
	while (i_ret < token_len)
	{
		if (var_properties != NULL) 
			var = var_properties->content;
		if (var != NULL && j_input == var->start)
		{
			if (var_values[l_values] != NULL)
			{
				j_input = var->start + var->len;
				if (ft_strlen(var_values[l_values]) != 0)
				{
					k_value = 0;
					while (var_values[l_values][k_value])
					{
						ret[i_ret] = var_values[l_values][k_value];
						k_value++;
						i_ret++;
					}
					j_input = var->start + var->len;
				}
			}
			var_properties = var_properties->next;
			l_values++;
		}
		else
		{
			ret[i_ret] = token->str[j_input];
			i_ret++;
			j_input++;
		}
	}
	ret[i_ret] = '\0';
	ptr = &var_values; // ??
	free_str_array(var_values);
	return (ret);
}

char	**extract_vars_and_split(char *str, t_list *var_properties, char **env, int stat_loc)
{
	t_var_props	*cur_var_props;
	char		**ret;
	char		**tmp;
	char		*var_name;
	char		*cur_str;
	int			i;

	ret = malloc(sizeof(char *));
	if (ret == NULL)
		return (NULL);
	ret[0] = NULL;
	while (var_properties != NULL)
	{
		cur_var_props = var_properties->content;
		var_name = ft_substr(str, cur_var_props->start, cur_var_props->len);
		if (var_name == NULL)
		{
			free_str_array(ret);
			return (NULL);
		}
		if (*var_name == '~')
			cur_str = get_var_value(env, "HOME");
		else if (*var_name == '?')
			cur_str = ft_itoa(stat_loc);
		else
			cur_str = get_var_value(env, var_name);
		free(var_name);
		tmp = ft_split(cur_str, ' ');
		i = 0;
		while (tmp[i] != NULL)
		{
			ret = push_str_to_array(ret, tmp[i]);
			i++;
		}
		var_properties = var_properties->next;
	}
	return (ret);
}

char	**expand_token_and_split(t_token *token, char **env, int stat_loc)
{
	char		**ret;

	ret = extract_vars_and_split(token->str, token->var_properties, env, stat_loc);
	return (ret);
}

void	apply_redir(char *cur_arg, enum e_redir_op redir)
{
	int			src_fd;
	int			redir_fd;
	mode_t		mode;
	int			flags;

	mode = 0;
	if (redir == FILEIN)
	{
		src_fd = STDIN_FILENO;
		flags = O_RDONLY;
	}
	else
	{
		src_fd = STDOUT_FILENO;
		flags = O_WRONLY | O_CREAT;
		mode = S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH;
		if (redir == APPEND)
			flags |= O_APPEND;
	}
	redir_fd = open(cur_arg, flags, mode);
	if (redir_fd >= 0) // else error?
	{
		dup2(redir_fd, src_fd); // error
	}
}

static int	check_split_flag(t_token *cur_token)
{
	t_list		*ptr;
	t_var_props	*tmp;

	ptr = cur_token->var_properties;
	if (ft_lstsize(ptr) != 1)
		return (1);
	while (ptr != NULL)
	{
		tmp = ptr->content;
		if (tmp->split_flag == TO_SPLIT)
			return (0);
		else
			ptr = ptr->next;
	}
	return (1);
}

char **split_var_to_args(char **args, t_token *cur_token, char **env, int stat_loc)
{
	char	**tmp;
	char	***array;
	int		i;

	array = &args;
	tmp = expand_token_and_split(cur_token, env, stat_loc);
	i = 0;
	while (tmp[i] != NULL)
	{
		*array = push_str_to_array(*array, tmp[i]);
		i++;
	}
	return (*array);
}

char	**prepare_args(t_command *command, char **env, int stat_loc)
{
	t_list		*tokens;
	t_token		*cur_token;
	char		**args;
	char		*cur_arg;

	tokens = command->tokens;
	args = (char**)malloc(sizeof(char*));
	if (args == NULL)
		return (NULL); // MALLOC_ERR
	args[0] = NULL;
	while (tokens != NULL)
	{
		cur_token = ft_lstshift(&tokens);
		if (check_split_flag(cur_token) == 0)
		{
			args = split_var_to_args(args, cur_token, env, stat_loc);
		}
		else
		{
			cur_arg = expand_token_vars(cur_token, env, stat_loc);
			if (cur_arg == NULL)
			{
				free_str_array(args);
				return (NULL); // MALLOC_ERR
			}
			if (cur_token->redir != NO_REDIR)
				apply_redir(cur_arg, cur_token->redir); // error?
			else
			{
				args = push_str_to_array(args, cur_arg);
				free(cur_arg);
			}
		}
		free_token(cur_token);
	}
	free(command);
	//	ft_printarray_fd(args, 1);
	return (args);
}
