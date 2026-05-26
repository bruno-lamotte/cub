#include "cub.h"

static void	init_light_properties(t_engine *eng, t_light *l, int idx,
				char sym)
{
	if (sym == 'T')
	{
		l->intensity = 0.4f;
		l->radius = 2.0f;
		l->is_alarm = 0;
		l->is_active = 1;
	}
	else
	{
		l->intensity = 0.8f;
		l->radius = 4.0f;
		l->is_alarm = 1;
		l->is_active = eng->data->obj_defs[idx].pad;
	}
	l->is_triggered = 0;
}

static void	add_light_at_pos(t_engine *eng, int x, int y, int w)
{
	t_light	*l;
	uint8_t	*occ;
	int		idx;

	occ = get_map_occ_ids(eng->blob);
	l = &eng->static_lights[eng->static_light_count++];
	l->x = x + 0.5;
	l->y = y + 0.5;
	l->angle = 0.0f;
	idx = occ[y * w + x];
	init_light_properties(eng, l, idx, eng->data->obj_defs[idx].symbol);
}

void	init_static_lights(t_engine *eng)
{
	uint8_t	*flags;
	int		w;
	int		pos[2];

	flags = get_map_flags(eng->blob);
	w = get_map_width(get_blob_hdr(eng->blob));
	eng->static_light_count = 0;
	eng->alarm_triggered = 0;
	eng->hacking_timer = 0;
	pos[1] = -1;
	while (++pos[1] < get_map_height(get_blob_hdr(eng->blob)))
	{
		pos[0] = -1;
		while (++pos[0] < w)
		{
			if (flags[pos[1] * w + pos[0]] & CELL_HAS_OBJ
				&& eng->static_light_count < 64)
				add_light_at_pos(eng, pos[0], pos[1], w);
		}
	}
	update_global_alarm_state(eng);
}

static int	check_los_shadow(double mx, double my, double px, double py)
{
	double	fx;
	double	fy;

	fx = px - (int)px;
	fy = py - (int)py;
	if (fabs(fx - 0.99) < 1e-9 && mx >= (int)px + 1)
		return (0);
	if (fabs(fx - 0.01) < 1e-9 && mx <= (int)px)
		return (0);
	if (fabs(fy - 0.99) < 1e-9 && my >= (int)py + 1)
		return (0);
	if (fabs(fy - 0.01) < 1e-9 && my <= (int)py)
		return (0);
	return (1);
}

static void	init_los_dda(double mx, double my, double px, double py,
				double d[4], double side[2], int step[4], int target[2])
{
	d[0] = px - mx;
	d[1] = py - my;
	d[2] = (d[0] == 0) ? 1e30 : fabs(1.0 / d[0]);
	d[3] = (d[1] == 0) ? 1e30 : fabs(1.0 / d[1]);
	step[2] = (int)mx;
	step[3] = (int)my;
	target[0] = (int)px;
	target[1] = (int)py;
	step[0] = (d[0] < 0) ? -1 : 1;
	step[1] = (d[1] < 0) ? -1 : 1;
	side[0] = (d[0] < 0) ? (mx - step[2]) * d[2] : (step[2] + 1.0 - mx) * d[2];
	side[1] = (d[1] < 0) ? (my - step[3]) * d[3] : (step[3] + 1.0 - my) * d[3];
}

static void	dda_step(double d[4], double side[2], int step[4])
{
	if (side[0] < side[1])
	{
		side[0] += d[2];
		step[2] += step[0];
	}
	else
	{
		side[1] += d[3];
		step[3] += step[1];
	}
}

int	check_los(double mx, double my, double px, double py, void *blob)
{
	double	d[4];
	double	side[2];
	int		step[4];
	int		target[2];
	int		w_h[2];

	if (!check_los_shadow(mx, my, px, py))
		return (0);
	init_los_dda(mx, my, px, py, d, side, step, target);
	w_h[0] = get_map_width(get_blob_hdr(blob));
	w_h[1] = get_map_height(get_blob_hdr(blob));
	while (step[2] != target[0] || step[3] != target[1])
	{
		dda_step(d, side, step);
		if (step[2] == target[0] && step[3] == target[1])
			break ;
		if (!is_walkable(step[2], step[3], w_h[0], w_h[1], blob))
			return (0);
	}
	return (1);
}

static float	get_player_light(double wx, double wy, t_player_rt *p,
					void *blob)
{
	double	dx;
	double	dy;
	float	d2;
	float	d;

	dx = wx - p->pos.x;
	dy = wy - p->pos.y;
	d2 = (float)(dx * dx + dy * dy);
	if (d2 >= 36.0f)
		return (0.0f);
	if (!check_los(p->pos.x, p->pos.y, wx, wy, blob))
		return (0.0f);
	d = d2 * fast_inv_sqrt(d2);
	return ((float)((1.0 - d / 6.0) * (1.0 - d / 6.0)));
}

static float	get_mstr_light(double wx, double wy, t_monster_rt *m,
					void *blob)
{
	double	dx;
	double	dy;
	float	d2;
	float	inv_d;
	float	d;
	double	cos_a;

	dx = wx - m->pos.x;
	dy = wy - m->pos.y;
	d2 = (float)(dx * dx + dy * dy);
	if (d2 >= 36.0f || d2 < 0.01f)
		return (0.0f);
	inv_d = fast_inv_sqrt(d2);
	cos_a = (dx * m->dir.x + dy * m->dir.y) * inv_d;
	if (cos_a < 0.866)
		return (0.0f);
	if (!check_los(m->pos.x, m->pos.y, wx, wy, blob))
		return (0.0f);
	d = d2 * inv_d;
	return ((float)((1.0 - d / 6.0) * (1.0 - d / 6.0)
		* ((cos_a - 0.866) / (1.0 - 0.866))));
}

float	compute_light_at_point(double wx, double wy,
			t_ray_data *ray __attribute__((unused)),
			t_lut *lut __attribute__((unused)), void *blob, t_engine *eng)
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
		if (d2 < l->radius * l->radius && check_los(l->x, l->y, wx, wy, blob)
			&& l->is_active)
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
		if (d2 < l->radius * l->radius && check_los(l->x, l->y, wx, wy, blob))
		{
			alarm_light += 1.5f * (0.6f + 0.5f
					* sinf(eng->pool.current_frame * 0.15f))
				* (1.0f - d2 * fast_inv_sqrt(d2) / 6.0f)
				* (1.0f - d2 * fast_inv_sqrt(d2) / 6.0f);
		}
	}
	return (alarm_light);
}
