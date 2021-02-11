/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   syntax_utils_2.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abenoit <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/02/11 18:31:08 by abenoit           #+#    #+#             */
/*   Updated: 2021/02/11 18:35:43 by abenoit          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parsing.h"

int	check_semicol(unsigned int *flags)
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
	return (SUCCESS);
}

int	check_pipe(unsigned int *flags)
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
	return (SUCCESS);
}

int	check_space(unsigned int *flags)
{
	(void)*flags;
	return (SUCCESS);
}

int	check_others(unsigned int *flags)
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
	return (SUCCESS);
}
