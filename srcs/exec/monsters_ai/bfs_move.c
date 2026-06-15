#include "cub.h"
#include <math.h>

t_vec2	find_next_step(t_monster_rt *m, t_engine *eng, t_vec2 target, t_worker *w)
{
	t_vec2	start;
	t_vec2	tgt;
	int		wdt;
	int		next_idx;

	start.i.x = (int)m->pos.d.x;
	start.i.y = (int)m->pos.d.y;
	tgt.i.x = (int)target.d.x;
	tgt.i.y = (int)target.d.y;
	wdt = get_map_width(get_blob_hdr(eng->blob));
	init_bfs_arrays(w->bfs_parent, wdt * get_map_height(get_blob_hdr(eng->blob)));
	if (bfs_run(start, tgt, eng, w))
	{
		next_idx = bfs_backtrack(start.i.y * wdt + start.i.x,
				tgt.i.y * wdt + tgt.i.x, w->bfs_parent);
		start.d.x = (next_idx % wdt) + 0.5;
		start.d.y = (next_idx / wdt) + 0.5;
	}
	else
		start = target;
	return (start);
}

void	mstr_move_towards(t_monster_rt *m, t_engine *eng, t_vec2 target)
{
	t_vec2	d;
	float	len2;
	int		self_idx;

	self_idx = m - get_monster_rt(eng->blob);
	d.d.x = target.d.x - m->pos.d.x;
	d.d.y = target.d.y - m->pos.d.y;
	len2 = (float)(d.d.x * d.d.x + d.d.y * d.d.y);
	if (len2 > 0.01f)
	{
		m->dir.d.x = d.d.x * fast_inv_sqrt(len2);
		m->dir.d.y = d.d.y * fast_inv_sqrt(len2);
	}
	d.d.x = m->pos.d.x + m->dir.d.x * MSTR_PATROL_SPEED;
	d.d.y = m->pos.d.y + m->dir.d.y * MSTR_PATROL_SPEED;
	if (is_valid_position(d.d.x, m->pos.d.y, eng->blob)
		&& check_monster_collision(d.d.x, m->pos.d.y, self_idx, eng))
		m->pos.d.x = d.d.x;
	if (is_valid_position(m->pos.d.x, d.d.y, eng->blob)
		&& check_monster_collision(m->pos.d.x, d.d.y, self_idx, eng))
		m->pos.d.y = d.d.y;
}

static void	check_alarm_dist(t_monster_rt *m, t_light *l, t_vec2 *ap, double *c)
{
	double	dist2;

	dist2 = (l->x - m->pos.d.x) * (l->x - m->pos.d.x)
		+ (l->y - m->pos.d.y) * (l->y - m->pos.d.y);
	if (dist2 < *c)
	{
		*c = dist2;
		ap->d.x = l->x;
		ap->d.y = l->y;
	}
}

void	find_closest_alarm(t_monster_rt *m, t_engine *eng, t_vec2 *alarm_pos)
{
	int		i;
	double	closest;

	alarm_pos->d.x = -1.0;
	alarm_pos->d.y = -1.0;
	closest = 1e9;
	i = -1;
	while (++i < eng->static_light_count)
	{
		if (eng->static_lights[i].is_alarm
			&& eng->static_lights[i].is_triggered)
			check_alarm_dist(m, &eng->static_lights[i], alarm_pos, &closest);
	}
}

void	mstr_go_to_alarm(t_monster_rt *m, t_engine *eng, t_vec2 alarm_pos, t_worker *w)
{
	t_vec2	next;

	next = find_next_step(m, eng, alarm_pos, w);
	mstr_move_towards(m, eng, next);
}
