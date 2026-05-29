#include "cub.h"
#include <math.h>

void	mstr_go_to_guard(t_monster_rt *m, t_engine *eng, t_worker *w)
{
	t_vec2	d;
	t_vec2	next;
	double	old;

	d.d.x = m->guard.d.x - m->pos.d.x;
	d.d.y = m->guard.d.y - m->pos.d.y;
	if (d.d.x * d.d.x + d.d.y * d.d.y < MSTR_DEST_REACHED_DIST_SQ)
	{
		m->pos.d.x = m->guard.d.x;
		m->pos.d.y = m->guard.d.y;
		old = m->dir.d.x;
		m->dir.d.x = m->dir.d.x * cos(MSTR_ROTATION_STEP) - m->dir.d.y * sin(MSTR_ROTATION_STEP);
		m->dir.d.y = old * sin(MSTR_ROTATION_STEP) + m->dir.d.y * cos(MSTR_ROTATION_STEP);
		return ;
	}
	if (m->alert_timer > MSTR_ALERT_PATROL_TIMEOUT)
	{
		assign_new_patrol_target(m, eng, w);
		m->alert_timer = 0;
		return ;
	}
	next = find_next_step(m, eng, m->guard, w);
	mstr_move_towards(m, eng, next);
}

void	mstr_chase(t_monster_rt *m, t_engine *eng)
{
	t_vec2	d;
	float	len2;
	int		self_idx;

	self_idx = m - get_monster_rt(eng->blob);
	d.d.x = eng->player->pos.d.x - m->pos.d.x;
	d.d.y = eng->player->pos.d.y - m->pos.d.y;
	len2 = (float)(d.d.x * d.d.x + d.d.y * d.d.y);
	if (len2 > 0.01f)
	{
		m->dir.d.x = d.d.x * fast_inv_sqrt(len2);
		m->dir.d.y = d.d.y * fast_inv_sqrt(len2);
	}
	d.d.x = m->pos.d.x + m->dir.d.x * MSTR_CHASE_SPEED;
	d.d.y = m->pos.d.y + m->dir.d.y * MSTR_CHASE_SPEED;
	if (is_valid_position(d.d.x, m->pos.d.y, eng->blob)
		&& check_monster_collision(d.d.x, m->pos.d.y, self_idx, eng))
		m->pos.d.x = d.d.x;
	if (is_valid_position(m->pos.d.x, d.d.y, eng->blob)
		&& check_monster_collision(m->pos.d.x, d.d.y, self_idx, eng))
		m->pos.d.y = d.d.y;
}

void	find_closest_alarm(t_monster_rt *m, t_engine *eng,
				t_vec2 *alarm_pos)
{
	int		i;
	double	closest;
	t_vec2	d;

	alarm_pos->d.x = -1.0;
	alarm_pos->d.y = -1.0;
	closest = 1e9;
	i = -1;
	while (++i < eng->static_light_count)
	{
		if (eng->static_lights[i].is_alarm
			&& eng->static_lights[i].is_triggered)
		{
			d.d.x = eng->static_lights[i].x - m->pos.d.x;
			d.d.y = eng->static_lights[i].y - m->pos.d.y;
			if (d.d.x * d.d.x + d.d.y * d.d.y < closest)
			{
				closest = d.d.x * d.d.x + d.d.y * d.d.y;
				alarm_pos->d.x = eng->static_lights[i].x;
				alarm_pos->d.y = eng->static_lights[i].y;
			}
		}
	}
}

void	mstr_go_to_alarm(t_monster_rt *m, t_engine *eng,
				t_vec2 alarm_pos, t_worker *w)
{
	t_vec2	next;

	next = find_next_step(m, eng, alarm_pos, w);
	mstr_move_towards(m, eng, next);
}
