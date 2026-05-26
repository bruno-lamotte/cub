#include "cub.h"
#include <math.h>

#define MOVE_SPEED 0.05
#define ROT_SPEED 0.03
#define COLLISION_RADIUS 0.2

int	is_walkable(int x, int y, int width, int height, void *blob)
{
	uint8_t		*flags;
	t_door_rt	*doors;
	int			door_count;
	int			i;

	flags = get_map_flags(blob);
	if (x < 0 || x >= width || y < 0 || y >= height)
		return (0);
	if (flags[y * width + x] & CELL_HAS_WALL)
		return (0);
	if (flags[y * width + x] & CELL_HAS_DOOR)
	{
		doors = get_door_rt(blob);
		door_count = get_blob_hdr(blob)->door_rt.count;
		i = -1;
		while (++i < door_count)
		{
			if (doors[i].map_id == (uint32_t)(y * width + x))
				return (doors[i].open_ratio_255 > 200);
		}
		return (0);
	}
	return (1);
}

int	is_valid_position(double x, double y, int width, int height, void *blob)
{
	if (!is_walkable((int)(x - COLLISION_RADIUS), (int)(y - COLLISION_RADIUS),
			width, height, blob))
		return (0);
	if (!is_walkable((int)(x + COLLISION_RADIUS), (int)(y - COLLISION_RADIUS),
			width, height, blob))
		return (0);
	if (!is_walkable((int)(x - COLLISION_RADIUS), (int)(y + COLLISION_RADIUS),
			width, height, blob))
		return (0);
	if (!is_walkable((int)(x + COLLISION_RADIUS), (int)(y + COLLISION_RADIUS),
			width, height, blob))
		return (0);
	return (1);
}

static int	check_player_mstr_collision(double x, double y, t_engine *eng)
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
		d2 = (x - mstr[i].pos.x) * (x - mstr[i].pos.x)
			+ (y - mstr[i].pos.y) * (y - mstr[i].pos.y);
		if (d2 < 0.36)
			return (0);
	}
	return (1);
}

static void	try_move_player(t_engine *eng, double dx, double dy)
{
	double	nx;
	double	ny;
	int		w;
	int		h;

	w = get_map_width(get_blob_hdr(eng->blob));
	h = get_map_height(get_blob_hdr(eng->blob));
	nx = eng->player->pos.x + dx;
	ny = eng->player->pos.y + dy;
	if (is_valid_position(nx, eng->player->pos.y, w, h, eng->blob)
		&& check_player_mstr_collision(nx, eng->player->pos.y, eng))
		eng->player->pos.x = nx;
	if (is_valid_position(eng->player->pos.x, ny, w, h, eng->blob)
		&& check_player_mstr_collision(eng->player->pos.x, ny, eng))
		eng->player->pos.y = ny;
}

void	update_position(t_engine *eng, t_keys *keys)
{
	t_player_rt	*p;

	p = eng->player;
	if (keys->w)
		try_move_player(eng, p->dir.x * MOVE_SPEED, p->dir.y * MOVE_SPEED);
	if (keys->s)
		try_move_player(eng, -p->dir.x * MOVE_SPEED, -p->dir.y * MOVE_SPEED);
	if (keys->d)
		try_move_player(eng, -p->dir.y * MOVE_SPEED, p->dir.x * MOVE_SPEED);
	if (keys->a)
		try_move_player(eng, p->dir.y * MOVE_SPEED, -p->dir.x * MOVE_SPEED);
}

void	update_rotation(t_player_rt *p, t_keys *keys)
{
	double	old_dir_x;
	double	old_plane_x;
	double	speed;

	if (!keys->left && !keys->right)
		return ;
	speed = ROT_SPEED;
	if (keys->left)
		speed = -ROT_SPEED;
	old_dir_x = p->dir.x;
	p->dir.x = p->dir.x * cos(speed) - p->dir.y * sin(speed);
	p->dir.y = old_dir_x * sin(speed) + p->dir.y * cos(speed);
	old_plane_x = p->plane.x;
	p->plane.x = p->plane.x * cos(speed) - p->plane.y * sin(speed);
	p->plane.y = old_plane_x * sin(speed) + p->plane.y * cos(speed);
}
