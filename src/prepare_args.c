#include "execution.h"
#include <unistd.h>
#include <fcntl.h>

char	**extract_vars(char *str, t_list *var_positions, char **env)
{
	t_var_pos	*cur_var_pos;
	char		**ret;
	char		*var_name;
	size_t		i;

	ret = malloc(sizeof(char *) * (ft_lstsize(var_positions) + 1));
	if (ret == NULL)
		return (NULL);
	i = 0;
	while (var_positions != NULL)
	{
		cur_var_pos = var_positions->content;
		var_name = ft_substr(str, cur_var_pos->start, cur_var_pos->len);
		if (var_name == NULL) // free previous extractions
			return (NULL);
		if (*var_name == '~')
			ret[i] = get_var_value(env, "HOME");
		else
			ret[i] = get_var_value(env, var_name);
		free(var_name);
		if (ret[i] == NULL) // free previous extractions
			return (NULL);
		i++;
		var_positions = var_positions->next;
	}
	ret[i] = NULL;
	return (ret);
}

size_t	resize_token(char *str, t_list *var_positions, char **var_values)
{
	size_t		ret;
	t_var_pos	*cur_var_pos;

	ret = ft_strlen(str);
	while (var_positions != NULL)
	{
		cur_var_pos = var_positions->content;
		ret -= cur_var_pos->len;
		var_positions = var_positions->next;
	}
	while (*var_values != NULL)
	{
		ret += ft_strlen(*var_values);
		var_values++;
	}
	return (ret);
}

char	*expand_token_vars(t_token *token, char **env)
{
	char		**var_values;
	char		*ret;
	size_t		token_len;
	t_list		*var_positions;
	t_var_pos	*var;
	size_t		i;
	size_t		j;
	size_t		k;
	size_t		l;

	// if (token->var_positions == NULL)
	// 	return (ft_strdup(token->str));
	var_values = extract_vars(token->str, token->var_positions, env);
	if (var_values == NULL)
		return (NULL);
	token_len = resize_token(token->str, token->var_positions, var_values);
	ret = (char *)malloc(sizeof(char) * (token_len + 2)); // + 2?
	if (ret == NULL)
		return (NULL);
	i = 0;
	j = 0;
	l = 0;
	var_positions = token->var_positions;
	while (i < token_len)
	{
		if (var_positions != NULL) 
			var = var_positions->content;
		if (var != NULL && j == var->start)
		{
			k = 0;
			if (var_values[l] != NULL)
			{
				j = var->start + var->len;
				if (ft_strlen(var_values[l]) != 0)
				{
					while (var_values[l][k])
					{
						ret[i] = var_values[l][k];
						k++;
						i++;
					}
					j = var->start + var->len;
				}
			}
			var_positions = var_positions->next;
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
	free_str_array(&var_values);
//	i = 0;
//	while (var_values[i] != NULL)
//		free(var_values[i++]);
//	free(var_values);
	return (ret);
}

void	apply_redir(char *cur_arg, enum e_redir_op redir)
{
//	int			old_fd;
	int			redir_fd;
	mode_t		mode;
	int			flags;

	flags = 0;
	mode = 0;
	fd_backup = dup(0);
	if (redir == FILEIN)
	{
		fd_old = STDIN_FILENO;
		flags = O_RDONLY;
	}
	else
	{
		fd_old = STDOUT_FILENO;
		flags = O_WRONLY;
		flags += O_CREAT;
		mode = S_IRUSR;
		mode += S_IWUSR;
		mode += S_IRGRP;
		mode += S_IROTH;
		if (redir == APPEND)
			flags += O_APPEND;
	}
	if ((redir_fd = open(cur_arg, flags, mode)) >= 0)
	{
		dup2(fd_old, fd_backup);
		dup2(redir_fd, fd_old);
		close(redir_fd);
	}
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
  		cur_arg = expand_token_vars(cur_token, env);
//		 do checks here for pipes, redir, etc...
		if (cur_token->redir != NO_REDIR)
			apply_redir(cur_arg, cur_token->redir);
		else
		{
			args[i] = cur_arg;
			i++;
		}
		free_token(cur_token);
	}
	args[i] = NULL;
	free(command);
//	ft_printarray_fd(args, 1);
	return (args);
}
