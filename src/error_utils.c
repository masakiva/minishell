/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   error_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abenoit <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/02/11 18:50:30 by abenoit           #+#    #+#             */
/*   Updated: 2021/02/11 18:54:13 by abenoit          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "execution.h"
#include "parsing.h"

int			parsing_error(int err_code, t_xe *xe)
{
	const char		*err_msg[] = {
		"No matching single quote",
		"No matching double quote",
		"Multiline inputs are currently not supported",
		"Redirection path invalid",
		"No redirection path specified",
		"Empty command before ; or |"};

	ft_putstr_fd("syntax error: ", STDERR_FILENO);
	ft_putendl_fd(err_msg[err_code - _PARSING_ERROR_ - 1], STDERR_FILENO);
	xe->stat_loc = 2;
	return (SUCCESS);
}

int			exec_error(int err_code, t_xe *xe)
{
	const char		*err_msg[] = {
		"Ambiguous redirection",
		"Commmand not found",
		"export: Variable identifier (name) invalid",
		"unset: Variable identifier (name) invalid",
		"HOME not set",
		"cd: too many arguments",
		"exit: too many arguments"};

	ft_putstr_fd("command error: ", STDERR_FILENO);
	ft_putendl_fd(err_msg[err_code - _EXEC_ERROR_ - 1], STDERR_FILENO);
	if (err_code == CMD_NOT_FOUND)
		xe->stat_loc = 127;
	if (xe->flags & EXEC_PIPE || xe->flags & CHILD)
		xe->flags = 0;
	else if (err_code == EXIT_ARG_ERR)
		xe->flags = RUN;
	return (SUCCESS);
}

static int	ext_cmd_exit(t_xe *xe)
{
	if (errno == ENOENT)
		xe->stat_loc = 127;
	else if (errno == EACCES || errno == ENOEXEC)
		xe->stat_loc = 126;
	return (exit_trigger(xe->stat_loc, xe));
}

int			err_output(int err_code, t_xe *xe)
{
	const char	*err_msg[] = {
		"Memory allocation failure",
		"Cannot write on standard output",
		"Cannot read standard input (GNL error)",
		"cd: cannot change directory",
		"cd: cannot set variable PWD",
		"pwd: cannot get current directory path",
		"Error reading a directory in PATH",
		"fd issue",
		"external command error",
		"fork failure",
		"wait failure"};

	(void)xe;
	ft_putstr_fd("error: ", STDERR_FILENO);
	ft_putstr_fd(err_msg[err_code - _ERRNO_MSG_ - 1], STDERR_FILENO);
	ft_putstr_fd(": ", STDERR_FILENO);
	ft_putendl_fd(strerror(errno), STDERR_FILENO);
	if (err_code == EXT_CMD_ERROR)
		return (ext_cmd_exit(xe));
	else if (err_code == FORK_ERROR || err_code == WAIT_ERROR)
		xe->stat_loc = 1;
	return (SUCCESS);
}
