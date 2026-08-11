/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   draw_floor_utils.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: blamotte <blamotte@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/11 00:33:49 by blamotte          #+#    #+#             */
/*   Updated: 2026/08/11 02:00:00 by blamotte         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub.h"

static void	calc_cam_dirs(t_worker *w, double cam[2], double dir[4])
{
	cam[0] = 2.0 * w->start_x / (double)w->screen->win_width - 1.0;
	dir[0] = w->player->dir.d.x + w->player->plane.d.x * cam[0];
	dir[1] = w->player->dir.d.y + w->player->plane.d.y * cam[0];
	cam[1] = 2.0 * (w->end_x - 1) / (double)w->screen->win_width - 1.0;
	dir[2] = w->player->dir.d.x + w->player->plane.d.x * cam[1];
	dir[3] = w->player->dir.d.y + w->player->plane.d.y * cam[1];
}

void	init_row_params(t_worker *w, int y, t_floor_row *r, t_lut *lut)
{
	double	cam[2];
	double	dir[4];
	double	f_start[2];
	double	f_end[2];
	int		len;

	r->y = y;
	r->p0 = (double)w->screen->win_height
		/ (2.0 * y - (double)w->screen->win_height);
	r->ambient_shade = lut->shade_table[clamp_idx((int)(r->p0 * 10.0))] * 0.2f;
	calc_cam_dirs(w, cam, dir);
	f_start[0] = w->player->pos.d.x + r->p0 * dir[0];
	f_start[1] = w->player->pos.d.y + r->p0 * dir[1];
	f_end[0] = w->player->pos.d.x + r->p0 * dir[2];
	f_end[1] = w->player->pos.d.y + r->p0 * dir[3];
	len = w->end_x - w->start_x;
	r->step_x = (f_end[0] - f_start[0]) / (len - 1);
	r->step_y = (f_end[1] - f_start[1]) / (len - 1);
	r->fx = f_start[0];
	r->fy = f_start[1];
}

float	get_diff(float a, float b)
{
	float	diff;

	diff = a - b;
	if (diff < 0.0f)
		return (-diff);
	return (diff);
}
