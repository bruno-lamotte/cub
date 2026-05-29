#include "cub.h"

static int	is_player_in_light(t_player_rt *p, t_engine *eng)
{
	return (compute_light_at_point(p->pos.d.x, p->pos.d.y,
			eng->blob, eng) > 0.05f);
}

static int	is_player_in_external_light(t_player_rt *p, t_engine *eng)
{
	int		i;
	t_light	*l;
	double	d2;

	i = -1;
	while (++i < eng->static_light_count)
	{
		l = &eng->static_lights[i];
		d2 = (p->pos.d.x - l->x) * (p->pos.d.x - l->x)
			+ (p->pos.d.y - l->y) * (p->pos.d.y - l->y);
		if (d2 >= l->radius * l->radius)
			continue ;
		if (l->is_alarm && l->is_triggered
			&& check_los((t_vec2){.d = {l->x, l->y}}, p->pos, eng->blob))
			return (1);
		if (!l->is_alarm && l->is_active
			&& check_los((t_vec2){.d = {l->x, l->y}}, p->pos, eng->blob))
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
		&& check_los_and_fov(m, p, (d[0] * m->dir.d.x + d[1] * m->dir.d.y) * fast_inv_sqrt(d2), eng->blob))
		return (1);
	if (is_player_in_light(p, eng))
		limit = PLAYER_DET_LIGHT_MAX_DIST_SQ;
	else
		limit = PLAYER_DET_DARK_MAX_DIST_SQ;
	if (d2 >= limit)
		return (0);
	if (d2 < PLAYER_DET_CLOSE_DIST_SQ && check_los(m->pos, p->pos, eng->blob))
		return (1);
	return (check_los_and_fov(m, p, (d[0] * m->dir.d.x + d[1] * m->dir.d.y) * fast_inv_sqrt(d2), eng->blob));
}
