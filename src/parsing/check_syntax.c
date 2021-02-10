/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   check_syntax.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mvidal-a <mvidal-a@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/02/01 11:25:20 by mvidal-a          #+#    #+#             */
/*   Updated: 2021/02/02 15:06:21 by abenoit          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parsing.h"

void	check_others(unsigned int *flags, char c)
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
		if (*flags & S_PIPE)
			*flags -= S_PIPE;
		if (*flags & S_SEMICOL)
			*flags -= S_SEMICOL;
	}
}

int		check_redirs_and_cmdsep(unsigned int *flags, char c)
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
	else if (c == ';')
	{
		if (!(*flags & S_SEMICOL) && !(*flags & S_PIPE))
			*flags += S_SEMICOL;
		else
			return (EMPTY_CMD);
		if (*flags & S_R_REDIR || *flags & S_L_REDIR)
			return (REDIR_PATH_MISSING);
		if (*flags & S_EMPTY)
			return (EMPTY_CMD);
		else
			*flags += S_EMPTY;
	}
	else if (c == '|')
	{
		if (!(*flags & S_SEMICOL) && !(*flags & S_PIPE))
			*flags += S_PIPE;
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

int		check_bsl_and_quotes(unsigned int *flags, char c)
{
	if (c == '\\')
	{
		if (*flags & S_QUOTE)
			return (SUCCESS);
		if (!(*flags & S_BACKSL))
			*flags += S_BACKSL;
	}
	else if (c == '\'')
	{
		if (*flags & S_DQUOTE)
			return (SUCCESS);
		else if (!(*flags & S_QUOTE))
			*flags += S_QUOTE;
		else if (*flags & S_R_REDIR)
			*flags -= S_R_REDIR;
		else if (*flags & S_L_REDIR)
			*flags -= S_L_REDIR;
		else if (*flags & S_EMPTY)
			*flags -= S_EMPTY;
		else
			*flags -= S_QUOTE;
	}
	else if (c == '\"')
	{
		if (*flags & S_QUOTE)
			return (SUCCESS);
		else if (!(*flags & S_DQUOTE))
			*flags += S_DQUOTE;
		else if (*flags & S_R_REDIR)
			*flags -= S_R_REDIR;
		else if (*flags & S_L_REDIR)
			*flags -= S_L_REDIR;
		else if (*flags & S_EMPTY)
			*flags -= S_EMPTY;
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
	if (flags & S_PIPE)
		return (ESCAPE_NL);
	return (SUCCESS);
}

int		check_syntax(char *line)
{
	int				i;
	int				ret;
	unsigned int	flags;

	i = 0;
	flags = S_EMPTY;
	while (line[i])
	{
		if (flags & S_BACKSL)
		{
			flags -= S_BACKSL;
			if (flags & S_EMPTY)
				flags -= S_EMPTY;
			if (flags & S_APPEND)
				flags -= S_APPEND;
			if (flags & S_R_REDIR)
				flags -= S_R_REDIR;
			if (flags & S_L_REDIR)
				flags -= S_L_REDIR;
			i++;
		}
		else if (flags & S_QUOTE && line[i] == '\'')
		{
			flags -= S_QUOTE;
			i++;
		}
		else if (flags & S_DQUOTE && (line[i] == '\'' || line[i] == '\"'))
		{
			if (line[i] == '\"')
				flags -= S_DQUOTE;
			if (line[i] == '\\')
				flags += S_BACKSL;
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
