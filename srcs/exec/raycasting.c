/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   raycasting.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: blamotte <blamotte@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/11 00:33:49 by blamotte          #+#    #+#             */
/*   Updated: 2026/08/11 02:00:00 by blamotte         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub.h"
#include <math.h>

static void	init_ray_steps(t_player_rt *p, t_ray_data *out, t_dda_state *s)
{
	if (s->ray_dir_x < 0.0)
	{
		s->step_x = -1;
		s->side_dist_x = (p->pos.d.x - (double)out->map_x) * s->delta_dist_x;
	}
	else
	{
		s->step_x = 1;
		s->side_dist_x = ((double)out->map_x + 1.0 - p->pos.d.x)
			* s->delta_dist_x;
	}
	if (s->ray_dir_y < 0.0)
	{
		s->step_y = -1;
		s->side_dist_y = (p->pos.d.y - (double)out->map_y) * s->delta_dist_y;
	}
	else
	{
		s->step_y = 1;
		s->side_dist_y = ((double)out->map_y + 1.0 - p->pos.d.y)
			* s->delta_dist_y;
	}
}

void	init_ray(double cam_x, t_player_rt *p, t_ray_data *out,
			t_dda_state *s)
{
	s->ray_dir_x = p->dir.d.x + p->plane.d.x * cam_x;
	s->ray_dir_y = p->dir.d.y + p->plane.d.y * cam_x;
	out->map_x = (int)p->pos.d.x;
	out->map_y = (int)p->pos.d.y;
	if (s->ray_dir_x == 0.0)
		s->delta_dist_x = 1e30;
	else
		s->delta_dist_x = fabs(1.0 / s->ray_dir_x);
	if (s->ray_dir_y == 0.0)
		s->delta_dist_y = 1e30;
	else
		s->delta_dist_y = fabs(1.0 / s->ray_dir_y);
	init_ray_steps(p, out, s);
	s->hit = 0;
	s->side = 0;
}

static void	dda_single_step(t_ray_data *out, t_dda_state *s)
{
	if (s->side_dist_x < s->side_dist_y)
	{
		s->side = 0;
		s->side_dist_x += s->delta_dist_x;
		out->map_x += s->step_x;
	}
	else
	{
		s->side = 1;
		s->side_dist_y += s->delta_dist_y;
		out->map_y += s->step_y;
	}
}

static void	sub_perform_dda(t_ray_data *out, t_dda_state *s, t_player_rt *p,
				void *blob)
{
	int		w;
	int		idx;
	uint8_t	*flg;

	w = get_map_width(get_blob_hdr(blob));
	flg = get_map_flags(blob);
	if (out->map_x < 0 || out->map_x >= w || out->map_y < 0
		|| out->map_y >= get_map_height(get_blob_hdr(blob)))
	{
		out->hit_type = 0;
		s->hit = 1;
		return ;
	}
	idx = out->map_y * w + out->map_x;
	if (flg[idx] & CELL_HAS_WALL)
		out->hit_type = get_map_block_ids(blob)[idx];
	if (flg[idx] & CELL_HAS_WALL)
		s->hit = 1;
	out->ray_dir.fp.x = double_to_fp(s->ray_dir_x);
	out->ray_dir.fp.y = double_to_fp(s->ray_dir_y);
	if ((flg[idx] & CELL_HAS_DOOR) && check_door_hit(out, p, blob))
	{
		out->hit_type = get_map_occ_ids(blob)[idx];
		s->hit = 1;
	}
}

void	perform_dda(t_ray_data *out, t_dda_state *s, t_player_rt *p,
			void *blob)
{
	while (s->hit == 0)
	{
		dda_single_step(out, s);
		sub_perform_dda(out, s, p, blob);
	}
}
