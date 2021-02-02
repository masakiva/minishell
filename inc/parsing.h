#ifndef PARSING_H
# define PARSING_H

# include "minishell.h"

# define BUF_SIZE		4096

# define S_EMPTY	0x01
# define S_BACKSL	0x02
# define S_QUOTE	0x04
# define S_DQUOTE	0x08
# define S_R_REDIR	0x10
# define S_L_REDIR	0x20
# define S_APPEND	0x40
# define S_CMDSEP	0x80

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

/*
**	check_syntax.c
*/
int				parsing_error(int err_code, t_xe *xe);
int				check_syntax(char *line);

/*
**	main functions
*/

t_command		*parse_one_command(t_xe *xe);
int				parse_input(char **line, char **env, int stat_loc,
		t_command *command);

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
