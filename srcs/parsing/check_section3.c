/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   check_section3.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: blamotte <blamotte@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/11 02:00:00 by blamotte          #+#    #+#             */
/*   Updated: 2026/08/11 02:00:00 by blamotte         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/cub.h"

bool	check_air_section(t_parsing *p, size_t len)
{
	t_index	*i;
	size_t	y;

	p->data->airs_len = len;
	i = &p->idx;
	p->data->airs_section_id = i->line;
	p->data->airs_defs = ft_calloc(len, sizeof(t_air_def));
	if (!p->data->airs_defs)
		return (i->err = PERR_MALLOC_ERR, false);
	y = 0;
	while (y < len)
	{
		p->idx.col = 0;
		if (!check_air_line(p, y))
			return (false);
		y++;
		i->line++;
	}
	return (true);
}

void	door_text_attributor(t_door_def *d, ssize_t tex_id, uint8_t inc)
{
	if (inc == 0)
		d->tex_front = (uint8_t)tex_id;
	else if (inc == 1)
		d->tex_back = (uint8_t)tex_id;
	else if (inc == 2)
		d->tex_side = (uint8_t)tex_id;
}

bool	sub_check_door_line(char *s, uint8_t inc, t_parsing *p, t_door_def *d)
{
	size_t	start;
	ssize_t	tex_id;
	t_index	*i;

	i = &p->idx;
	if (!skip_separator(s, i, ','))
		return (false);
	start = i->col;
	if (!skip_char_fmt(s, i))
		return (false);
	tex_id = find_tex_from_str(&s[start], i->col - start, p);
	if (tex_id < 0)
		return (i->err = PERR_NO_TEX_MATCH, false);
	door_text_attributor(d, tex_id, inc);
	return (true);
}

bool	check_door_line(t_parsing *p, size_t id)
{
	t_index			*i;
	char			*s;
	t_door_def		*door;
	uint8_t			inc;
	char			b_val;

	i = &p->idx;
	s = p->file_content[i->line];
	door = &p->data->doors_defs[id];
	inc = -1;
	if (!skip_and_get_symbol(s, i, &door->symbol))
		return (false);
	if (!skip_separator(s, i, ':'))
		return (false);
	if (!skip_and_get_symbol(s, i, &b_val))
		return (false);
	if (b_val != '1' && b_val != '0')
		return (i->err = PERR_NO_BOOL_FOUND, false);
	door->is_closed = b_val - '0';
	while (++inc < 3)
	{
		if (!sub_check_door_line(s, inc, p, door))
			return (false);
	}
	return (skip_space_tab(s, i, true));
}

bool	check_door_section(t_parsing *p, size_t len)
{
	t_index	*i;
	size_t	y;

	p->data->doors_len = len;
	i = &p->idx;
	p->data->doors_section_id = i->line;
	p->data->doors_defs = ft_calloc(len, sizeof(t_door_def));
	if (!p->data->doors_defs)
		return (i->err = PERR_MALLOC_ERR, false);
	y = 0;
	while (y < len)
	{
		p->idx.col = 0;
		if (!check_door_line(p, y))
			return (false);
		y++;
		i->line++;
	}
	return (true);
}
