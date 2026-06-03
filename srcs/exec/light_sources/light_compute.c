#include "cub.h"
#include <math.h>

static float	get_player_light(float fx, float fy, t_player_rt *p,
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
	if (!check_los(p->pos, (t_vec2){.d = {fx, fy}}, blob))
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
	float	dist;
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
	if (!check_los(m->pos, (t_vec2){.d = {fx, fy}}, blob))
		return (0.0f);
	dist = d2 * inv_d;
	return ((float)((1.0 - dist / 6.0) * (1.0 - dist / 6.0)
		* ((cos_a - 0.866) / (1.0 - 0.866))));
}

static inline float	accumulate_monster_light(float fx, float fy,
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

static inline float	add_static_light(float d2, t_light *l)
{
	float	attenuation;

	attenuation = 1.0f - d2 * fast_inv_sqrt(d2) / l->radius;
	if (attenuation > 0.0f)
		return (l->intensity * (attenuation * attenuation));
	return (0.0f);
}

static inline float	accumulate_static_lights(float fx, float fy,
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
		if (!check_los((t_vec2){.d = {l->x, l->y}},
			(t_vec2){.d = {fx, fy}}, blob))
			continue ;
		light += add_static_light(d2, l);
	}
	return (light);
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

static inline float	add_alarm_light(float d2, t_light *l, float factor)
{
	float	attenuation;

	(void)l;
	attenuation = 1.0f - d2 * fast_inv_sqrt(d2) / 6.0f;
	if (attenuation > 0.0f)
		return (factor * (attenuation * attenuation));
	return (0.0f);
}

static inline float	accumulate_alarm_lights(float fx, float fy,
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
		if (!check_los((t_vec2){.d = {l->x, l->y}},
			(t_vec2){.d = {fx, fy}}, blob))
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
