#include "../../includes/cub.h"

void print_map_details(char id, t_data *d)
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

void print_textures_def(t_texture_def *tex, size_t len)
{
    size_t i;

    i = 0;
    if (!tex)
        return ;
    while(i < len)
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

void print_walls_def(t_wall_def *w, size_t len, t_texture_def *tex)
{
	size_t i;

	i = 0;
	if (!w)
		return ;
	while (i < len)
	{
		printf("%c : %s, %s, %s, %s\n", w[i].symbol, tex[w[i].tex_north].name, tex[w[i].tex_west].name, \
			tex[w[i].tex_south].name, tex[w[i].tex_east].name);
		i++;
	}
	printf("------------------\n");
}

void print_airs_def(t_air_def *a, size_t len, t_texture_def *tex)
{
	size_t i;

	i = 0;
	if (!a)
		return ;
	while (i < len)
	{
		printf("%c : %s, %s\n", a[i].symbol, tex[a[i].tex_floor].name, tex[a[i].tex_ceiling].name);
		i++;
	}
	printf("------------------\n");
}

void print_doors_def(t_door_def *d, size_t len, t_texture_def *tex)
{
	size_t i;

	i = 0;
	if (!d)
		return ;
	while (i < len)
	{
		printf("%c : %s, %s, %s\n", d[i].symbol, tex[d[i].tex_front].name, tex[d[i].tex_back].name, \
			tex[d[i].tex_side].name);
		i++;
	}
	printf("------------------\n");
}

void print_monsters_def(t_monster_def *m, size_t len, t_texture_def *tex)
{
	size_t i;

	i = 0;
	if (!m)
		return ;
	while (i < len)
	{
		printf("%c : %u%%, %u hp, %s\n", m[i].symbol, m[i].size_pc, m[i].hp, tex[m[i].tex].name);
		i++;
	}
	printf("------------------\n");
}

void print_objects_def(t_obj_def *o, size_t len, t_texture_def *tex)
{
	size_t i;

	i = 0;
	if (!o)
		return ;
	while (i < len)
	{
		printf("%c : %u%%, %s\n", o[i].symbol, o[i].size_pc, tex[o[i].tex].name);
		i++;
	}
	printf("------------------\n");
}

char get_dir(t_dir dir)
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

void print_players_def(t_player_def *o, size_t len, t_texture_def *tex)
{
	size_t i;

	i = 0;
	if (!o)
		return ;
	while (i < len)
	{
		printf("%c : dir = %c, tex = %s\n", o[i].symbol, get_dir(o[i].dir), tex[o[i].tex].name);
		i++;
	}
	printf("------------------\n");
}

void print_line_exclude_sym(char *s, char *sym)
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

void print_map_exclude_sym(char **map, char *sym)
{
	uint16_t i;

	i = 0;
	while (map[i])
	{
		print_line_exclude_sym(map[i], sym);
		i++;
	}

}

void print_defs_ids(t_data *d)
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

void print_parsing(t_data *d)
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
		printf("-------------\n");
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
		printf("-------------\n");
	}
	print_defs_ids(d);
}

void	print_perr(t_index *i)
{
    t_perr err;
    err = i->err;

	if (err == PERR_NONE)
		return ;

	printf("Error on line %zd col %zd : ", i->line + 1, i->col);

	switch (err)
	{
		case PERR_OVERFLOW:
			printf("overflow detected\n");
			break;
		case PERR_SEGMENT_TOO_LONG:
			printf("segment too long\n");
			break;
		case PERR_INVALID_CHAR:
			printf("invalid character\n");
			break;
		case PERR_UNFINISHED_LINE:
			printf("unfinished line\n");
			break;
		case PERR_INVALID_ID:
			printf("invalid identifier\n");
			break;
		case PERR_SECTION_TITLE:
			printf("invalid section title\n");
			break;
		case PERR_OPEN_BRACE:
			printf("missing opening brace\n");
			break;
		case PERR_CLOSE_BRACE:
			printf("missing closing brace\n");
			break;
		case PERR_NO_CONTENT:
			printf("no content in section\n");
			break;
		case PERR_MAX_CONTENT:
			printf("too many elements in section\n");
			break;
		case PERR_UNEXPECTED_EOF:
			printf("unexpected end of file\n");
			break;
		case PERR_UNEXPECTED_EOL:
			printf("unexpected end of line\n");
			break;
		case PERR_UNEXPECTED_REST_OF_LINE:
			printf("unexpected characters at end of line\n");
			break;
		case PERR_INVALID_HEX_COLOR:
			printf("invalid hex color\n");
			break;
		case PERR_BAD_SEPARATOR:
			printf("invalid separator\n");
			break;
		case PERR_BAD_KEYWORD:
			printf("invalid keyword\n");
			break;
		case PERR_INVALID_LINE_FORMAT:
			printf("invalid line format\n");
			break;
		case PERR_NO_BOOL_FOUND:
			printf("boolean value not found\n");
			break;
		case PERR_NO_DIR_FOUND:
			printf("direction not found\n");
			break;
		case INVALIDE_TEXT_TYPE:
			printf("invalid texture type\n");
			break;
		case PERR_INVALID_PATH:
			printf("invalid path\n");
			break;
		case PERR_TEXT_NOT_RECOGNIZED:
			printf("texture not recognized\n");
			break;
		case PERR_PLAYER_NOT_FOUND:
			printf("player not found\n");
			break;
		case PERR_UNEVEN_MAPS:
			printf("maps are not aligned\n");
			break;
		case PERR_BAD_HEX_PREFIX:
			printf("invalid hex prefix\n");
			break;
		case PERR_BAD_HEX_COLOR_LEN:
			printf("invalid hex color length\n");
			break;
		case PERR_BAD_FILEPATH:
			printf("invalid file path\n");
			break;
		case PERR_MALLOC_ERR:
			printf("malloc failed\n");
			break;
		default:
			printf("unknown error\n");
			break;
	}
}