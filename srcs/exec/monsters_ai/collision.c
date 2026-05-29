#include "cub.h"

int	check_monster_collision(double x, double y, int self_idx,
				t_engine *eng)
{
	t_monster_rt	*mstr;
	int				i;
	double			dist2;

	dist2 = (x - eng->player->pos.d.x) * (x - eng->player->pos.d.x)
		+ (y - eng->player->pos.d.y) * (y - eng->player->pos.d.y);
	if (dist2 < MSTR_COLLISION_DIST_SQ)
		return (0);
	mstr = get_monster_rt(eng->blob);
	i = -1;
	while (++i < eng->data->monster_rt_count)
	{
		if (i == self_idx)
			continue ;
		if (mstr[i].flags & MONSTER_DEAD)
			continue ;
		dist2 = (x - mstr[i].pos.d.x) * (x - mstr[i].pos.d.x)
			+ (y - mstr[i].pos.d.y) * (y - mstr[i].pos.d.y);
		if (dist2 < MSTR_COLLISION_DIST_SQ)
			return (0);
	}
	return (1);
}

int	check_monster_self_collision(t_monster_rt *self, t_engine *eng)
{
	t_monster_rt	*mstr;
	int				i;
	double			d2;

	mstr = get_monster_rt(eng->blob);
	i = -1;
	while (++i < eng->data->monster_rt_count)
	{
		if (&mstr[i] == self || (mstr[i].flags & MONSTER_DEAD))
			continue ;
		d2 = (self->pos.d.x - mstr[i].pos.d.x) * (self->pos.d.x - mstr[i].pos.d.x)
			+ (self->pos.d.y - mstr[i].pos.d.y) * (self->pos.d.y - mstr[i].pos.d.y);
		if (d2 < MSTR_COLLISION_DIST_SQ)
			return (1);
	}
	return (0);
}

int	is_bottleneck(int x, int y, void *blob)
{
	uint8_t	*flags;
	int		w;

	w = get_map_width(get_blob_hdr(blob));
	flags = get_map_flags(blob);
	if (!is_walkable(x, y, blob))
		return (0);
	if (flags[y * w + x] & CELL_HAS_DOOR)
		return (1);
	if (!is_walkable(x - 1, y, blob)
		&& !is_walkable(x + 1, y, blob)
		&& is_walkable(x, y - 1, blob)
		&& is_walkable(x, y + 1, blob))
		return (1);
	if (!is_walkable(x, y - 1, blob)
		&& !is_walkable(x, y + 1, blob)
		&& is_walkable(x - 1, y, blob)
		&& is_walkable(x + 1, y, blob))
		return (1);
	return (0);
}

int	is_any_monster_near(t_engine *eng, t_light *l)
{
	t_monster_rt	*mstr;
	int				j;
	t_vec2			d;

	mstr = get_monster_rt(eng->blob);
	j = -1;
	while (++j < eng->data->monster_rt_count)
	{
		if (!(mstr[j].flags & MONSTER_DEAD))
		{
			d.d.x = mstr[j].pos.d.x - l->x;
			d.d.y = mstr[j].pos.d.y - l->y;
			if (d.d.x * d.d.x + d.d.y * d.d.y < MSTR_LIGHT_PROX_DIST_SQ)
				return (1);
		}
	}
	return (0);
}
