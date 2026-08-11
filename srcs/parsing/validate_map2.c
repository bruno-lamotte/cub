/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   validate_map2.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: blamotte <blamotte@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/11 02:00:00 by blamotte          #+#    #+#             */
/*   Updated: 2026/08/11 02:00:00 by blamotte         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/cub.h"

bool	set_error_tmp_map(t_tmp_map_data *f, ssize_t r, ssize_t c, t_perr err)
{
	printf("FLOOD ERROR: row %zd col %zd\n", r, c);
	if (r < 0)
		r = 0;
	if (c < 0)
		c = 0;
	f->r = (size_t)r;
	f->c = (size_t)c;
	f->err = err;
	return (false);
}

bool	flood_fill(t_tmp_map_data *f, ssize_t r, ssize_t c)
{
	char	**map;
	bool	**mask;

	map = f->map;
	mask = f->mask;
	if (r < 0 || !map[r] || c < 0 || !map[r][c])
		return (set_error_tmp_map(f, r, c, PERR_UNCLOSED_MAP));
	if (map[r][c] == ' ')
		return (set_error_tmp_map(f, r, c, PERR_UNEXPECTED_SPACE));
	if (ft_isset(map[r][c], f->wall_sym))
		return (true);
	if (mask[r][c])
		return (true);
	mask[r][c] = true;
	return (flood_fill(f, r + 1, c) && flood_fill(f, r - 1, c)
		&& flood_fill(f, r, c + 1) && flood_fill(f, r, c - 1));
}

bool	check_bonus_map(t_tmp_map_data *f)
{
	char	**map;
	char	**b;
	ssize_t	r;
	ssize_t	c;

	map = f->map;
	b = f->bonus;
	r = -1;
	while (map[++r])
	{
		c = -1;
		while (map[r][++c])
		{
			if (!b[r] || !b[r][c])
				return (set_error_tmp_map(f, r, c, PERR_MAP_MISMATCH));
			if (b[r][c] != map[r][c] && !ft_isset(map[r][c], f->others_sym))
				return (set_error_tmp_map(f, r, c, PERR_MAP_MISMATCH));
		}
		if (b[r][c] != '\0')
			return (set_error_tmp_map(f, r, c, PERR_MAP_MISMATCH));
	}
	if (b[r] != NULL)
		return (set_error_tmp_map(f, r, 0, PERR_MAP_MISMATCH));
	return (true);
}

bool	validate_map(t_data *d, t_index *i)
{
	t_tmp_map_data	f;
	bool			flood_success;

	if (!check_m_map_sym(d->all_ids, d->m_map, i, d->m_map_section_id))
		return (false);
	if (!check_player(d, i))
		return (false);
	f.map = d->m_map;
	f.bonus = d->b_map;
	f.wall_sym = d->wall_ids;
	f.air_sym = d->air_ids;
	f.others_sym = &d->all_ids[d->walls_len + d->airs_len];
	f.mask = dup_str_tab_to_bool_tab(f.map, d->m_height);
	if (!f.mask)
		return (i->err = PERR_MALLOC_ERR, printf("1\n"), false);
	flood_success = flood_fill(&f, d->player_row, d->player_col);
	free_bool_tab(f.mask);
	if (!flood_success)
		return (i->line = d->m_map_section_id + f.r, i->col = f.c,
			i->err = f.err, false);
	if (d->have_bonus && !check_bonus_map(&f))
		return (i->line = d->b_map_section_id + f.r, i->col = f.c,
			i->err = f.err, false);
	return (true);
}
