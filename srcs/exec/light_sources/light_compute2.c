/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   light_compute2.c                                   :+:      :+:    :+:   */
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

float	compute_light_at_point(double wx, double wy, void *blob, t_engine *eng)
{
	float	light;
	float	fx;
	float	fy;

	fx = (float)wx;
	fy = (float)wy;
	light = get_player_light(fx, fy, eng->player, blob);
	light += accumulate_monster_light(fx, fy, eng, blob);
	light += accumulate_static_lights(fx, fy, eng, blob);
	return (light);
}

static float	add_alarm_light(float d2, t_light *l, float factor)
{
	float	attenuation;

	(void)l;
	attenuation = 1.0f - d2 * fast_inv_sqrt(d2) / 6.0f;
	if (attenuation > 0.0f)
		return (factor * (attenuation * attenuation));
	return (0.0f);
}

static float	accumulate_alarm_lights(float fx, float fy,
					t_engine *eng, void *blob)
{
	t_light	*l;
	float	d2;
	float	light;
	float	factor;
	int		i;

	light = 0.0f;
	factor = 1.5f * (0.6f + 0.5f * sinf(eng->pool.current_frame * 0.15f));
	i = -1;
	while (++i < eng->static_light_count)
	{
		l = &eng->static_lights[i];
		if (!l->is_alarm || !l->is_triggered)
			continue ;
		d2 = (fx - l->x) * (fx - l->x) + (fy - l->y) * (fy - l->y);
		if (d2 >= l->radius_sq)
			continue ;
		if (!check_los(make_vec2(l->x, l->y), make_vec2(fx, fy), blob))
			continue ;
		light += add_alarm_light(d2, l, factor);
	}
	return (light);
}

float	get_alarm_light_at_point(double wx, double wy, void *blob,
			t_engine *eng)
{
	if (!eng->alarm_triggered)
		return (0.0f);
	return (accumulate_alarm_lights((float)wx, (float)wy, eng, blob));
}
