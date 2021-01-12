#include "parsing.h"

int		parsing_error(int	err_code)
{
	const char		*err_msg[NB_PARSING_ERRORS] = {
		"=== no error (here for padding) ===",
		"No matching single quote",
		"No matching double quote",
		"Multiline inputs are currently not supported",
		"Redirection path invalid",
		"No redirection path specified",
		"Empty command before ; or |"};

	(void)err_msg;
	ft_putstr_fd("minishell: syntax error: ", STDERR_FILENO);
	ft_putstr_fd(err_msg[err_code], STDERR_FILENO);
	ft_putchar_fd('\n', STDERR_FILENO);
	return (PARSING_ERR); // define to remove
}

int		check_syntax(char *line)
{
	int		i;
	t_byte	flags;
	
	i = 0;
	flags = S_BEGIN;
	while (line[i])
	{
		if (flags & S_BACKSL)
		{
			flags -= S_BACKSL;
			i++;
		}
		else
		{
			if (line[i] == '\\')
			{
				if (!(flags & S_BACKSL))
					flags += S_BACKSL;
			}
			else if (line[i] == '\'')
			{
				if (!(flags & S_QUOTE))
					flags += S_QUOTE;
				else
					flags -= S_QUOTE;
			}
			else if (line[i] == '\"')
			{
				if (!(flags & S_DQUOTE))
					flags += S_DQUOTE;
				else
					flags -= S_DQUOTE;
			}
			else if (line[i] == '>' || line[i] == '<')
			{
				if (!(flags & S_REDIR))
					flags += S_REDIR;
			}
			else if (line[i] == ';' || line[i] == '|')
			{
				if (flags & S_REDIR)
					return (parsing_error(REDIR_PATH_MISSING));
				if (flags & S_BEGIN)
					return (parsing_error(EMPTY_CMD));
				else
					flags += S_BEGIN;
			//	if (!(flags & S_CMDSEP))
			//		flags += S_CMDSEP;
			}
			else
			{
				if (!(ft_isspace(line[i])))
				{
					if (flags & S_REDIR)
						flags -= S_REDIR;
				}
				if (flags & S_BEGIN)
					flags -= S_BEGIN;
			}
			i++;
		}
	}
	if (flags & S_BACKSL)
		return (parsing_error(ESCAPE_NL));
	if (flags & S_REDIR)
		return (parsing_error(REDIR_PATH_MISSING));
	if (flags & S_QUOTE)
		return (parsing_error(SQUOTE_MISSING));
	if (flags & S_DQUOTE)
		return (parsing_error(DQUOTE_MISSING));
	return (SUCCESS);
}
