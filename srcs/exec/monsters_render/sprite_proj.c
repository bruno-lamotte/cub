#include "cub.h"
#include <stdlib.h>

static int	sh_idx(double t)
{
	int	idx;

	idx = (int)(t * 10.0f);
	if (idx < 0)
		return (0);
	if (idx >= DIST_MAX)
		return (DIST_MAX - 1);
	return (idx);
}

static void	calc_object_projection(t_engine *eng, double trans[2], int w_h[5])
{
	w_h[0] = abs((int)(eng->screen->win_height / trans[1]));
	w_h[1] = -w_h[0] / 2 + (int)((eng->screen->win_width / 2) * (1.0 + trans[0]
				/ trans[1]));
	if (w_h[1] < 0)
		w_h[1] = 0;
	w_h[2] = w_h[0] / 2 + (int)((eng->screen->win_width / 2) * (1.0 + trans[0]
				/ trans[1]));
	if (w_h[2] >= eng->screen->win_width)
		w_h[2] = eng->screen->win_width - 1;
	w_h[3] = -w_h[0] / 2 + eng->screen->win_height / 2;
	if (w_h[3] < 0)
		w_h[3] = 0;
	w_h[4] = w_h[0] / 2 + eng->screen->win_height / 2;
	if (w_h[4] >= eng->screen->win_height)
		w_h[4] = eng->screen->win_height - 1;
}

#ifndef MONSTER_SCALE
# define MONSTER_SCALE 1.8
#endif

static void	render_sprite(t_engine *eng, t_sprite *s, t_lut *lut)
{
	double	sp[2];
	double	trans[2];
	int		w_h[5];
	int		stripe;
	int		x_start;
	int		x_end;

	if (get_sprite_coords(eng, s->pos.d.x, s->pos.d.y, sp, trans) <= 0.1)
		return ;
	calc_object_projection(eng, trans, w_h);
	s->shade = lut->shade_table[sh_idx(trans[1])];
	if (s->is_monster)
	{
		int sprite_screen_x = (int)((eng->screen->win_width / 2) * (1.0 + trans[0] / trans[1]));
		int scaled_width = (int)(w_h[0] * MONSTER_SCALE);
		x_start = -scaled_width / 2 + sprite_screen_x;
		x_end = scaled_width / 2 + sprite_screen_x;
		if (x_start < 0)
			x_start = 0;
		if (x_end >= eng->screen->win_width)
			x_end = eng->screen->win_width - 1;
	}
	else
	{
		x_start = w_h[1];
		x_end = w_h[2];
	}
	stripe = x_start - 1;
	while (++stripe <= x_end)
	{
		if (trans[1] < FP_TO_FLOAT(eng->z_buffer[stripe].perp_wall_dist))
		{
			if (s->is_monster)
				draw_monster_stripe(eng, stripe, w_h, s);
			else
				draw_object_stripe(eng, stripe, w_h, s);
		}
	}
}

void	draw_monsters_3d(t_engine *eng)
{
	t_sprite	sprites[512];
	t_lut		*lut;
	int			count;
	int			i;

	count = 0;
	count = collect_monsters(eng, sprites, count);
	count = collect_objects(eng, sprites, count);
	sort_sprites(sprites, count);
	lut = get_lut_bdef(eng->blob);
	i = -1;
	while (++i < count)
		render_sprite(eng, &sprites[i], lut);
}
