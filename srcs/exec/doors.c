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
	t_door_rt	*doors;
	int			door_count;
	int			width;
	int			i;
	t_xy_double	d;

	doors = get_door_rt(engine->blob);
	door_count = get_blob_hdr(engine->blob)->door_rt.count;
	width = get_map_width(get_blob_hdr(engine->blob));
	i = -1;
	while (++i < door_count)
	{
		d.x = (double)(doors[i].map_id % width) + 0.5 - engine->player->pos.x;
		d.y = (double)(doors[i].map_id / width) + 0.5 - engine->player->pos.y;
		check_door_proximity(&doors[i], engine, d.x * d.x + d.y * d.y);
		process_door_transition(&doors[i]);
	}
}
