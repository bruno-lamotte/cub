#include "cub.h"
#include <math.h>

typedef struct s_mstr_draw
{
	t_img			*tex;
	unsigned int	transparent_color;
	double			sprite_height;
	int				draw_start_y;
	int				draw_start_x;
	double			scaled_width;
	int				tex_x;
	int				y_start;
	int				y_end;
	int				stripe;
	int				w_h_0;
	int				mirror;
}	t_mstr_draw;

static int	get_speed_div(t_engine *eng, t_mstr_anim *anim)
{
	if (anim == &eng->anims[MSTR_ANIM_COURSE])
		return (MSTR_COURSE_SPEED_DIV);
	if (anim == &eng->anims[MSTR_ANIM_PUNCH])
		return (MSTR_PUNCH_SPEED_DIV);
	return (MSTR_PATROL_SPEED_DIV);
}

static int	calc_mstr_draw_params(t_engine *eng, t_sprite *s, t_mstr_draw *d)
{
	t_vec2			vt[2];
	t_mstr_anim		*anim;
	double			scale;

	anim = &eng->anims[get_monster_active_anim(eng, s, &d->mirror)];
	if (anim->frame_count <= 0)
		return (0);
	d->tex = &anim->frames[(eng->pool.current_frame / get_speed_div(eng,
				anim)) % anim->frame_count];
	d->transparent_color = get_transparent_color(&anim->frames[0]);
	get_sprite_coords(eng, s->pos, &vt[0], &vt[1]);
	scale = (d->w_h_0 * MONSTER_SCALE) / MSTR_ORIG_WIDTH;
	if (anim == &eng->anims[MSTR_ANIM_PUNCH])
		scale *= 0.7;
	d->scaled_width = d->tex->width * scale;
	if (d->scaled_width <= 0.0)
		return (0);
	d->draw_start_x = (int)((eng->screen->win_width / 2)
			* (1.0 + vt[1].d.x / vt[1].d.y)) + anim->offset.i.x * scale
		- d->scaled_width / 2.0;
	d->tex_x = (int)((d->stripe - d->draw_start_x) * d->tex->width
			/ d->scaled_width);
	if (!d->mirror)
		d->tex_x = d->tex->width - 1 - d->tex_x;
	if (d->tex_x < 0 || d->tex_x >= d->tex->width)
		return (0);
	d->sprite_height = d->tex->height * scale;
	d->draw_start_y = (eng->screen->win_height / 2 + d->w_h_0 * 0.5)
		- d->sprite_height;
	return (1);
}

static void	loop_draw_mstr_stripe(t_engine *eng, t_sprite *s, t_mstr_draw *d)
{
	double			step[2];
	int				y;
	int				tex_y;
	unsigned int	col;

	d->y_start = fmax(0, d->draw_start_y);
	d->y_end = fmin(eng->screen->win_height - 1,
			d->draw_start_y + d->sprite_height);
	step[1] = (double)d->tex->height / d->sprite_height;
	step[0] = (d->y_start - d->draw_start_y) * step[1];
	y = d->y_start - 1;
	while (++y <= d->y_end)
	{
		tex_y = (int)step[0];
		step[0] += step[1];
		if (tex_y < 0 || tex_y >= d->tex->height || tex_y >= 1264)
			continue ;
		col = *(unsigned int *)(d->tex->addr + (tex_y * d->tex->line_len
					+ d->tex_x * (d->tex->bpp / 8)));
		if (col == d->transparent_color || (col & 0x00FFFFFF) == 0x00FFFFFF)
			continue ;
		put_pixel(&eng->screen->img2, d->stripe, y,
			((int)(((col >> 16) & 0xFF) * s->shade) << 16) |
			((int)(((col >> 8) & 0xFF) * s->shade) << 8) |
			(int)((col & 0xFF) * s->shade));
	}
}

void	draw_monster_stripe(t_engine *eng, int stripe, int *w_h,
			t_sprite *s)
{
	t_mstr_draw	d;

	if (w_h[0] <= 0)
		return ;
	d.stripe = stripe;
	d.w_h_0 = w_h[0];
	if (!calc_mstr_draw_params(eng, s, &d))
		return ;
	loop_draw_mstr_stripe(eng, s, &d);
}
