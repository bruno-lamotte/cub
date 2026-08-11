/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   check_section.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: blamotte <blamotte@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/11 02:00:00 by blamotte          #+#    #+#             */
/*   Updated: 2026/08/11 02:00:00 by blamotte         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/cub.h"

static void	free_tex_content(t_texture_def *t)
{
	if (!t->is_color)
	{
		if (t->path)
			free(t->path);
	}
	if (t->name)
		free(t->name);
}

bool	check_texture_line(t_parsing *p, size_t id, t_index *i, char *s)
{
	t_texture_def	*tex;

	i = &p->idx;
	s = p->file_content[i->line];
	tex = &p->data->textures_defs[id];
	skip_space_tab(s, i, false);
	if (!skip_tex_name(s, i, &tex->name))
		return (false);
	if (!skip_separator(s, i, ':'))
		return (free_tex_content(tex), false);
	if (skip_keyword(s, i, "HEXA"))
		tex->is_color = true;
	else if (skip_keyword(s, i, "PATH"))
		tex->is_color = false;
	else
		return (i->err = PERR_BAD_KEYWORD, free_tex_content(tex), false);
	if (!skip_separator(s, i, ','))
		return (free_tex_content(tex), false);
	if (tex->is_color == true && !skip_hex_color(s, i, &tex->color))
		return (free_tex_content(tex), false);
	else if (tex->is_color == false && !skip_file_path(s, i, &tex->path))
		return (free_tex_content(tex), false);
	return (skip_space_tab(s, i, true));
}

bool	check_texture_section(t_parsing *p, size_t len)
{
	t_index			*i;
	size_t			y;

	p->data->textures_len = len;
	i = &p->idx;
	p->data->textures_section_id = i->line;
	p->data->textures_defs = ft_calloc(sizeof(t_texture_def), len);
	if (!p->data->textures_defs)
		return (i->err = PERR_MALLOC_ERR, false);
	y = 0;
	while (y < len)
	{
		p->idx.col = 0;
		if (!check_texture_line(p, y, NULL, NULL))
			return (free_textures_def(&p->data->textures_defs, y), false);
		y++;
		i->line++;
	}
	return (true);
}

void	wall_text_attributor(t_wall_def *wall, ssize_t tex_id, uint8_t inc)
{
	if (inc == 0)
		wall->tex_north = (uint8_t)tex_id;
	else if (inc == 1)
		wall->tex_west = (uint8_t)tex_id;
	else if (inc == 2)
		wall->tex_south = (uint8_t)tex_id;
	else if (inc == 3)
		wall->tex_east = (uint8_t)tex_id;
}

bool	sub_check_wall_line(char *s, uint8_t inc, t_parsing *p, t_wall_def *w)
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
	wall_text_attributor(w, tex_id, inc);
	if (inc < 3)
	{
		if (!skip_separator(s, i, ','))
			return (false);
	}
	return (true);
}
