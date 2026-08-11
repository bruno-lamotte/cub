/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   light_compute.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: blamotte <blamotte@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/11 00:33:49 by blamotte          #+#    #+#             */
/*   Updated: 2026/08/11 02:00:00 by blamotte         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub.h"
#include <math.h>

static t_vec2	make_vec2(double x, double y)
{
	t_vec2	v;

	v.d.x = x;
	v.d.y = y;
	return (v);
}

float	get_player_light(float fx, float fy, t_player_rt *p,
			void *blob)
{
	t_vec2	d;
	float	d2;
	float	dist;

	d.d.x = (double)fx - p->pos.d.x;
	d.d.y = (double)fy - p->pos.d.y;
	d2 = (float)(d.d.x * d.d.x + d.d.y * d.d.y);
	if (d2 >= 36.0f)
		return (0.0f);
	if (!check_los(p->pos, make_vec2(fx, fy), blob))
		return (0.0f);
	dist = d2 * fast_inv_sqrt(d2);
	return ((float)((1.0 - dist / 6.0) * (1.0 - dist / 6.0)));
}

static float	get_mstr_light(float fx, float fy, t_monster_rt *m,
					void *blob)
{
	t_vec2	d;
	float	d2;
	float	inv_d;
	double	cos_a;

	d.d.x = (double)fx - m->pos.d.x;
	d.d.y = (double)fy - m->pos.d.y;
	d2 = (float)(d.d.x * d.d.x + d.d.y * d.d.y);
	if (d2 >= 36.0f || d2 < 0.01f)
		return (0.0f);
	inv_d = fast_inv_sqrt(d2);
	cos_a = (d.d.x * m->dir.d.x + d.d.y * m->dir.d.y) * inv_d;
	if (cos_a < 0.866)
		return (0.0f);
	if (!check_los(m->pos, make_vec2(fx, fy), blob))
		return (0.0f);
	return ((float)((1.0 - (d2 * inv_d) / 6.0) * (1.0 - (d2 * inv_d) / 6.0)
		* ((cos_a - 0.866) / (1.0 - 0.866))));
}

float	accumulate_monster_light(float fx, float fy,
			t_engine *eng, void *blob)
{
	t_monster_rt	*m;
	int				i;
	float			light;

	m = get_monster_rt(blob);
	light = 0.0f;
	i = -1;
	while (++i < eng->data->monster_rt_count)
	{
		if (!(m[i].flags & MONSTER_DEAD))
			light += get_mstr_light(fx, fy, &m[i], blob);
	}
	return (light);
}

float	accumulate_static_lights(float fx, float fy,
			t_engine *eng, void *blob)
{
	t_light	*l;
	float	d2;
	float	light;
	int		i;

	light = 0.0f;
	i = -1;
	while (++i < eng->static_light_count)
	{
		l = &eng->static_lights[i];
		if (!l->is_active || (l->is_alarm && l->is_triggered))
			continue ;
		d2 = (fx - l->x) * (fx - l->x) + (fy - l->y) * (fy - l->y);
		if (d2 >= l->radius_sq)
			continue ;
		if (!check_los(make_vec2(l->x, l->y), make_vec2(fx, fy), blob))
			continue ;
		if (1.0f - d2 * fast_inv_sqrt(d2) / l->radius > 0.0f)
			light += l->intensity * ((1.0f - d2 * fast_inv_sqrt(d2)
						/ l->radius) * (1.0f - d2 * fast_inv_sqrt(d2)
						/ l->radius));
	}
	return (light);
}
