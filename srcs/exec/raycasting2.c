/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   raycasting2.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: blamotte <blamotte@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/11 00:33:49 by blamotte          #+#    #+#             */
/*   Updated: 2026/08/11 00:33:49 by blamotte         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub.h"
#include <math.h>

static double	get_dda_dist(t_dda_state *s)
{
	double	dist;

	if (s->side == 0)
		dist = s->side_dist_x - s->delta_dist_x;
	else
		dist = s->side_dist_y - s->delta_dist_y;
	if (dist <= 0.0)
		return (0.1);
	if (dist > 30000.0)
		return (30000.0);
	return (dist);
}

static void	fill_ray_out(t_ray_data *out, t_dda_state *s, t_player_rt *p,
				void *blob)
{
	double	dist;
	double	wx;
	int		w;
	int		h;

	w = get_map_width(get_blob_hdr(blob));
	h = get_map_height(get_blob_hdr(blob));
	if (out->map_x >= 0 && out->map_x < w && out->map_y >= 0 && out->map_y < h
		&& (get_map_flags(blob)[out->map_y * w + out->map_x] & CELL_HAS_WALL))
	{
		dist = get_dda_dist(s);
		if (s->side == 0)
			wx = p->pos.d.y + dist * s->ray_dir_y;
		else
			wx = p->pos.d.x + dist * s->ray_dir_x;
		wx -= floor(wx);
		out->perp_wall_dist = double_to_fp(dist);
		out->side = s->side;
		out->wall_x = double_to_fp(wx);
	}
	out->ray_dir.fp.x = double_to_fp(s->ray_dir_x);
	out->ray_dir.fp.y = double_to_fp(s->ray_dir_y);
}

void	cast_ray(t_worker *w, int x, t_ray_data *out)
{
	t_dda_state	s;
	double		cam_x;

	cam_x = 2.0 * x / (double)w->screen->win_width - 1.0;
	init_ray(cam_x, w->player, out, &s);
	perform_dda(out, &s, w->player, w->blob);
	fill_ray_out(out, &s, w->player, w->blob);
}
