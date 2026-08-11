/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   check_section4.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: blamotte <blamotte@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/11 02:00:00 by blamotte          #+#    #+#             */
/*   Updated: 2026/08/11 02:00:00 by blamotte         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/cub.h"

bool	sub_check_monster_line(char *s, t_parsing *p, t_monster_def *m)
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
	m->tex = (uint8_t)tex_id;
	return (true);
}

bool	check_monster_line(t_parsing *p, size_t id)
{
	t_index			*i;
	char			*s;
	t_monster_def	*m;

	i = &p->idx;
	s = p->file_content[i->line];
	m = &p->data->monsters_defs[id];
	if (!skip_and_get_symbol(s, i, &m->symbol))
		return (false);
	if (!skip_separator(s, i, ':'))
		return (false);
	if (!skip_percent_value(s, i, &m->size_pc))
		return (false);
	if (!skip_separator(s, i, ','))
		return (false);
	if (!skip_uint8_value(s, i, &m->hp))
		return (false);
	if (!sub_check_monster_line(s, p, m))
		return (false);
	return (skip_space_tab(s, i, true));
}

bool	check_monster_section(t_parsing *p, size_t len)
{
	t_index	*i;
	size_t	y;

	p->data->monsters_len = len;
	i = &p->idx;
	p->data->monsters_section_id = i->line;
	p->data->monsters_defs = ft_calloc(len, sizeof(t_monster_def));
	if (!p->data->monsters_defs)
		return (i->err = PERR_MALLOC_ERR, false);
	y = 0;
	while (y < len)
	{
		p->idx.col = 0;
		if (!check_monster_line(p, y))
			return (false);
		y++;
		i->line++;
	}
	return (true);
}

bool	sub_check_object_line(char *s, t_parsing *p, t_obj_def *o)
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
	o->tex = (uint8_t)tex_id;
	return (true);
}

bool	parse_obj_flag(char *s, t_index *i, t_obj_def *o)
{
	if (s[i->col] == ',')
	{
		i->col++;
		while (s[i->col] == ' ' || s[i->col] == '\t')
			i->col++;
		if (ft_strncmp(&s[i->col], "ON", 2) == 0)
		{
			o->pad = 1;
			i->col += 2;
		}
		else if (ft_strncmp(&s[i->col], "OFF", 3) == 0)
		{
			o->pad = 0;
			i->col += 3;
		}
		else
			return (i->err = PERR_BAD_KEYWORD, false);
	}
	return (true);
}
