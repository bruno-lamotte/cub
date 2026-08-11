/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   draw_floor.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: blamotte <blamotte@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/11 00:33:49 by blamotte          #+#    #+#             */
/*   Updated: 2026/08/11 00:33:49 by blamotte         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub.h"

static void	draw_interpolated_pixels(t_worker *w, t_floor_row *r,
				int xs[2], float sh_se[2][2])
{
	float	sh[2];
	double	t;
	double	den;
	int		x;

	den = xs[1] - xs[0] + (xs[0] == xs[1]);
	x = xs[0] - 1;
	while (++x <= xs[1])
	{
		t = (double)(x - xs[0]) / den;
		sh[0] = sh_se[0][0] + t * (sh_se[1][0] - sh_se[0][0]);
		sh[1] = sh_se[0][1] + t * (sh_se[1][1] - sh_se[0][1]);
		draw_floor_pixel(w, r, x, sh);
	}
}

static void	draw_exact_pixels(t_worker *w, t_floor_row *r,
				int x_start, int x_end)
{
	float	sh[2];
	double	px;
	double	py;
	int		x;

	x = x_start - 1;
	while (++x <= x_end)
	{
		px = r->fx + (x - w->start_x) * r->step_x;
		py = r->fy + (x - w->start_x) * r->step_y;
		sh[0] = compute_light_at_point(px, py, w->blob, w->engine);
		sh[1] = get_alarm_light_at_point(px, py, w->blob, w->engine);
		draw_floor_pixel(w, r, x, sh);
	}
}

static void	draw_floor_segment(t_worker *w, t_floor_row *r,
				int x_start, int x_end)
{
	double	pos[4];
	float	sh_se[2][2];
	int		xs[2];

	pos[0] = r->fx + (x_start - w->start_x) * r->step_x;
	pos[1] = r->fy + (x_start - w->start_x) * r->step_y;
	pos[2] = r->fx + (x_end - w->start_x) * r->step_x;
	pos[3] = r->fy + (x_end - w->start_x) * r->step_y;
	sh_se[0][0] = compute_light_at_point(pos[0], pos[1], w->blob, w->engine);
	sh_se[0][1] = get_alarm_light_at_point(pos[0], pos[1], w->blob, w->engine);
	sh_se[1][0] = compute_light_at_point(pos[2], pos[3], w->blob, w->engine);
	sh_se[1][1] = get_alarm_light_at_point(pos[2], pos[3], w->blob, w->engine);
	if (get_diff(sh_se[0][0], sh_se[1][0]) > 0.02f
		|| get_diff(sh_se[0][1], sh_se[1][1]) > 0.02f)
		draw_exact_pixels(w, r, x_start, x_end);
	else
	{
		xs[0] = x_start;
		xs[1] = x_end;
		draw_interpolated_pixels(w, r, xs, sh_se);
	}
}

static void	draw_floor_row(t_worker *w, t_floor_row *r)
{
	int	x_start;
	int	x_end;

	x_start = w->start_x;
	while (x_start < w->end_x)
	{
		x_end = x_start + 4;
		if (x_end >= w->end_x)
			x_end = w->end_x - 1;
		draw_floor_segment(w, r, x_start, x_end);
		x_start = x_end + 1;
	}
}

void	render_floor_slice(t_worker *w)
{
	t_floor_row	r;
	t_lut		*lut;
	int			y;

	lut = get_lut_bdef(w->blob);
	y = w->screen->win_height / 2 - 1;
	while (++y < w->screen->win_height)
	{
		init_row_params(w, y, &r, lut);
		draw_floor_row(w, &r);
	}
}
