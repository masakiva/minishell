/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_error.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abenoit <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/02/11 18:41:13 by abenoit           #+#    #+#             */
/*   Updated: 2021/02/11 18:50:48 by abenoit          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "execution.h"
#include "parsing.h"

int				exit_trigger(int ret, t_xe *xe)
{
	if (!(xe->flags & EXIT_ABORT))
	{
		free_str_array(xe->exported);
		free_str_array(xe->env);
		free(xe);
		exit(ret);
	}
	else
	{
		xe->flags = RUN;
		return (SUCCESS);
	}
}

int				clean_and_exit(int err_code, t_xe *xe)
{
	int	ret;

	if (err_code == ARG_ERR)
	{
		ret = 1;
		ft_putstr_fd("minishell takes no argument\n", STDERR_FILENO);
	}
	else if (err_code == SUCCESS && !(xe->flags & EXIT_FLAG))
		ret = EXIT_SUCCESS;
	else
	{
		if (err_code == EXT_CMD_ERROR)
		{
			xe->flags += EXIT_ABORT;
			ft_putstr_fd("External function error: ", STDERR_FILENO);
			ft_putendl_fd(strerror(errno), STDERR_FILENO);
		}
		else if (err_code == EXIT_NAN)
			ft_putstr_fd("exit: bad argument\n", STDERR_FILENO);
		ret = (xe->stat_loc);
	}
	return (exit_trigger(ret, xe));
}

int				ft_error(int ret, t_xe *xe)
{
	if (xe->flags & EXIT_FLAG && !(xe->flags & CHILD))
		ft_putstr_fd("exit\n", STDERR_FILENO);
	if (ret > _ERRNO_MSG_)
		ft_putstr_fd("minishell: ", STDERR_FILENO);
	if (ret > _PARSING_ERROR_)
		return (parsing_error(ret, xe));
	else if (ret > _EXEC_ERROR_)
		return (exec_error(ret, xe));
	else if (ret > _ERRNO_MSG_)
		return (err_output(ret, xe));
	else if (ret > _EXIT_CODE_ || !(xe->flags & RUN))
		ret = clean_and_exit(ret, xe);
	else if (ret != SUCCESS)
		ft_putstr_fd("Fatal error", STDERR_FILENO);
	return (SUCCESS);
}

int				error_and_exit(enum e_retcode ret, t_xe *xe)
{
	ret = ft_error(ret, xe);
	return (exit_trigger(ret, xe));
}
