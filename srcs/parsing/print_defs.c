/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   print_defs.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rstarcev <rstarcev@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/28 14:53:05 by rstarcev          #+#    #+#             */
/*   Updated: 2026/05/28 16:36:54 by rstarcev         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/cub.h"

void	print_map_details(char id, t_data *d)
{
	if (id == 'm')
	{
		printf("m height = %u\n", d->m_height);
		printf("m width  = %u\n", d->m_max_width);
	}
	else if (id == 'b')
	{
		printf("b height = %u\n", d->b_height);
		printf("b width  = %u\n", d->b_max_width);
	}
}

void	print_textures_def(t_texture_def *tex, size_t len)
{
	size_t	i;

	i = 0;
	if (!tex)
		return ;
	while (i < len)
	{
		if (tex[i].name)
			printf("name = %15s | ", tex[i].name);
		if (!tex[i].is_color && tex[i].path)
			printf("path = %s\n", tex[i].path);
		else if (tex[i].is_color)
			printf("hexa = %#08x\n", tex[i].color);
		i++;
	}
	printf("------------------\n");
}

void	print_walls_def(t_wall_def *w, size_t len, t_texture_def *tex)
{
	size_t	i;

	i = 0;
	if (!w)
		return ;
	while (i < len)
	{
		printf("%c : %s, %s, %s, %s\n", w[i].symbol, tex[w[i].tex_north].name, \
tex[w[i].tex_west].name, tex[w[i].tex_south].name, tex[w[i].tex_east].name);
		i++;
	}
	printf("------------------\n");
}

void	print_airs_def(t_air_def *a, size_t len, t_texture_def *tex)
{
	size_t	i;

	i = 0;
	if (!a)
		return ;
	while (i < len)
	{
		printf("%c : %s, %s\n", a[i].symbol, tex[a[i].tex_floor].name, \
tex[a[i].tex_ceiling].name);
		i++;
	}
	printf("------------------\n");
}

void	print_doors_def(t_door_def *d, size_t len, t_texture_def *tex)
{
	size_t	i;

	i = 0;
	if (!d)
		return ;
	while (i < len)
	{
		printf("%c : %s, %s, %s\n", d[i].symbol, tex[d[i].tex_front].name, \
tex[d[i].tex_back].name, tex[d[i].tex_side].name);
		i++;
	}
	printf("------------------\n");
}

void	print_monsters_def(t_monster_def *m, size_t len, t_texture_def *tex)
{
	size_t	i;

	i = 0;
	if (!m)
		return ;
	while (i < len)
	{
		printf("%c : %u%%, %u hp, %s\n", m[i].symbol, m[i].size_pc, m[i].hp, \
tex[m[i].tex].name);
		i++;
	}
	printf("------------------\n");
}

void	print_objects_def(t_obj_def *o, size_t len, t_texture_def *tex)
{
	size_t	i;

	i = 0;
	if (!o)
		return ;
	while (i < len)
	{
		printf("%c : %u%%, %s\n", o[i].symbol, o[i].size_pc, \
tex[o[i].tex].name);
		i++;
	}
	printf("------------------\n");
}

char	get_dir(t_dir dir)
{
	if (dir == DIR_NORTH)
		return ('N');
	else if (dir == DIR_WEST)
		return ('W');
	else if (dir == DIR_SOUTH)
		return ('S');
	else if (dir == DIR_EAST)
		return ('E');
	else
		return ('X');
}

void	print_players_def(t_player_def *o, size_t len, t_texture_def *tex)
{
	size_t	i;

	i = 0;
	if (!o)
		return ;
	while (i < len)
	{
		printf("%c : dir = %c, tex = %s\n", o[i].symbol, get_dir(o[i].dir), \
tex[o[i].tex].name);
		i++;
	}
	printf("------------------\n");
}

void	print_line_exclude_sym(char *s, char *sym)
{
	char		buf[MAX_DEBUG_LINE + 1];
	uint16_t	i;

	i = 0;
	while (s[i] && i < MAX_DEBUG_LINE)
	{
		buf[i] = s[i];
		if (ft_isset(buf[i], sym))
			buf[i] = ' ';
		i++;
	}
	buf[i] = '\0';
	printf("%s\n", buf);
}

void	print_map_exclude_sym(char **map, char *sym)
{
	uint16_t	i;

	i = 0;
	while (map[i])
	{
		print_line_exclude_sym(map[i], sym);
		i++;
	}
}

void	print_defs_ids(t_data *d)
{
	printf("IDs -----------------------\n");
	printf("all  = %s\n", d->all_ids);
	printf("wall = %s\n", d->wall_ids);
	printf("air  = %s\n", d->air_ids);
	printf("door = %s\n", d->door_ids);
	printf("mons = %s\n", d->monster_ids);
	printf("obj  = %s\n", d->obj_ids);
	printf("play = %s\n", d->player_ids);
}

void	print_parsing(t_data *d)
{
	print_textures_def(d->textures_defs, d->textures_len);
	print_walls_def(d->walls_defs, d->walls_len, d->textures_defs);
	print_airs_def(d->airs_defs, d->airs_len, d->textures_defs);
	print_doors_def(d->doors_defs, d->doors_len, d->textures_defs);
	print_monsters_def(d->monsters_defs, d->monsters_len, d->textures_defs);
	print_objects_def(d->obj_defs, d->obj_len, d->textures_defs);
	print_players_def(d->player_defs, d->player_len, d->textures_defs);
	if (!d->have_bonus)
	{
		print_str_tab(d->m_map);
		printf("-------------\n");
		print_map_details('m', d);
	}
	else
	{
		print_map_exclude_sym(d->m_map, d->air_ids);
		printf("-------------\n");
		print_map_details('m', d);
		printf("-------------\n");
		print_str_tab(d->b_map);
		printf("-------------\n");
		print_map_details('b', d);
	}
	print_defs_ids(d);
}

bool	print_perr1(t_perr err)
{
	if (err == PERR_SEGMENT_TOO_LONG)
		printf("segment too long\n");
	else if (err == PERR_INVALID_CHAR)
		printf("invalid character\n");
	else if (err == PERR_UNFINISHED_LINE)
		printf("unfinished line\n");
	else if (err == PERR_INVALID_ID)
		printf("invalid identelse ifier\n");
	else if (err == PERR_SECTION_TITLE)
		printf("invalid section title\n");
	else if (err == PERR_OPEN_BRACE)
		printf("missing opening brace\n");
	else if (err == PERR_CLOSE_BRACE)
		printf("missing closing brace\n");
	else if (err == PERR_NO_CONTENT)
		printf("no content in section\n");
	else if (err == PERR_MAX_CONTENT)
		printf("too many elements in section\n");
	else if (err == PERR_UNEXPECTED_EOF)
		printf("unexpected end of file\n");
	else if (err == PERR_UNEXPECTED_EOL)
		printf("unexpected end of line\n");
	else
		return (false);
	return (true);
}

bool	print_perr2(t_perr err)
{
	if (err == PERR_UNEXPECTED_REST_OF_LINE)
		printf("unexpected characters at end of line\n");
	else if (err == PERR_INVALID_HEX_COLOR)
		printf("invalid hex color\n");
	else if (err == PERR_BAD_SEPARATOR)
		printf("invalid separator\n");
	else if (err == PERR_BAD_KEYWORD)
		printf("invalid keyword\n");
	else if (err == PERR_INVALID_LINE_FORMAT)
		printf("invalid line format\n");
	else if (err == PERR_NO_BOOL_FOUND)
		printf("boolean value not found\n");
	else if (err == PERR_NO_DIR_FOUND)
		printf("direction not found\n");
	else if (err == INVALIDE_TEXT_TYPE)
		printf("invalid texture type\n");
	else if (err == PERR_INVALID_PATH)
		printf("invalid path\n");
	else if (err == PERR_TEXT_NOT_RECOGNIZED)
		printf("texture not recognized\n");
	else if (err == PERR_PLAYER_NOT_FOUND)
		printf("player not found\n");
	else
		return (false);
	return (true);
}

void	print_perr(t_index *i)
{
	if (i->err == PERR_NONE)
		return ;
	printf("Error on line %zd col %zd : ", i->line + 1, i->col);
	if (print_perr1(i->err))
		return ;
	if (print_perr2(i->err))
		return ;
	if (i->err == PERR_UNEVEN_MAPS)
		printf("maps are not aligned\n");
	else if (i->err == PERR_OVERFLOW)
		printf("overflow detected\n");
	else if (i->err == PERR_BAD_HEX_PREFIX)
		printf("invalid hex prefix\n");
	else if (i->err == PERR_BAD_HEX_COLOR_LEN)
		printf("invalid hex color length\n");
	else if (i->err == PERR_BAD_FILEPATH)
		printf("invalid file path\n");
	else if (i->err == PERR_MALLOC_ERR)
		printf("malloc failed\n");
	else
		printf("unknown error\n");
}
