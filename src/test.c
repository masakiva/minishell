#include "execution.h"
#include <unistd.h>
#include <fcntl.h>

#include <stdio.h>

void	apply_redir(char **cur_arg, enum e_redir_op redir)
{
	int			src_fd;
	int			redir_fd;
	mode_t		mode;
	int			flags;

	mode = 0;
	if (ft_arraylen(cur_arg) != 1)
	{
		printf("redirection ambiguë\n");
		free_str_array(cur_arg);
		return ;
	}
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

char	**prepare_args(t_command *command, char **env, int stat_loc)
{
	t_list		*tokens;
	t_token		*cur_token;
	char		**args;
	char		**cur_args;
	int			i;

	tokens = command->tokens;
	args = (char**)malloc(sizeof(char*));
	if (args == NULL)
		return (NULL); // MALLOC_ERR
	args[0] = NULL;
	while (tokens != NULL)
	{
		cur_token = ft_lstshift(&tokens);
		cur_args = expand_token_vars(cur_token, env, stat_loc);
		if (cur_arg == NULL)
		{
			free_str_array(args);
			return (NULL); // MALLOC_ERR
		}
		if (cur_token->redir != NO_REDIR)
			apply_redir(cur_args, cur_token->redir); // error?
		else
		{
			args = merge_str_array(args, cur_args);
		}
		free_token(cur_token);
	}
	free(command);
	//	ft_printarray_fd(args, 1);
	return (args);
}
