#include "parsing.h"

int		parsing_error(int err_code, int *stat_loc)
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
	*stat_loc = 2;
	return (SUCCESS);
}

void	check_others(t_byte *flags, char c)
{
	if (!(ft_isspace(c)))
	{
		if (*flags & S_R_REDIR)
			*flags -= S_R_REDIR;
		if (*flags & S_L_REDIR)
			*flags -= S_L_REDIR;
		if (*flags & S_APPEND)
			*flags -= S_APPEND;
		if (*flags & S_EMPTY)
			*flags -= S_EMPTY;
		if (*flags & S_CMDSEP)
			*flags -= S_CMDSEP;
	}
}

int		check_redirs_and_cmdsep(t_byte *flags, char c)
{
	if (c == '>')
	{
		if ((*flags & S_R_REDIR))
		{
			*flags += S_APPEND;
			*flags -= S_R_REDIR;
		}
		else if (!(*flags & S_R_REDIR) && !(*flags & S_L_REDIR))
			*flags += S_R_REDIR;
		else
			return (REDIR_PATH_MISSING);
	}
	else if (c == '<')
	{
		if (!(*flags & S_L_REDIR) && !(*flags & S_R_REDIR))
			*flags += S_L_REDIR;
		else
			return (REDIR_PATH_MISSING);
	}
	else if (c == ';' || c == '|')
	{
		if (!(*flags & S_CMDSEP))
			*flags += S_CMDSEP;
		else
			return (EMPTY_CMD);
		if (*flags & S_R_REDIR || *flags & S_L_REDIR)
			return (REDIR_PATH_MISSING);
		if (*flags & S_EMPTY)
			return (EMPTY_CMD);
		else
			*flags += S_EMPTY;
	}
	else
		check_others(flags, c);
	return (SUCCESS);
}

int		check_bsl_and_quotes(t_byte *flags, char c)
{
	if (c == '\\')
	{
		if (!(*flags & S_BACKSL))
			*flags += S_BACKSL;
	}
	else if (c == '\'')
	{
		if (!(*flags & S_QUOTE))
			*flags += S_QUOTE;
		else
			*flags -= S_QUOTE;
	}
	else if (c == '\"')
	{
		if (!(*flags & S_DQUOTE))
			*flags += S_DQUOTE;
		else
			*flags -= S_DQUOTE;
	}
	else
		return (check_redirs_and_cmdsep(flags, c));
	return (SUCCESS);
}

int		final_checks(int flags)
{
	if (flags & S_BACKSL)
		return (ESCAPE_NL);
	if (flags & S_APPEND)
		return (REDIR_PATH_MISSING);
	if (flags & S_R_REDIR)
		return (REDIR_PATH_MISSING);
	if (flags & S_L_REDIR)
		return (REDIR_PATH_MISSING);
	if (flags & S_QUOTE)
		return (SQUOTE_MISSING);
	if (flags & S_DQUOTE)
		return (DQUOTE_MISSING);
	return (SUCCESS);
}

int		check_syntax(char *line)
{
	int		i;
	int		ret;
	t_byte	flags;

	i = 0;
	flags = S_EMPTY;
	while (line[i])
	{
		if (flags & S_BACKSL)
		{
			flags -= S_BACKSL;
			i++;
		}
		else
		{
			ret = check_bsl_and_quotes(&flags, line[i]);
			if (ret != SUCCESS)
				return (ret);
			i++;
		}
	}
	return (final_checks(flags));
}
