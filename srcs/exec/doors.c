/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   doors.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: blamotte <blamotte@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/11 00:33:49 by blamotte          #+#    #+#             */
/*   Updated: 2026/08/11 00:33:49 by blamotte         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub.h"
#include <math.h>

static void	process_door_transition(t_door_rt *door)
{
	if (door->flags & DOOR_OPENING)
	{
		if (door->open_ratio_255 < 255 - 15)
			door->open_ratio_255 += 15;
		else
		{
			door->open_ratio_255 = 255;
			door->flags &= ~DOOR_OPENING;
			door->flags |= DOOR_IS_OPEN;
		}
	}
	else if (door->flags & DOOR_CLOSING)
	{
		if (door->open_ratio_255 > 15)
			door->open_ratio_255 -= 15;
		else
		{
			door->open_ratio_255 = 0;
			door->flags &= ~DOOR_CLOSING;
		}
	}
}

static void	check_blocked_door(t_door_rt *door)
{
	door->flags &= ~(DOOR_OPENING | DOOR_IS_OPEN);
	if (door->open_ratio_255 > 0)
		door->flags |= DOOR_CLOSING;
}

static void	check_door_proximity(t_door_rt *door, t_engine *eng, double d2)
{
	t_door_def	*def;

	if (door->flags & DOOR_BLOCKED)
		return (check_blocked_door(door));
	def = &eng->data->doors_defs[door->def_id];
	if (d2 < 2.25)
	{
		door->flags &= ~DOOR_CLOSING;
		door->flags |= DOOR_HAD_PLAYER_NEAR;
		if (door->open_ratio_255 < 255)
			door->flags |= DOOR_OPENING;
	}
	else if (d2 > 4.0)
	{
		door->flags &= ~(DOOR_OPENING | DOOR_IS_OPEN);
		if ((def->is_closed || (door->flags & DOOR_HAD_PLAYER_NEAR))
			&& door->open_ratio_255 > 0)
			door->flags |= DOOR_CLOSING;
	}
}

static double	get_min_door_dist(t_door_rt *door, t_engine *eng, int width)
{
	t_monster_rt	*mstr;
	t_vec2			door_pos;
	t_vec2			diff;
	double			min_d2;
	int				j;

	door_pos.d.x = (double)(door->map_id % width) + 0.5;
	door_pos.d.y = (double)(door->map_id / width) + 0.5;
	diff.d.x = door_pos.d.x - eng->player->pos.d.x;
	diff.d.y = door_pos.d.y - eng->player->pos.d.y;
	min_d2 = diff.d.x * diff.d.x + diff.d.y * diff.d.y;
	mstr = get_monster_rt(eng->blob);
	j = -1;
	while (++j < eng->data->monster_rt_count)
	{
		if (mstr[j].flags & MONSTER_DEAD)
			continue ;
		diff.d.x = door_pos.d.x - mstr[j].pos.d.x;
		diff.d.y = door_pos.d.y - mstr[j].pos.d.y;
		if (diff.d.x * diff.d.x + diff.d.y * diff.d.y < min_d2)
			min_d2 = diff.d.x * diff.d.x + diff.d.y * diff.d.y;
	}
	return (min_d2);
}

void	update_doors(t_engine *engine)
{
	t_door_rt	*doors;
	int			door_count;
	int			width;
	int			i;

	doors = get_door_rt(engine->blob);
	door_count = get_blob_hdr(engine->blob)->door_rt.u_data.count;
	width = get_map_width(get_blob_hdr(engine->blob));
	i = -1;
	while (++i < door_count)
	{
		check_door_proximity(&doors[i], engine,
			get_min_door_dist(&doors[i], engine, width));
		process_door_transition(&doors[i]);
	}
}
