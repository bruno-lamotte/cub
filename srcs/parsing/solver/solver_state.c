#include "cub.h"

uint64_t	get_alarm_mask(t_engine *eng)
{
	uint64_t	mask;
	int			i;

	mask = 0;
	i = -1;
	while (++i < eng->static_light_count)
	{
		if (eng->static_lights[i].is_alarm
			&& eng->static_lights[i].is_triggered)
		{
			if (i < 64)
				mask |= (1ULL << i);
		}
	}
	return (mask);
}

uint64_t	get_door_mask(t_engine *eng)
{
	uint64_t	mask;
	t_door_rt	*doors;
	int			count;
	int			i;

	mask = 0;
	doors = get_door_rt(eng->blob);
	count = get_blob_hdr(eng->blob)->door_rt.count;
	if (count > 64)
		count = 64;
	i = -1;
	while (++i < count)
	{
		if (doors[i].flags & DOOR_BLOCKED)
			mask |= (1ULL << i);
	}
	return (mask);
}

uint64_t	get_monster_mask(t_engine *eng)
{
	uint64_t		mask;
	t_monster_rt	*mstr;
	int				count;
	int				i;

	mask = 0;
	mstr = get_monster_rt(eng->blob);
	count = eng->data->monster_rt_count;
	if (count > 64)
		count = 64;
	i = -1;
	while (++i < count)
	{
		if (mstr[i].flags & MONSTER_DEAD)
			mask |= (1ULL << i);
		else
		{
			mask ^= ((uint64_t)((int)mstr[i].pos.d.x * 31
						+ (int)mstr[i].pos.d.y) << (i % 32));
		}
	}
	return (mask);
}

int	compare_states(t_map_state *a, t_map_state *b)
{
	if (a->player_pos.i.x != b->player_pos.i.x)
		return (a->player_pos.i.x - b->player_pos.i.x);
	if (a->player_pos.i.y != b->player_pos.i.y)
		return (a->player_pos.i.y - b->player_pos.i.y);
	if (a->door_mask != b->door_mask)
		return (a->door_mask < b->door_mask ? -1 : 1);
	if (a->alarm_mask != b->alarm_mask)
		return (a->alarm_mask < b->alarm_mask ? -1 : 1);
	return (0);
}
