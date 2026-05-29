#include "cub.h"

void	draw_monster_stripe(t_engine *eng, int stripe, int *w_h,
			t_sprite *s)
{
	int				y;
	unsigned int	col;
	int				r;
	int				g;
	int				b;

	y = w_h[3] - 1;
	while (++y <= w_h[4])
	{
		col = 0xAA00FF;
		if (y >= w_h[3] + (w_h[4] - w_h[3]) * 0.25
			&& y <= w_h[3] + (w_h[4] - w_h[3]) * 0.35)
			col = 0xFF0000;
		r = (int)(((col >> 16) & 0xFF) * s->shade);
		g = (int)(((col >> 8) & 0xFF) * s->shade);
		b = (int)((col & 0xFF) * s->shade);
		put_pixel(&eng->screen->img2, stripe, y, (r << 16) | (g << 8) | b);
	}
}

static unsigned int	get_object_stripe_color(t_engine *eng, int diff,
						int h, char sym)
{
	if (sym == 'T')
	{
		if (diff >= h * 0.3 && diff <= h * 0.6)
			return (0x00FFFF);
		return (0x00FF00);
	}
	if (eng->alarm_triggered && (eng->pool.current_frame / 10) % 2)
		return (0xFF0000);
	if (eng->alarm_triggered)
		return (0x550000);
	return (0xCCCCCC);
}

void	draw_object_stripe(t_engine *eng, int stripe, int *w_h,
			t_sprite *s)
{
	int				y;
	unsigned int	col;
	int				r;
	int				g;
	int				b;

	y = w_h[3] - 1;
	while (++y <= w_h[4])
	{
		col = get_object_stripe_color(eng, y - w_h[3], w_h[4] - w_h[3],
				s->sym);
		r = (int)(((col >> 16) & 0xFF) * s->shade);
		g = (int)(((col >> 8) & 0xFF) * s->shade);
		b = (int)((col & 0xFF) * s->shade);
		put_pixel(&eng->screen->img2, stripe, y, (r << 16) | (g << 8) | b);
	}
}

void	draw_objects_3d(t_engine *eng)
{
	(void)eng;
}
