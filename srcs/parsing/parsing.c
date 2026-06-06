/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rstarcev <rstarcev@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/28 16:37:31 by rstarcev          #+#    #+#             */
/*   Updated: 2026/06/05 10:26:07 by rstarcev         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/cub.h"

bool	check_filename(char *filename)
{
	int	len;

	len = 0;
	if (!filename)
		return (false);
	len = ft_strlen(filename);
	if (len <= 4)
		return (false);
	if (ft_strcmp(filename + len - 4, ".cub"))
		return (false);
	return (true);
}

size_t	count_doors_rt(t_data *d)
{
	size_t	count;
	char	**map;
	size_t	i;
	size_t	y;

	map = d->m_map;
	count = 0;
	i = 0;
	while (i < d->m_height)
	{
		y = 0;
		while (map[i][y])
		{
			if (ft_isset(map[i][y], d->door_ids))
				count++;
			y++;
		}
		i++;
	}
	return (count);
}

size_t	count_monster_rt(t_data *d)
{
	size_t	count;
	char	**map;
	size_t	i;
	size_t	y;

	map = d->m_map;
	count = 0;
	i = 0;
	while (i < d->m_height)
	{
		y = 0;
		while (map[i][y])
		{
			if (ft_isset(map[i][y], d->monster_ids))
				count++;
			y++;
		}
		i++;
	}
	return (count);
}

bool	sub_parse_file(t_parsing *p)
{
	if (!check_s("WALLS\n", check_wall_section, p, 0))
		return (false);
	if (!check_s("AIRS\n", check_air_section, p, 0))
		return (false);
	if (!check_s("DOORS\n", check_door_section, p, 0))
		return (false);
	if (!check_s("MONSTERS\n", check_monster_section, p, 0))
		return (false);
	if (!check_s("OBJECTS\n", check_object_section, p, 0))
		return (false);
	if (!check_s("PLAYERS\n", check_player_section, p, 0))
		return (false);
	if (!check_s("MANDATORY_MAP\n", check_mandatory_map_section, p, 0))
		return (false);
	if (p->file_content[p->idx.line] && !ft_strcmp\
("BONUS_MAP\n", p->file_content[p->idx.line]))
		p->data->have_bonus = true;
	if (p->data->have_bonus && !check_s("BONUS_MAP\n", \
check_bonus_map_section, p, 0) && p->idx.err != PERR_NONE)
		return (false);
	return (true);
}

bool	parse_file(char *filename, t_data *d)
{
	t_parsing	p;

	if (!check_filename(filename))
		return (false);
	p.file_content = get_file(filename);
	if (!p.file_content)
		return (false);
	p.data = d;
	ft_bzero(&p.idx, sizeof(t_index));
	if (!check_s("TEXTURES\n", check_texture_section, &p, 0))
		return (free_str_tab(p.file_content), \
print_perr(&p.idx), false);
	if (!sub_parse_file(&p))
		return (free_str_tab(p.file_content), free_all_def(p.data), \
print_perr(&p.idx), false);
	if (!get_ids(&p))
		return (free_str_tab(p.file_content), free_ids(p.data), \
free_all_def(p.data), print_perr(&p.idx), false);
	if (!validate_map(d, &p.idx))
		return (free_str_tab(p.file_content), free_ids(p.data), \
free_all_def(p.data), print_perr(&p.idx), false);
	p.data->door_rt_count = (uint16_t)count_doors_rt(p.data);
	p.data->monster_rt_count = (uint16_t)count_monster_rt(p.data);
	free_str_tab(p.file_content);
	return (true);
}
