/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   launch_command.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abenoit <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/02/11 18:15:26 by abenoit           #+#    #+#             */
/*   Updated: 2021/02/11 18:15:37 by abenoit          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "execution.h"
#include "builtins.h"

static int		exec_parent_end(pid_t *pid, t_xe *xe)
{
	int	ret;

	signal(SIGINT, SIG_IGN);
	signal(SIGQUIT, SIG_IGN);
	ret = waitpid(*pid, &xe->stat_loc, 0);
	if (ret == ERROR)
		return (WAIT_ERROR);
	if (WIFSIGNALED(xe->stat_loc))
	{
		xe->stat_loc = WTERMSIG(xe->stat_loc);
		if (xe->stat_loc == SIGQUIT)
			ft_putstr_fd("\b\b^\\Quit (core dumped)\n", STDERR_FILENO);
		else if (xe->stat_loc == SIGINT)
		{
			xe->flags += INTERUPT;
			ft_putstr_fd("\n", STDERR_FILENO);
		}
		xe->stat_loc += 128;
	}
	else if (WIFEXITED(xe->stat_loc))
		xe->stat_loc = WEXITSTATUS(xe->stat_loc);
	signal_handler();
	return (SUCCESS);
}

static int		exec_cmd(char *cmd, char **args, t_xe *xe)
{
	pid_t	pid;

	pid = fork();
	if (pid == ERROR)
		return (FORK_ERROR);
	else if (pid == 0)
	{
		xe->flags += EXEC_PIPE;
		if (execve(cmd, args, xe->env) == ERROR)
		{
			xe->flags -= RUN;
			return (EXT_CMD_ERROR);
		}
		else
		{
			xe->flags -= RUN;
			return (SUCCESS);
		}
	}
	else
	{
		return (exec_parent_end(&pid, xe));
	}
}

static int		launch_ext(char **args, t_xe *xe)
{
	int		ret;
	char	*cmd;
	char	**path;

	ret = SUCCESS;
	if (ft_strchr(args[0], '/') != NULL)
	{
		cmd = ft_strdup(args[0]);
		if (cmd == NULL)
			return (MALLOC_ERR);
	}
	else
	{
		path = create_path_array(xe);
		if (path == NULL)
			return (MALLOC_ERR);
		ret = create_cmd(&cmd, path, args);
		free_str_array(path);
		if (ret != SUCCESS)
			return (ret);
	}
	ret = exec_cmd(cmd, args, xe);
	free(cmd);
	return (ret);
}

enum e_cmd_code	get_cmd_code(char *arg)
{
	int			i;
	static char	*cmd_list[] = {
		"echo",
		"cd",
		"pwd",
		"env",
		"export",
		"unset",
		"exit"};

	i = 0;
	while (i < 7)
	{
		if (ft_strcmp(arg, cmd_list[i]) == 0)
			return (i);
		i++;
	}
	return (ELSE);
}

int				execute_cmd(char **args, char **redir_paths,
					enum e_redir_op *redir_types, t_xe *xe)
{
	int				i;
	int				ret;
	enum e_cmd_code	cmd_code;
	const t_exec	command[8] = {ft_echo, ft_cd, ft_pwd, ft_env, ft_export,
		ft_unset, ft_exit, launch_ext};

	i = 0;
	if (redir_paths != NULL)
	{
		ret = apply_redirs(redir_paths, redir_types, xe);
		if (ret != SUCCESS)
			return (ret);
	}
	cmd_code = get_cmd_code(args[0]);
	ret = command[cmd_code](args, xe);
	if (cmd_code < 4)
	{
		if (ret == SUCCESS)
			xe->stat_loc = 0;
		else
			xe->stat_loc = 1;
	}
	return (ret);
}
