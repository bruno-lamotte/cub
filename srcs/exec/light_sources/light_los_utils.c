/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   light_los_utils.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: blamotte <blamotte@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/11 00:33:49 by blamotte          #+#    #+#             */
/*   Updated: 2026/08/11 00:33:49 by blamotte         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub.h"
#include <math.h>

int	check_los_shadow(t_vec2 m, t_vec2 p)
{
	float	fx;
	float	fy;

	fx = (float)p.d.x - (int)p.d.x;
	fy = (float)p.d.y - (int)p.d.y;
	if (fabsf(fx - 0.99f) < 1e-6f && (float)m.d.x >= (int)p.d.x + 1)
		return (0);
	if (fabsf(fx - 0.01f) < 1e-6f && (float)m.d.x <= (int)p.d.x)
		return (0);
	if (fabsf(fy - 0.99f) < 1e-6f && (float)m.d.y >= (int)p.d.y + 1)
		return (0);
	if (fabsf(fy - 0.01f) < 1e-6f && (float)m.d.y <= (int)p.d.y)
		return (0);
	return (1);
}

static int	check_door_walkable_los(t_map_data *map, int idx)
{
	uint32_t	i;

	i = -1;
	while (++i < map->door_count)
	{
		if (map->doors[i].map_id == (uint32_t)idx)
		{
			if (map->doors[i].open_ratio_255 > 200)
				return (1);
			break ;
		}
	}
	return (0);
}

static int	is_cell_los_walkable(int x, int y, t_map_data *map)
{
	if (x < 0 || x >= map->w || y < 0 || y >= map->h)
		return (0);
	if (map->flags[y * map->w + x] & CELL_HAS_WALL)
		return (0);
	if (map->flags[y * map->w + x] & CELL_HAS_DOOR)
		return (check_door_walkable_los(map, y * map->w + x));
	return (1);
}

static void	dda_step(float d[4], int step[4], int pos[2], float s[2])
{
	if (s[0] < s[1])
	{
		s[0] += d[2];
		pos[0] += step[0];
	}
	else
	{
		s[1] += d[3];
		pos[1] += step[1];
	}
}

int	run_dda(int step[4], float d[4], float side[2], t_map_data *map)
{
	int		pos[2];
	float	s[2];

	pos[0] = step[2];
	pos[1] = step[3];
	s[0] = side[0];
	s[1] = side[1];
	while (pos[0] != map->target_x || pos[1] != map->target_y)
	{
		dda_step(d, step, pos, s);
		if (pos[0] == map->target_x && pos[1] == map->target_y)
			break ;
		if (!is_cell_los_walkable(pos[0], pos[1], map))
			return (0);
	}
	return (1);
}
