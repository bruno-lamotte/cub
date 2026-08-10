#include "cub.h"

typedef struct s_obj_draw
{
	t_img	*tex;
	int		coords[6];
	int		stripe;
	double	shade;
	char	sym;
}				t_obj_draw;

static t_img	*get_obj_tex_and_coords(t_engine *eng, char sym, int *w_h,
					int coords[6])
{
	t_img	*tex;

	if (sym == 'L' || sym == 'X')
	{
		tex = &eng->lamp_tex;
		coords[0] = w_h[3];
		coords[1] = w_h[3] + w_h[0] * 0.3;
		coords[5] = w_h[0] * 0.3;
	}
	else
	{
		tex = &eng->terminal_tex;
		coords[0] = w_h[4] - w_h[0] * 0.6;
		coords[1] = w_h[4];
		coords[5] = w_h[0] * 0.6;
	}
	return (tex);
}

static int	setup_obj_draw_coords(int stripe, int *w_h, t_obj_draw *d,
				int win_height)
{
	if (d->coords[5] <= 0)
		return (0);
	if (d->coords[0] < 0)
		d->coords[0] = 0;
	if (d->coords[1] >= win_height)
		d->coords[1] = win_height - 1;
	d->coords[4] = (w_h[1] + w_h[0] / 2) - d->coords[5] / 2;
	d->coords[2] = (int)((stripe - d->coords[4]) * d->tex->width
			/ (double)d->coords[5]);
	if (d->coords[2] < 0 || d->coords[2] >= d->tex->width)
		return (0);
	return (1);
}

static void	loop_draw_obj_stripe(t_engine *eng, int *w_h, t_obj_draw *d)
{
	double			step[2];
	double			offset;
	int				y;
	unsigned int	col;

	offset = w_h[4] - w_h[0] * 0.6;
	step[1] = (double)d->tex->height / (w_h[0] * 0.6);
	if (d->sym == 'L' || d->sym == 'X')
		offset = w_h[3];
	if (d->sym == 'L' || d->sym == 'X')
		step[1] = (double)d->tex->height / (w_h[0] * 0.3);
	step[0] = (d->coords[0] - offset) * step[1];
	y = d->coords[0] - 1;
	while (++y <= d->coords[1])
	{
		d->coords[3] = (int)step[0];
		step[0] += step[1];
		if (d->coords[3] < 0 || d->coords[3] >= d->tex->height)
			continue ;
		col = *(unsigned int *)(d->tex->addr + (d->coords[3] * d->tex->line_len
					+ d->coords[2] * (d->tex->bpp / 8)));
		if (col == get_transparent_color(d->tex) || (col & 0xFFFFFF) == 0xFFFFFF)
			continue ;
		put_pixel(&eng->screen->img2, d->stripe, y,
			((int)(((col >> 16) & 0xFF) * d->shade) << 16)
			| ((int)(((col >> 8) & 0xFF) * d->shade) << 8)
			| (int)((col & 0xFF) * d->shade));
	}
}

void	draw_object_stripe(t_engine *eng, int stripe, int *w_h,
			t_sprite *s)
{
	t_obj_draw	d;

	if (s->sym != 'L' && s->sym != 'X' && s->sym != 'T')
		return ;
	if (w_h[0] <= 0)
		return ;
	d.sym = s->sym;
	d.stripe = stripe;
	d.shade = s->shade;
	d.tex = get_obj_tex_and_coords(eng, s->sym, w_h, d.coords);
	if (!setup_obj_draw_coords(stripe, w_h, &d, eng->screen->win_height))
		return ;
	loop_draw_obj_stripe(eng, w_h, &d);
}

void	draw_objects_3d(t_engine *eng)
{
	(void)eng;
}
