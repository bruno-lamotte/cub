/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   check_section.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rstarcev <rstarcev@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/27 17:24:55 by rstarcev          #+#    #+#             */
/*   Updated: 2026/05/27 18:11:41 by rstarcev         ###   ########.fr       */
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
	if (!skip_space_tab(s, i, true))
		return (free_tex_content(tex), false);
	return (true);
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

/*WALL*/

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
	if (!skip_space_tab(s, i, true))
		return (false);
	return (true);
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

/*AIR*/

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
	if (!skip_space_tab(s, i, true))
		return (false);
	return (true);
}

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

/*DOORS*/

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

/*MONSTER*/
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
	if (!skip_space_tab(s, i, true))
		return (false);
	return (true);
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

/*OBJECTS*/
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

static bool	parse_obj_flag(char *s, t_index *i, t_obj_def *o)
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

/*PLAYER DEF*/
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

/*MAP*/
size_t	ft_strlen_no_right_space(char *s)
{
	bool	in_space;
	size_t	last_end;
	size_t	i;

	i = 0;
	last_end = 0;
	in_space = true;
	while (s && s[i])
	{
		if (in_space && !ft_isspace(s[i]))
			in_space = false;
		else if (!in_space && ft_isspace(s[i]))
		{
			last_end = i;
			in_space = true;
		}
		i++;
	}
	if (in_space)
		i = last_end;
	return (i);
}

char	**dup_map_and_trim_right_space(char **map, size_t len)
{
	char	**out;
	size_t	i;

	i = 0;
	out = malloc(sizeof(char *) * (len + 1));
	if (!out)
		return (NULL);
	while (i < len)
	{
		out[i] = ft_strndup(map[i], ft_strlen_no_right_space(map[i]));
		if (!out[i])
			return (free_str_tab(out), NULL);
		i++;
	}
	out[i] = NULL;
	return (out);
}

size_t	get_map_max_widht(char **map)
{
	size_t	i;
	size_t	max;
	size_t	res;

	max = 0;
	i = 0;
	while (map[i])
	{
		res = ft_strlen(map[i]);
		if (res > max)
			max = res;
		i++;
	}
	return (max);
}

bool	check_mandatory_map_section(t_parsing *p, size_t len)
{
	char	**map;
	t_index	*i;

	i = &p->idx;
	map = dup_map_and_trim_right_space(&p->file_content[i->line], len);
	if (!map)
		return (i->err = PERR_MALLOC_ERR, false);
	p->data->m_max_width = get_map_max_widht(map);
	p->data->m_map_section_id = i->line;
	p->data->m_height = len;
	if (p->data->m_max_width > MAX_MAP_WIDTH || len > MAX_MAP_HEIGHT)
		return (i->err = PERR_MAP_SIZE_TOO_LARGE, free_str_tab(map), false);
	p->data->m_map = map;
	i->line += len;
	return (true);
}

bool	check_bonus_map_section(t_parsing *p, size_t len)
{
	char	**map;
	t_index	*i;

	i = &p->idx;
	map = dup_map_and_trim_right_space(&p->file_content[i->line], len);
	if (!map)
		return (i->err = PERR_MALLOC_ERR, false);
	p->data->b_max_width = get_map_max_widht(map);
	p->data->b_map_section_id = i->line;
	p->data->b_height = len;
	if (p->data->b_max_width > MAX_MAP_WIDTH || len > MAX_MAP_HEIGHT)
		return (i->err = PERR_MAP_SIZE_TOO_LARGE, false);
	p->data->b_map = map;
	i->line += len;
	return (true);
}

/*MAIN FUNC*/
bool	check_s(char *n, bool (*f)(t_parsing *, size_t), t_parsing *p, size_t l)
{
	char	**file;
	t_index	*i;

	file = p->file_content;
	i = &p->idx;
	if (!file[i->line] || ft_strcmp(n, file[i->line++]))
		return (i->err = PERR_SECTION_TITLE, false);
	if (!file[i->line] || ft_strcmp("{\n", file[i->line++]))
		return (i->err = PERR_OPEN_BRACE, false);
	while (file[i->line])
	{
		if (!ft_strcmp("}\n", file[i->line]) || !i->line++)
			break ;
		l++;
	}
	if (!file[i->line] || ft_strcmp("}\n", file[i->line]))
		return (i->err = PERR_CLOSE_BRACE, false);
	if (!l)
		return (i->err = PERR_NO_CONTENT, false);
	if (l > MAX_SECTION_ITEMS)
		return (i->err = PERR_MAX_CONTENT, false);
	i->line -= l;
	if (!f(p, l))
		return (false);
	return (i->line++, true);
}
