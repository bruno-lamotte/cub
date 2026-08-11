/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: blamotte <blamotte@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/11 02:00:00 by blamotte          #+#    #+#             */
/*   Updated: 2026/08/11 02:00:00 by blamotte         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/cub.h"

bool	is_char_fmt(char c)
{
	return (ft_isalnum(c) || c == '_');
}

bool	is_space_fmt(char c)
{
	return (c == '\t' || c == ' ');
}

bool	skip_space_tab(char *s, t_index *i, bool expect_eol)
{
	while (s[i->col] && is_space_fmt(s[i->col]))
		i->col++;
	if (!expect_eol && (!s[i->col] || s[i->col] == '\n'))
		return (i->err = PERR_UNEXPECTED_EOL, false);
	return (true);
}

bool	skip_char_fmt(char *s, t_index *i)
{
	if (!s[i->col] || !is_char_fmt(s[i->col]))
		return (i->err = PERR_INVALID_CHAR, false);
	while (s[i->col] && is_char_fmt(s[i->col]))
		i->col++;
	if (s[i->col] && s[i->col] != '\n' && s[i->col] != ','
		&& !is_space_fmt(s[i->col]))
		return (i->err = PERR_INVALID_CHAR, false);
	return (true);
}

bool	skip_separator(char *s, t_index *i, char sep)
{
	if (!skip_space_tab(s, i, false))
		return (false);
	if (s[i->col] != sep)
		return (i->err = PERR_BAD_SEPARATOR, false);
	i->col++;
	if (!skip_space_tab(s, i, false))
		return (false);
	return (true);
}
