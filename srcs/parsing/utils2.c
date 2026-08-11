/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils2.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: blamotte <blamotte@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/11 02:00:00 by blamotte          #+#    #+#             */
/*   Updated: 2026/08/11 02:00:00 by blamotte         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/cub.h"

bool	skip_keyword(char *s, t_index *i, char *keyword)
{
	size_t	len;

	len = ft_strlen(keyword);
	if (ft_strncmp(keyword, &s[i->col], len))
		return (false);
	i->col += len;
	return (true);
}

bool	is_hex_digit(char c)
{
	return ((c >= '0' && c <= '9')
		|| (c >= 'a' && c <= 'f')
		|| (c >= 'A' && c <= 'F'));
}

bool	isalpha_up(int c)
{
	if (c >= 'A' && c <= 'F')
		return (1);
	return (0);
}

bool	skip_hex_color(char *s, t_index *i, int32_t *dest)
{
	size_t	start;
	char	old;

	start = i->col;
	if (!s[i->col] || s[i->col] != '0')
		return (i->err = PERR_BAD_HEX_PREFIX, false);
	i->col++;
	if (!s[i->col] || (s[i->col] != 'x' && s[i->col] != 'X'))
		return (i->err = PERR_BAD_HEX_PREFIX, false);
	i->col++;
	while (is_hex_digit(s[i->col]) && (i->col - start) < HEX_COLOR_LEN)
	{
		if (s[i->col] >= 'A' && s[i->col] <= 'F')
			s[i->col] = (char)ft_tolower((char)s[i->col]);
		i->col++;
	}
	if ((i->col - start) != HEX_COLOR_LEN)
		return (i->err = PERR_BAD_HEX_COLOR_LEN, false);
	old = s[i->col];
	s[i->col] = '\0';
	*dest = (int32_t)ft_atoi_base(&s[start + 2], "0123456789abcdef");
	s[i->col] = old;
	return (true);
}

size_t	get_eol(char *s)
{
	size_t	i;

	i = 0;
	while (s[i] && s[i] != '\n')
		i++;
	if (!s[i])
		return (0);
	return (i);
}
