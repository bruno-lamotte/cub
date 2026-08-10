#include "cub.h"

#define COLLISION_RADIUS 0.2

static int	check_door_walkable(int x, int y, int w, void *blob)
{
	t_door_rt	*doors;
	uint32_t	i;

	doors = get_door_rt(blob);
	i = -1;
	while (++i < get_blob_hdr(blob)->door_rt.count)
	{
		if (doors[i].map_id == (uint32_t)(y * w + x))
			return (doors[i].open_ratio_255 > 200);
	}
	return (0);
}

int	is_walkable(int x, int y, void *blob)
{
	uint8_t		*flags;
	int			w;

	w = get_map_width(get_blob_hdr(blob));
	flags = get_map_flags(blob);
	if (x < 0 || x >= w || y < 0 || y >= get_map_height(get_blob_hdr(blob)))
		return (0);
	if (flags[y * w + x] & CELL_HAS_WALL)
		return (0);
	if (flags[y * w + x] & CELL_HAS_DOOR)
		return (check_door_walkable(x, y, w, blob));
	return (1);
}

int	is_walkable_for_routing(int x, int y, void *blob)
{
	uint8_t		*flags;
	int			w;
	int			h;

	w = get_map_width(get_blob_hdr(blob));
	h = get_map_height(get_blob_hdr(blob));
	flags = get_map_flags(blob);
	if (x < 0 || x >= w || y < 0 || y >= h)
		return (0);
	if (flags[y * w + x] & CELL_HAS_WALL)
		return (0);
	return (1);
}

int	is_valid_position(double x, double y, void *blob)
{
	if (!is_walkable((int)(x - COLLISION_RADIUS), (int)(y - COLLISION_RADIUS),
			blob))
		return (0);
	if (!is_walkable((int)(x + COLLISION_RADIUS), (int)(y - COLLISION_RADIUS),
			blob))
		return (0);
	if (!is_walkable((int)(x - COLLISION_RADIUS), (int)(y + COLLISION_RADIUS),
			blob))
		return (0);
	if (!is_walkable((int)(x + COLLISION_RADIUS), (int)(y + COLLISION_RADIUS),
			blob))
		return (0);
	return (1);
}

int	check_player_mstr_collision(double x, double y, t_engine *eng)
{
	t_monster_rt	*mstr;
	int				i;
	double			d2;

	mstr = get_monster_rt(eng->blob);
	i = -1;
	while (++i < eng->data->monster_rt_count)
	{
		if (mstr[i].flags & MONSTER_DEAD)
			continue ;
		d2 = (x - mstr[i].pos.d.x) * (x - mstr[i].pos.d.x)
			+ (y - mstr[i].pos.d.y) * (y - mstr[i].pos.d.y);
		if (d2 < 0.36)
			return (0);
	}
	return (1);
}
