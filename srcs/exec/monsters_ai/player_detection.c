/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   player_detection.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: blamotte <blamotte@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/11 01:39:07 by blamotte          #+#    #+#             */
/*   Updated: 2026/08/11 02:00:00 by blamotte         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub.h"

static t_vec2	make_vec2(double x, double y)
{
	t_vec2	v;

	v.d.x = x;
	v.d.y = y;
	return (v);
}

static int	is_player_in_light(t_player_rt *p, t_engine *eng)
{
	return (compute_light_at_point(p->pos.d.x, p->pos.d.y,
			eng->blob, eng) > 1.05f);
}

static int	is_player_in_external_light(t_player_rt *p, t_engine *eng)
{
	int		i;
	t_light	*l;
	float	d2;
	float	px;
	float	py;

	px = (float)p->pos.d.x;
	py = (float)p->pos.d.y;
	i = -1;
	while (++i < eng->static_light_count)
	{
		l = &eng->static_lights[i];
		d2 = (px - l->x) * (px - l->x) + (py - l->y) * (py - l->y);
		if (d2 >= l->radius_sq)
			continue ;
		if (l->is_alarm && l->is_triggered
			&& check_los(make_vec2(l->x, l->y), p->pos, eng->blob))
			return (1);
		if (!l->is_alarm && l->is_active
			&& check_los(make_vec2(l->x, l->y), p->pos, eng->blob))
			return (1);
	}
	return (0);
}

static int	check_los_and_fov(t_monster_rt *m, t_player_rt *p,
				double cos_a, void *blob)
{
	return (cos_a >= 0.866 && check_los(m->pos, p->pos, blob));
}

int	detect_player(t_monster_rt *m, t_engine *eng)
{
	double		d[2];
	float		d2;
	float		limit;
	t_player_rt	*p;

	p = eng->player;
	d[0] = p->pos.d.x - m->pos.d.x;
	d[1] = p->pos.d.y - m->pos.d.y;
	d2 = (float)(d[0] * d[0] + d[1] * d[1]);
	if (d2 < 0.01f)
		return (1);
	if (is_player_in_external_light(p, eng)
		&& check_los_and_fov(m, p, (d[0] * m->dir.d.x + d[1] * m->dir.d.y)
			* fast_inv_sqrt(d2), eng->blob))
		return (1);
	limit = PLAYER_DET_DARK_MAX_DIST_SQ;
	if (is_player_in_light(p, eng))
		limit = PLAYER_DET_LIGHT_MAX_DIST_SQ;
	if (d2 >= limit)
		return (0);
	if (d2 < PLAYER_DET_CLOSE_DIST_SQ && check_los(m->pos, p->pos, eng->blob))
		return (1);
	return (check_los_and_fov(m, p, (d[0] * m->dir.d.x + d[1] * m->dir.d.y)
			* fast_inv_sqrt(d2), eng->blob));
}
