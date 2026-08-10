#include "cub.h"
#include <math.h>

static void	rotate_monster_dir(t_monster_rt *m)
{
	double	old;

	old = m->dir.d.x;
	m->dir.d.x = m->dir.d.x * cos(MSTR_ROTATION_STEP)
		- m->dir.d.y * sin(MSTR_ROTATION_STEP);
	m->dir.d.y = old * sin(MSTR_ROTATION_STEP)
		+ m->dir.d.y * cos(MSTR_ROTATION_STEP);
}

void	mstr_go_to_guard(t_monster_rt *m, t_engine *eng, t_worker *w)
{
	t_vec2	d;

	d.d.x = m->guard.d.x - m->pos.d.x;
	d.d.y = m->guard.d.y - m->pos.d.y;
	if (d.d.x * d.d.x + d.d.y * d.d.y < MSTR_DEST_REACHED_DIST_SQ)
	{
		m->pos = m->guard;
		rotate_monster_dir(m);
		return ;
	}
	if (m->alert_timer > MSTR_ALERT_PATROL_TIMEOUT)
	{
		assign_new_patrol_target(m, eng, w);
		m->alert_timer = 0;
		return ;
	}
	mstr_move_towards(m, eng, find_next_step(m, eng, m->guard, w));
}

static t_vec2	get_chase_target(t_monster_rt *m, t_engine *eng, t_worker *w)
{
	int		dx;
	int		dy;
	double	d2;

	d2 = (m->pos.d.x - eng->player->pos.d.x) * (m->pos.d.x - eng->player->pos.d.x)
		+ (m->pos.d.y - eng->player->pos.d.y) * (m->pos.d.y - eng->player->pos.d.y);
	if (d2 < 2.0 && check_los(m->pos, eng->player->pos, eng->blob))
		return (eng->player->pos);
	dx = (int)m->pos.d.x - (int)eng->player->pos.d.x;
	dy = (int)m->pos.d.y - (int)eng->player->pos.d.y;
	if (dx < 0)
		dx = -dx;
	if (dy < 0)
		dy = -dy;
	if (dx + dy <= 1 || check_thick_los(m->pos, eng->player->pos, MSTR_LOS_THICKNESS,
			eng->blob))
		return (eng->player->pos);
	return (find_next_step(m, eng, eng->player->pos, w));
}

static void	update_mstr_chase_dir(t_monster_rt *m, t_vec2 d)
{
	float	len2;

	len2 = (float)(d.d.x * d.d.x + d.d.y * d.d.y);
	if (len2 > 0.01f)
	{
		m->dir.d.x = d.d.x * fast_inv_sqrt(len2);
		m->dir.d.y = d.d.y * fast_inv_sqrt(len2);
	}
}

void	mstr_chase(t_monster_rt *m, t_engine *eng, t_worker *w)
{
	t_vec2	d;
	t_vec2	tgt;
	int		self;

	self = m - get_monster_rt(eng->blob);
	tgt = get_chase_target(m, eng, w);
	d.d.x = tgt.d.x - m->pos.d.x;
	d.d.y = tgt.d.y - m->pos.d.y;
	update_mstr_chase_dir(m, d);
	d.d.x = m->pos.d.x + m->dir.d.x * MSTR_CHASE_SPEED;
	d.d.y = m->pos.d.y + m->dir.d.y * MSTR_CHASE_SPEED;
	if (is_valid_position(d.d.x, m->pos.d.y, eng->blob)
		&& check_monster_collision(d.d.x, m->pos.d.y, self, eng))
		m->pos.d.x = d.d.x;
	if (is_valid_position(m->pos.d.x, d.d.y, eng->blob)
		&& check_monster_collision(m->pos.d.x, d.d.y, self, eng))
		m->pos.d.y = d.d.y;
}
