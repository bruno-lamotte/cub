#include "cub.h"

static void	draw_monster_pixel(t_engine *eng, t_vec2 pos)
{
	put_pixel(&eng->screen->img2, pos.i.x, pos.i.y, 0xFF0000);
	put_pixel(&eng->screen->img2, pos.i.x - 1, pos.i.y, 0xFF0000);
	put_pixel(&eng->screen->img2, pos.i.x + 1, pos.i.y, 0xFF0000);
	put_pixel(&eng->screen->img2, pos.i.x, pos.i.y - 1, 0xFF0000);
	put_pixel(&eng->screen->img2, pos.i.x, pos.i.y + 1, 0xFF0000);
}

static int	get_monster_shade_idx(t_engine *eng, t_monster_rt *m)
{
	float	d2;
	int		idx;

	d2 = (m->pos.d.x - eng->player->pos.d.x) * (m->pos.d.x - eng->player->pos.d.x)
		+ (m->pos.d.y - eng->player->pos.d.y) * (m->pos.d.y - eng->player->pos.d.y);
	if (d2 <= 0.001f)
		return (0);
	idx = (int)(d2 * fast_inv_sqrt(d2) * 10.0f);
	if (idx < 0)
		return (0);
	if (idx >= DIST_MAX)
		return (DIST_MAX - 1);
	return (idx);
}

static void	draw_single_minimap_monster(t_engine *eng, t_monster_rt *m,
				t_vec2 c, int radius, double px_per_tile)
{
	t_vec2		pos;

	if (get_lut_bdef(eng->blob)->shade_table[get_monster_shade_idx(eng, m)] <= 0.05)
		return ;
	pos.i.x = c.i.x + (int)((m->pos.d.x - eng->player->pos.d.x) * px_per_tile);
	pos.i.y = c.i.y + (int)((m->pos.d.y - eng->player->pos.d.y) * px_per_tile);
	if ((pos.i.x - c.i.x) * (pos.i.x - c.i.x) + (pos.i.y - c.i.y) * (pos.i.y - c.i.y)
		<= (radius - 5) * (radius - 5))
		draw_monster_pixel(eng, pos);
}

void	draw_minimap_monsters(t_engine *eng, t_vec2 c, int radius,
			double px_per_tile)
{
	t_monster_rt	*m;
	int				i;

	m = get_monster_rt(eng->blob);
	i = -1;
	while (++i < eng->data->monster_rt_count)
	{
		if (!(m[i].flags & MONSTER_DEAD))
			draw_single_minimap_monster(eng, &m[i], c, radius, px_per_tile);
	}
}

static void	init_triangle_points(t_vec2 c, t_vec2 dir, double pt[6])
{
	pt[0] = c.i.x + dir.d.x * 8.0;
	pt[1] = c.i.y + dir.d.y * 8.0;
	pt[2] = c.i.x - dir.d.x * 4.0 - dir.d.y * 3.0;
	pt[3] = c.i.y - dir.d.y * 4.0 + dir.d.x * 3.0;
	pt[4] = c.i.x - dir.d.x * 4.0 + dir.d.y * 3.0;
	pt[5] = c.i.y - dir.d.y * 4.0 - dir.d.x * 3.0;
}

void	draw_minimap_player(t_engine *eng, t_vec2 c)
{
	double		pt[6];
	t_vec2		p;
	double		d[3];

	init_triangle_points(c, eng->player->dir, pt);
	p.i.y = c.i.y - 11;
	while (++p.i.y <= c.i.y + 9)
	{
		p.i.x = c.i.x - 11;
		while (++p.i.x <= c.i.x + 9)
		{
			d[0] = (p.i.x - pt[2]) * (pt[1] - pt[3])
				- (pt[0] - pt[2]) * (p.i.y - pt[3]);
			d[1] = (p.i.x - pt[4]) * (pt[3] - pt[5])
				- (pt[2] - pt[4]) * (p.i.y - pt[5]);
			d[2] = (p.i.x - pt[0]) * (pt[5] - pt[1])
				- (pt[4] - pt[0]) * (p.i.y - pt[1]);
			if ((d[0] >= -0.001 && d[1] >= -0.001 && d[2] >= -0.001)
				|| (d[0] <= 0.001 && d[1] <= 0.001 && d[2] <= 0.001))
				put_pixel(&eng->screen->img2, p.i.x, p.i.y, 0x00FF66);
		}
	}
}
