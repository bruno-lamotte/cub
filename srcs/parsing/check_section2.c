/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   check_section2.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: blamotte <blamotte@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/11 02:00:00 by blamotte          #+#    #+#             */
/*   Updated: 2026/08/11 02:00:00 by blamotte         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/cub.h"

bool	check_wall_line(t_parsing *p, size_t id)
{
	t_index			*i;
	char			*s;
	t_wall_def		*wall;
	uint8_t			inc;

	i = &p->idx;
	s = p->file_content[i->line];
	wall = &p->data->walls_defs[id];
	inc = 0;
	if (!skip_and_get_symbol(s, i, &wall->symbol))
		return (false);
	if (!skip_separator(s, i, ':'))
		return (false);
	while (inc < 4)
	{
		if (!sub_check_wall_line(s, inc, p, wall))
			return (false);
		inc++;
	}
	return (skip_space_tab(s, i, true));
}

bool	check_wall_section(t_parsing *p, size_t len)
{
	t_index	*i;
	size_t	y;

	p->data->walls_len = len;
	i = &p->idx;
	p->data->walls_section_id = i->line;
	p->data->walls_defs = ft_calloc(len, sizeof(t_wall_def));
	if (!p->data->walls_defs)
		return (i->err = PERR_MALLOC_ERR, false);
	y = 0;
	while (y < len)
	{
		p->idx.col = 0;
		if (!check_wall_line(p, y))
			return (false);
		y++;
		i->line++;
	}
	return (true);
}

void	air_text_attributor(t_air_def *wall, ssize_t tex_id, uint8_t inc)
{
	if (inc == 0)
		wall->tex_floor = (uint8_t)tex_id;
	else if (inc == 1)
		wall->tex_ceiling = (uint8_t)tex_id;
}

bool	sub_check_air_line(char *s, uint8_t inc, t_parsing *p, t_air_def *a)
{
	size_t	start;
	ssize_t	tex_id;
	t_index	*i;

	i = &p->idx;
	start = i->col;
	if (!skip_char_fmt(s, i))
		return (false);
	tex_id = find_tex_from_str(&s[start], i->col - start, p);
	if (tex_id < 0)
		return (i->err = PERR_NO_TEX_MATCH, false);
	air_text_attributor(a, tex_id, inc);
	if (inc < 1)
	{
		if (!skip_separator(s, i, ','))
			return (false);
	}
	return (true);
}

bool	check_air_line(t_parsing *p, size_t id)
{
	t_index			*i;
	char			*s;
	t_air_def		*air;
	uint8_t			inc;

	i = &p->idx;
	s = p->file_content[i->line];
	air = &p->data->airs_defs[id];
	inc = 0;
	if (!skip_and_get_symbol(s, i, &air->symbol))
		return (false);
	if (!skip_separator(s, i, ':'))
		return (false);
	while (inc < 2)
	{
		if (!sub_check_air_line(s, inc, p, air))
			return (false);
		inc++;
	}
	return (skip_space_tab(s, i, true));
}
