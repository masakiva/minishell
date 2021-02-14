/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mvidal-a <mvidal-a@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/02/11 16:49:21 by mvidal-a          #+#    #+#             */
/*   Updated: 2021/02/14 22:07:17 by abenoit          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef EXECUTION_H
# define EXECUTION_H

# include <sys/types.h>
# include <sys/wait.h>
# include <fcntl.h>
# include <dirent.h>

# include "minishell.h"
# include "error.h"

# define NOT_FOUND	-1
# define KEEP_ON	303

enum		e_cmd_code
{
	ECHO,
	CD,
	PWD,
	EXPORT,
	UNSET,
	ENV,
	EXIT,
	ELSE
};

typedef int	(*t_exec)(char **args, t_xe *xe);

/*
**	main function
*/

int			handle_exec_return(t_xe *xe, int fd_in, int proc);

int			handle_execution(t_xe *xe, int fd_in, int proc);

int			parent_pipe_end(t_command *cur_command, t_xe *xe,
									int fd_in, int proc);

int			execute_cmd(char **args, char **redir_paths,
						enum e_redir_op *redir_types, t_xe *xe);

int			apply_redirs(char **redir_paths,
							enum e_redir_op *redir_types, t_xe *xe);
int			find_ambig_redir(char **redir_paths, enum e_redir_op *redir_types);

/*
**	main function
*/

int			create_cmd(char **tmp, char **path, char **args);
char		**create_path_array(t_xe *xe);

#endif
