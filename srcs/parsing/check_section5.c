/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   check_section5.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: blamotte <blamotte@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/11 02:00:00 by blamotte          #+#    #+#             */
/*   Updated: 2026/08/11 02:00:00 by blamotte         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/cub.h"

bool	check_object_line(t_parsing *p, size_t id)
{
	t_index			*i;
	char			*s;
	t_obj_def		*o;

	i = &p->idx;
	s = p->file_content[i->line];
	o = &p->data->obj_defs[id];
	o->pad = 1;
	if (!skip_and_get_symbol(s, i, &o->symbol))
		return (false);
	if (!skip_separator(s, i, ':'))
		return (false);
	if (!skip_percent_value(s, i, &o->size_pc))
		return (false);
	if (!sub_check_object_line(s, p, o))
		return (false);
	if (!parse_obj_flag(s, i, o))
		return (false);
	if (!skip_space_tab(s, i, true))
		return (false);
	return (true);
}

bool	check_object_section(t_parsing *p, size_t len)
{
	t_index	*i;
	size_t	y;

	p->data->obj_len = len;
	i = &p->idx;
	p->data->objs_section_id = i->line;
	p->data->obj_defs = ft_calloc(len, sizeof(t_obj_def));
	if (!p->data->obj_defs)
		return (i->err = PERR_MALLOC_ERR, false);
	y = 0;
	while (y < len)
	{
		p->idx.col = 0;
		if (!check_object_line(p, y))
			return (false);
		y++;
		i->line++;
	}
	return (true);
}

bool	sub_check_player_line(char *s, t_parsing *p, t_player_def *o)
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

bool	check_player_line(t_parsing *p, size_t id)
{
	t_index			*i;
	char			*s;
	t_player_def	*o;

	i = &p->idx;
	s = p->file_content[i->line];
	o = &p->data->player_defs[id];
	if (!skip_and_get_symbol(s, i, &o->symbol))
		return (false);
	if (!skip_separator(s, i, ':'))
		return (false);
	if (!s[i->col] || !ft_isset(s[i->col], "NWSE"))
		return (i->err = PERR_NO_DIR_FOUND, false);
	if (s[i->col] == 'N')
		o->dir = DIR_NORTH;
	else if (s[i->col] == 'W')
		o->dir = DIR_WEST;
	else if (s[i->col] == 'S')
		o->dir = DIR_SOUTH;
	else if (s[i->col] == 'E')
		o->dir = DIR_EAST;
	i->col++;
	if (!sub_check_player_line(s, p, o))
		return (false);
	return (skip_space_tab(s, i, true));
}

bool	check_player_section(t_parsing *p, size_t len)
{
	t_index	*i;
	size_t	y;

	p->data->player_len = len;
	i = &p->idx;
	p->data->players_section_id = i->line;
	p->data->player_defs = ft_calloc(len, sizeof(t_player_def));
	if (!p->data->player_defs)
		return (i->err = PERR_MALLOC_ERR, false);
	y = 0;
	while (y < len)
	{
		p->idx.col = 0;
		if (!check_player_line(p, y))
			return (false);
		y++;
		i->line++;
	}
	return (true);
}
