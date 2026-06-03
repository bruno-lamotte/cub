#include "cub.h"
#include <math.h>

typedef struct s_map_data
{
	uint8_t		*flags;
	t_door_rt	*doors;
	int			w;
	int			h;
	uint32_t	door_count;
	int			target_x;
	int			target_y;
}	t_map_data;

static inline int	check_los_shadow(float mx, float my, float px, float py)
{
	float	fx;
	float	fy;

	fx = px - (int)px;
	fy = py - (int)py;
	if (fabsf(fx - 0.99f) < 1e-6f && mx >= (int)px + 1)
		return (0);
	if (fabsf(fx - 0.01f) < 1e-6f && mx <= (int)px)
		return (0);
	if (fabsf(fy - 0.99f) < 1e-6f && my >= (int)py + 1)
		return (0);
	if (fabsf(fy - 0.01f) < 1e-6f && my <= (int)py)
		return (0);
	return (1);
}

static inline void	init_los_steps(float mx, float my, float d[4], float side[2],
						int step[4])
{
	step[2] = (int)mx;
	step[3] = (int)my;
	if (d[0] < 0)
	{
		step[0] = -1;
		side[0] = (mx - step[2]) * d[2];
	}
	else
	{
		step[0] = 1;
		side[0] = (step[2] + 1.0f - mx) * d[2];
	}
	if (d[1] < 0)
	{
		step[1] = -1;
		side[1] = (my - step[3]) * d[3];
	}
	else
	{
		step[1] = 1;
		side[1] = (step[3] + 1.0f - my) * d[3];
	}
}

static inline void	init_los_dda(float mx, float my, float px, float py,
						float d[4], float side[2], int step[4])
{
	d[0] = px - mx;
	d[1] = py - my;
	if (d[0] == 0.0f)
		d[2] = 1e30f;
	else
		d[2] = fabsf(1.0f / d[0]);
	if (d[1] == 0.0f)
		d[3] = 1e30f;
	else
		d[3] = fabsf(1.0f / d[1]);
	init_los_steps(mx, my, d, side, step);
}

static inline int	check_door_walkable(t_map_data *map, int idx)
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

static inline void	dda_step(float d[4], int step[4], int pos[2], float s[2])
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

static inline int	run_dda(int step[4], float d[4], float side[2],
						t_map_data *map)
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
		if (pos[0] < 0 || pos[0] >= map->w || pos[1] < 0 || pos[1] >= map->h)
			return (0);
		if ((map->flags[pos[1] * map->w + pos[0]] & CELL_HAS_WALL))
			return (0);
		if ((map->flags[pos[1] * map->w + pos[0]] & CELL_HAS_DOOR)
			&& !check_door_walkable(map, pos[1] * map->w + pos[0]))
			return (0);
	}
	return (1);
}

int	check_los(t_vec2 p1, t_vec2 p2, void *blob)
{
	t_map_data	map;
	t_blob_hdr	*hdr;
	int			step[4];
	float		d[4];
	float		side[2];

	hdr = get_blob_hdr(blob);
	map.w = hdr->map.map_data.width;
	map.h = hdr->map.map_data.height;
	map.flags = get_map_flags(blob);
	map.doors = get_door_rt(blob);
	map.door_count = hdr->door_rt.count;
	map.target_x = (int)p2.d.x;
	map.target_y = (int)p2.d.y;
	if (!check_los_shadow((float)p1.d.x, (float)p1.d.y,
			(float)p2.d.x, (float)p2.d.y))
		return (0);
	init_los_dda((float)p1.d.x, (float)p1.d.y, (float)p2.d.x, (float)p2.d.y,
		d, side, step);
	return (run_dda(step, d, side, &map));
}
