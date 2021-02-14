/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mvidal-a <mvidal-a@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/02/11 16:52:23 by mvidal-a          #+#    #+#             */
/*   Updated: 2021/02/11 23:34:03 by abenoit          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

/*
**	**********************************
**	**		GENERAL INCLUDES		**
**	**********************************
*/

# include <unistd.h>
# include <stdlib.h>
# include <errno.h>
# include <signal.h>

# include "libft.h"

/*
**	**********************************
**	**		GENERAL DEFINES			**
**	**********************************
*/

# define SUCCESS	1
# define FAILURE	0

# define TRUE		1
# define FALSE		0

# define FT_PS1			"\033[032m$: \033[0m"

# define SHLVL_STR		"SHLVL"

# define TO_SPLIT		1
# define NOT_TO_SPLIT	0

# define RUN			0x01
# define EXEC_PIPE		0x02
# define CMD_PIPE		0x04
# define CHILD			0x08
# define EXIT_ABORT		0x10
# define EXIT_FLAG		0x20
# define INTERUPT		0x40

/*
**	**********************************
**	**	GENERAL TYPES AND STRUCTS	**
**	**********************************
*/

enum			e_redir_op
{
	NO_REDIR,
	FILEIN,
	FILEOUT,
	APPEND,
	AMBIG
};

typedef struct	s_command
{
	char				**args;
	char				**redir_paths;
	enum e_redir_op		*redir_types;
	t_byte				pipe_flag;
	t_byte				var_flag;
	uint8_t				pad[6];
}				t_command;

typedef struct	s_xe
{
	pid_t	gpid;
	int		backup_stdin;
	int		backup_stdout;
	int		stat_loc;
	char	*line;
	char	**env;
	char	**exported;
	char	flags;
	uint8_t	pad[7];
}				t_xe;

/*
**	**********************************
**	**			PROTOTYPES			**
**	**********************************
*/

/*
**	signals
*/

void			signal_handler(void);

/*
**	env management
*/

int				env_replace_var(char *var_name, char *value, t_xe *xe);

/*
**	free utils
*/

void			free_command(void *content);
void			free_commands(t_list **commands);

#endif
