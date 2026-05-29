#include "cub.h"
#include <math.h>

void	mstr_patrol_behavior(t_monster_rt *m, t_engine *eng, t_worker *w)
{
	m->alert_timer++;
	mstr_go_to_guard(m, eng, w);
}

void	mstr_chase_behavior(t_monster_rt *m, t_engine *eng, t_worker *w)
{
	(void)w;
	if (!detect_player(m, eng))
		m->alert_timer++;
	mstr_chase(m, eng);
}

void	mstr_scan_behavior(t_monster_rt *m, t_engine *eng, t_worker *w)
{
	double	old;

	(void)eng;
	(void)w;
	m->alert_timer++;
	old = m->dir.d.x;
	m->dir.d.x = m->dir.d.x * cos(MSTR_ROTATION_STEP) - m->dir.d.y * sin(MSTR_ROTATION_STEP);
	m->dir.d.y = old * sin(MSTR_ROTATION_STEP) + m->dir.d.y * cos(MSTR_ROTATION_STEP);
}

void	mstr_reloc_behavior(t_monster_rt *m, t_engine *eng, t_worker *w)
{
	assign_new_patrol_target(m, eng, w);
	m->alert_timer = 0;
	execute_slr_transition(eng, m, "ACT_RECALCULATE_PATROL");
}
