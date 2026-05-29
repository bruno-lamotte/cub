#include "cub.h"
#include <math.h>

static float	get_player_light(double wx, double wy, t_player_rt *p,
					void *blob)
{
	t_vec2	d;
	float	d2;
	float	dist;

	d.d.x = wx - p->pos.d.x;
	d.d.y = wy - p->pos.d.y;
	d2 = (float)(d.d.x * d.d.x + d.d.y * d.d.y);
	if (d2 >= 36.0f)
		return (0.0f);
	if (!check_los(p->pos, (t_vec2){.d = {wx, wy}}, blob))
		return (0.0f);
	dist = d2 * fast_inv_sqrt(d2);
	return ((float)((1.0 - dist / 6.0) * (1.0 - dist / 6.0)));
}

static float	get_mstr_light(double wx, double wy, t_monster_rt *m,
					void *blob)
{
	t_vec2	d;
	float	d2;
	float	inv_d;
	float	dist;
	double	cos_a;

	d.d.x = wx - m->pos.d.x;
	d.d.y = wy - m->pos.d.y;
	d2 = (float)(d.d.x * d.d.x + d.d.y * d.d.y);
	if (d2 >= 36.0f || d2 < 0.01f)
		return (0.0f);
	inv_d = fast_inv_sqrt(d2);
	cos_a = (d.d.x * m->dir.d.x + d.d.y * m->dir.d.y) * inv_d;
	if (cos_a < 0.866)
		return (0.0f);
	if (!check_los(m->pos, (t_vec2){.d = {wx, wy}}, blob))
		return (0.0f);
	dist = d2 * inv_d;
	return ((float)((1.0 - dist / 6.0) * (1.0 - dist / 6.0)
		* ((cos_a - 0.866) / (1.0 - 0.866))));
}

float	compute_light_at_point(double wx, double wy, void *blob, t_engine *eng)
{
	float			light;
	t_monster_rt	*m;
	t_light			*l;
	int				i;
	float			d2;

	light = get_player_light(wx, wy, eng->player, blob);
	m = get_monster_rt(blob);
	i = -1;
	while (++i < eng->data->monster_rt_count)
		if (!(m[i].flags & MONSTER_DEAD))
			light += get_mstr_light(wx, wy, &m[i], blob);
	i = -1;
	while (++i < eng->static_light_count)
	{
		l = &eng->static_lights[i];
		if (l->is_alarm && l->is_triggered)
			continue ;
		d2 = (float)((wx - l->x) * (wx - l->x) + (wy - l->y) * (wy - l->y));
		if (d2 < l->radius * l->radius && check_los((t_vec2){.d = {l->x, l->y}},
			(t_vec2){.d = {wx, wy}}, blob) && l->is_active)
			light += l->intensity * (1.0f - d2 * fast_inv_sqrt(d2) / l->radius)
				* (1.0f - d2 * fast_inv_sqrt(d2) / l->radius);
	}
	return (light);
}

float	get_alarm_light_at_point(double wx, double wy, void *blob,
			t_engine *eng)
{
	float	alarm_light;
	t_light	*l;
	int		i;
	float	d2;

	if (!eng->alarm_triggered)
		return (0.0f);
	alarm_light = 0.0f;
	i = -1;
	while (++i < eng->static_light_count)
	{
		l = &eng->static_lights[i];
		if (!l->is_alarm || !l->is_triggered)
			continue ;
		d2 = (float)((wx - l->x) * (wx - l->x) + (wy - l->y) * (wy - l->y));
		if (d2 < l->radius * l->radius && check_los((t_vec2){.d = {l->x, l->y}},
			(t_vec2){.d = {wx, wy}}, blob))
		{
			alarm_light += 1.5f * (0.6f + 0.5f
					* sinf(eng->pool.current_frame * 0.15f))
				* (1.0f - d2 * fast_inv_sqrt(d2) / 6.0f)
				* (1.0f - d2 * fast_inv_sqrt(d2) / 6.0f);
		}
	}
	return (alarm_light);
}
