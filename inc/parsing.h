#ifndef PARSING_H
# define PARSING_H

# include "minishell.h"

# define BUF_SIZE		4096

enum e_state
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

enum e_parsing_error
{
	NO_ERROR,
	SQUOTE_MISSING,
	DQUOTE_MISSING,
	ESCAPE_NL,
	REDIR_PATH_INVALID,
	REDIR_PATH_MISSING,
	EMPTY_CMD,
	NB_PARSING_ERRORS
};

typedef struct		s_state_machine
{
	char			buf[BUF_SIZE];
	size_t			len;
	char			**env;
	char			*cur_arg;
	char			**args;
	char			**redir_paths;
	char			***cur_token_stack;
	enum e_redir_op	*redir_types;
	enum e_state	state;
	int				stat_loc;
	t_byte			pipe_flag;
	uint8_t			pad[7];
}					t_state_machine;

typedef char	*(*t_parse)(t_state_machine *, char *);

/*
**	main function
*/

char	**parse_one_command(char **line, char **env, int stat_loc, t_command *command);
int		parse_commands(t_xe *xe, char *line);

/*
**	utils
*/

int		new_command(t_list **commands);
char	*new_redir_info(t_state_machine *machine, char *line);
int		add_variable(t_list **var_list, size_t start, size_t len, t_byte split_flag);
int		parse_exit_status(t_state_machine *machine);
char	*parse_quoted_variable(t_state_machine *machine, char *line);
char	*parse_variable(t_state_machine *machine, char *line);
int		add_arg(t_state_machine *machine);
int		reset_buf(t_state_machine *machine);
int		add_to_buf(t_state_machine *machine, char c);

#endif
