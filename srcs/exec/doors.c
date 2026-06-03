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

void	update_doors(t_engine *engine)
{
	t_door_rt		*doors;
	t_monster_rt	*mstr;
	int				door_count;
	int				width;
	int				i;
	int				j;
	double			door_x;
	double			door_y;
	double			dx;
	double			dy;
	double			min_d2;
	double			d2;

	doors = get_door_rt(engine->blob);
	door_count = get_blob_hdr(engine->blob)->door_rt.count;
	width = get_map_width(get_blob_hdr(engine->blob));
	mstr = get_monster_rt(engine->blob);
	i = -1;
	while (++i < door_count)
	{
		door_x = (double)(doors[i].map_id % width) + 0.5;
		door_y = (double)(doors[i].map_id / width) + 0.5;
		dx = door_x - engine->player->pos.d.x;
		dy = door_y - engine->player->pos.d.y;
		min_d2 = dx * dx + dy * dy;
		j = -1;
		while (++j < engine->data->monster_rt_count)
		{
			if (mstr[j].flags & MONSTER_DEAD)
				continue ;
			dx = door_x - mstr[j].pos.d.x;
			dy = door_y - mstr[j].pos.d.y;
			d2 = dx * dx + dy * dy;
			if (d2 < min_d2)
				min_d2 = d2;
		}
		check_door_proximity(&doors[i], engine, min_d2);
		process_door_transition(&doors[i]);
	}
}
