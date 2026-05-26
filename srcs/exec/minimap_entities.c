#include "cub.h"

static void	draw_monster_pixel(t_engine *eng, t_vec2_i pos)
{
	put_pixel(&eng->screen->img2, pos.x, pos.y, 0xFF0000);
	put_pixel(&eng->screen->img2, pos.x - 1, pos.y, 0xFF0000);
	put_pixel(&eng->screen->img2, pos.x + 1, pos.y, 0xFF0000);
	put_pixel(&eng->screen->img2, pos.x, pos.y - 1, 0xFF0000);
	put_pixel(&eng->screen->img2, pos.x, pos.y + 1, 0xFF0000);
}

static void	draw_single_minimap_monster(t_engine *eng, t_monster_rt *m,
				t_vec2_i c, int radius)
{
	t_vec2_i	pos;
	float		d2;
	float		dist;

	d2 = (float)((m->pos.x - eng->player->pos.x)
			* (m->pos.x - eng->player->pos.x)
			+ (m->pos.y - eng->player->pos.y)
			* (m->pos.y - eng->player->pos.y));
	dist = 0.0f;
	if (d2 > 0.001f)
		dist = d2 * fast_inv_sqrt(d2);
	if (get_lut_bdef(eng->blob)->shade_table[(int)(dist * 10.0f) < 0 ? 0
		: ((int)(dist * 10.0f) >= DIST_MAX ? DIST_MAX - 1
		: (int)(dist * 10.0f))] <= 0.05)
		return ;
	pos.x = c.x + (int)((m->pos.x - eng->player->pos.x) * 15.0);
	pos.y = c.y + (int)((m->pos.y - eng->player->pos.y) * 15.0);
	if ((pos.x - c.x) * (pos.x - c.x) + (pos.y - c.y) * (pos.y - c.y)
		<= (radius - 5) * (radius - 5))
		draw_monster_pixel(eng, pos);
}

void	draw_minimap_monsters(t_engine *eng, t_vec2_i c, int radius)
{
	t_monster_rt	*m;
	int				i;

	m = get_monster_rt(eng->blob);
	i = -1;
	while (++i < eng->data->monster_rt_count)
	{
		if (!(m[i].flags & MONSTER_DEAD))
			draw_single_minimap_monster(eng, &m[i], c, radius);
	}
}

static void	init_triangle_points(t_vec2_i c, t_xy_double dir, double pt[6])
{
	pt[0] = c.x + dir.x * 8.0;
	pt[1] = c.y + dir.y * 8.0;
	pt[2] = c.x - dir.x * 4.0 - dir.y * 3.0;
	pt[3] = c.y - dir.y * 4.0 + dir.x * 3.0;
	pt[4] = c.x - dir.x * 4.0 + dir.y * 3.0;
	pt[5] = c.y - dir.y * 4.0 - dir.x * 3.0;
}

void	draw_minimap_player(t_engine *eng, t_vec2_i c)
{
	double		pt[6];
	t_vec2_i	p;
	double		d[3];

	init_triangle_points(c, eng->player->dir, pt);
	p.y = c.y - 11;
	while (++p.y <= c.y + 9)
	{
		p.x = c.x - 11;
		while (++p.x <= c.x + 9)
		{
			d[0] = (p.x - pt[2]) * (pt[1] - pt[3])
				- (pt[0] - pt[2]) * (p.y - pt[3]);
			d[1] = (p.x - pt[4]) * (pt[3] - pt[5])
				- (pt[2] - pt[4]) * (p.y - pt[5]);
			d[2] = (p.x - pt[0]) * (pt[5] - pt[1])
				- (pt[4] - pt[0]) * (p.y - pt[1]);
			if ((d[0] >= -0.001 && d[1] >= -0.001 && d[2] >= -0.001)
				|| (d[0] <= 0.001 && d[1] <= 0.001 && d[2] <= 0.001))
				put_pixel(&eng->screen->img2, p.x, p.y, 0x00FF66);
		}
	}
}
