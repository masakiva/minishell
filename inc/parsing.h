/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mvidal-a <mvidal-a@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/02/11 16:48:07 by mvidal-a          #+#    #+#             */
/*   Updated: 2021/02/11 18:39:19 by abenoit          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PARSING_H
# define PARSING_H

# include "minishell.h"
# include "error.h"

# define BUF_SIZE	4096

# define S_EMPTY	0x001
# define S_BACKSL	0x002
# define S_QUOTE	0x004
# define S_DQUOTE	0x008
# define S_R_REDIR	0x010
# define S_L_REDIR	0x020
# define S_APPEND	0x040
# define S_SEMICOL	0x080
# define S_PIPE		0x100

# define METACHARS	"\\\'\"><;| "

enum	e_state
{
	SPACE,
	LETTER,
	BACKSLASH,
	DOLLAR,
	SINGLE_QUOTE,
	DOUBLE_QUOTE,
	QUOTED_BACKSLASH,
	QUOTED_DOLLAR,
	ANGLE_BRACKET,
	END,
	NB_STATES
};

typedef struct	s_state_machine
{
	char			buf[BUF_SIZE];
	size_t			len;
	char			**env;
	char			*cur_arg;
	char			**args;
	char			**redir_paths;
	char			***cur_token_stack;
	enum e_redir_op	*redir_types;
	size_t			var_token_count;
	enum e_state	state;
	int				stat_loc;
	t_byte			pipe_flag;
	t_byte			var_flag;
	t_byte			var_state;
	t_byte			quote_state;
	uint8_t			pad[4];
}				t_state_machine;

typedef char	*(*t_parse)(t_state_machine *, char *);
typedef int		(*t_syntax)(unsigned int *flags);

/*
**	check_syntax.c
*/

int				parsing_error(int err_code, t_xe *xe);
int				check_syntax(char *line);

/*
**	syntax_utils_1.c
*/

int				check_backsl(unsigned int *flags);
int				check_quote(unsigned int *flags);
int				check_dquote(unsigned int *flags);
int				check_r_redir(unsigned int *flags);
int				check_l_redir(unsigned int *flags);

/*
**	syntax_utils_2.c
*/

int				check_semicol(unsigned int *flags);
int				check_pipe(unsigned int *flags);
int				check_space(unsigned int *flags);
int				check_others(unsigned int *flags);

/*
**	main function
*/

t_command		*parse_one_command(t_xe *xe);

/*
**	parsing states
*/

char			*space(t_state_machine *machine, char *line);
char			*letter(t_state_machine *machine, char *line);
char			*backslash(t_state_machine *machine, char *line);
char			*dollar(t_state_machine *machine, char *line);
char			*single_quote(t_state_machine *machine, char *line);
char			*double_quote(t_state_machine *machine, char *line);
char			*quoted_backslash(t_state_machine *machine, char *line);
char			*quoted_dollar(t_state_machine *machine, char *line);
char			*angle_bracket(t_state_machine *machine, char *line);

/*
**	variables
*/

char			*parse_variable(t_state_machine *machine, char *line);
int				parse_exit_status(t_state_machine *machine);
char			*parse_quoted_variable(t_state_machine *machine, char *line);

/*
**	redirs
*/

char			*new_redir_info(t_state_machine *machine, char *line);

/*
**	utils
*/

int				add_to_buf(t_state_machine *machine, char c);
int				reset_buf(t_state_machine *machine);
int				add_arg(t_state_machine *machine);

#endif
