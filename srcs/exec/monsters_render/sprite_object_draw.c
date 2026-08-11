/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sprite_object_draw.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: blamotte <blamotte@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/11 01:39:07 by blamotte          #+#    #+#             */
/*   Updated: 2026/08/11 02:00:00 by blamotte         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub.h"

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

static int	setup_obj_draw_params(int stripe, int *w_h, t_obj_draw *d,
				double step[3])
{
	double	off;

	if (d->coords[5] <= 0)
		return (0);
	if (d->coords[0] < 0)
		d->coords[0] = 0;
	if (d->coords[1] >= step[2])
		d->coords[1] = (int)step[2] - 1;
	d->coords[4] = (w_h[1] + w_h[0] / 2) - d->coords[5] / 2;
	d->coords[2] = (int)((stripe - d->coords[4]) * d->tex->width
			/ (double)d->coords[5]);
	if (d->coords[2] < 0 || d->coords[2] >= d->tex->width)
		return (0);
	off = w_h[4] - w_h[0] * 0.6;
	step[1] = (double)d->tex->height / (w_h[0] * 0.6);
	if (d->sym == 'L' || d->sym == 'X')
		off = w_h[3];
	if (d->sym == 'L' || d->sym == 'X')
		step[1] = (double)d->tex->height / (w_h[0] * 0.3);
	step[0] = (d->coords[0] - off) * step[1];
	return (1);
}

static int	calc_obj_color(unsigned int col, double shade)
{
	int	r;
	int	g;
	int	b;

	r = (int)(((col >> 16) & 0xFF) * shade);
	g = (int)(((col >> 8) & 0xFF) * shade);
	b = (int)((col & 0xFF) * shade);
	return ((r << 16) | (g << 8) | b);
}

static void	loop_draw_obj_stripe(t_engine *eng, t_obj_draw *d, double step[2])
{
	int				y;
	unsigned int	col;

	y = d->coords[0] - 1;
	while (++y <= d->coords[1])
	{
		d->coords[3] = (int)step[0];
		step[0] += step[1];
		if (d->coords[3] < 0 || d->coords[3] >= d->tex->height)
			continue ;
		col = *(unsigned int *)(d->tex->addr + d->coords[3] * d->tex->line_len
				+ d->coords[2] * (d->tex->bpp / 8));
		if (col != get_transparent_color(d->tex)
			&& (col & 0xFFFFFF) != 0xFFFFFF)
			put_pixel(&eng->screen->img2, d->stripe, y,
				calc_obj_color(col, d->shade));
	}
}

void	draw_object_stripe(t_engine *eng, int stripe, int *w_h,
			t_sprite *s)
{
	t_obj_draw	d;
	double		step[3];

	if (s->sym != 'L' && s->sym != 'X' && s->sym != 'T')
		return ;
	if (w_h[0] <= 0)
		return ;
	d.sym = s->sym;
	d.stripe = stripe;
	d.shade = s->shade;
	d.tex = get_obj_tex_and_coords(eng, s->sym, w_h, d.coords);
	step[2] = (double)eng->screen->win_height;
	if (!setup_obj_draw_params(stripe, w_h, &d, step))
		return ;
	loop_draw_obj_stripe(eng, &d, step);
}
