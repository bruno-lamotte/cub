/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils3.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: blamotte <blamotte@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/11 02:00:00 by blamotte          #+#    #+#             */
/*   Updated: 2026/08/11 02:00:00 by blamotte         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/cub.h"

bool	skip_file_path(char *s, t_index *i, char **dest)
{
	size_t	eol;

	eol = get_eol(&s[i->col]);
	if (!eol)
		return (i->err = PERR_UNEXPECTED_EOL, false);
	*dest = ft_strndup(&s[i->col], eol);
	if (!(*dest))
		return (i->err = PERR_MALLOC_ERR, false);
	i->col += eol;
	return (true);
}

bool	skip_tex_name(char *s, t_index *i, char **dest)
{
	size_t	start;

	start = i->col;
	if (!skip_char_fmt(s, i))
		return (false);
	*dest = ft_strndup(&s[start], i->col - start);
	if (!(*dest))
		return (i->err = PERR_MALLOC_ERR);
	return (true);
}

bool	skip_and_get_symbol(char *s, t_index *i, char *dest)
{
	skip_space_tab(s, i, false);
	if (!s[i->col] || s[i->col] == '\n')
		return (i->err = PERR_UNEXPECTED_EOL, false);
	*dest = s[i->col];
	i->col++;
	return (true);
}

ssize_t	find_tex_from_str(char *s, size_t n, t_parsing *p)
{
	t_texture_def	*tex;
	ssize_t			j;

	tex = p->data->textures_defs;
	j = 0;
	while (j < p->data->textures_len)
	{
		if (ft_strlen(tex[j].name) == n && !ft_strncmp(s, tex[j].name, n))
			return (j);
		j++;
	}
	return (-1);
}

bool	skip_uint16_value(char *s, t_index *i, uint16_t *out)
{
	uint32_t	val;
	uint32_t	digit;

	if (!s[i->col] || !ft_isdigit((unsigned char)s[i->col]))
		return (i->err = PERR_NO_NUMBER, false);
	val = 0;
	while (s[i->col] && ft_isdigit((unsigned char)s[i->col]))
	{
		digit = (uint32_t)(s[i->col] - '0');
		if (val > (UINT16_MAX - digit) / 10)
			return (i->err = PERR_TOO_LARGE_NUMBER, false);
		val = val * 10 + digit;
		i->col++;
	}
	*out = (uint16_t)val;
	return (true);
}
