#include "cub.h"
#include <math.h>

static void	init_los_steps(float mx, float my, float d[4], float side[2],
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

static void	init_los_dda(float mx, float my, float px, float py,
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

static int	check_side_los(t_vec2 p1, t_vec2 p2, t_vec2 perp, void *blob)
{
	t_vec2	p1_offset;
	t_vec2	p2_offset;

	p1_offset.d.x = p1.d.x + perp.d.x;
	p1_offset.d.y = p1.d.y + perp.d.y;
	p2_offset.d.x = p2.d.x + perp.d.x;
	p2_offset.d.y = p2.d.y + perp.d.y;
	return (check_los(p1_offset, p2_offset, blob));
}

int	check_thick_los(t_vec2 p1, t_vec2 p2, double radius, void *blob)
{
	t_vec2	perp;
	double	dx;
	double	dy;
	double	len;

	if (!check_los(p1, p2, blob))
		return (0);
	dx = p2.d.x - p1.d.x;
	dy = p2.d.y - p1.d.y;
	len = sqrt(dx * dx + dy * dy);
	if (len < 0.01)
		return (1);
	perp.d.x = (-dy / len) * radius;
	perp.d.y = (dx / len) * radius;
	if (!check_side_los(p1, p2, perp, blob))
		return (0);
	perp.d.x = -perp.d.x;
	perp.d.y = -perp.d.y;
	return (check_side_los(p1, p2, perp, blob));
}
