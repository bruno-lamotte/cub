/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   print_defs3.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: blamotte <blamotte@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/11 02:00:00 by blamotte          #+#    #+#             */
/*   Updated: 2026/08/11 02:00:00 by blamotte         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/cub.h"

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
		printf("invalid identifier\n");
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
