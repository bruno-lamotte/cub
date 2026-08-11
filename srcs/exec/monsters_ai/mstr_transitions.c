/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mstr_transitions.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: blamotte <blamotte@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/11 00:33:49 by blamotte          #+#    #+#             */
/*   Updated: 2026/08/11 00:33:49 by blamotte         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub.h"

int	is_dest_reached(t_monster_rt *m)
{
	t_vec2	d;

	d.d.x = m->guard.d.x - m->pos.d.x;
	d.d.y = m->guard.d.y - m->pos.d.y;
	return (d.d.x * d.d.x + d.d.y * d.d.y < MSTR_DEST_REACHED_DIST_SQ);
}

void	patrol_transitions(t_monster_rt *m, t_engine *eng, int spotted)
{
	if (spotted)
		execute_slr_transition(eng, m, "STIM_LOS_TRUE");
	else if (check_monster_self_collision(m, eng))
		execute_slr_transition(eng, m, "STIM_COLLISION");
	else if (is_dest_reached(m))
	{
		if (is_bottleneck_cell(eng, (int)m->guard.d.x, (int)m->guard.d.y))
		{
			if (m->alert_timer > MSTR_STATION_TIMEOUT)
				execute_slr_transition(eng, m, "STIM_STATION_TIMEOUT");
		}
		else if (m->alert_timer > MSTR_ALERT_TIMEOUT)
			execute_slr_transition(eng, m, "STIM_ALERT_TIMEOUT");
	}
}

void	chase_transitions(t_monster_rt *m, t_engine *eng, double d2,
			int spotted)
{
	if (d2 < MSTR_MELEE_DIST_SQ)
		execute_slr_transition(eng, m, "ACT_MELEE_STRIKE");
	else if (!spotted && m->alert_timer > MSTR_CHASE_LOST_LOS_TIMEOUT)
		execute_slr_transition(eng, m, "STIM_LOS_FALSE");
}

void	scan_transitions(t_monster_rt *m, t_engine *eng, int spotted)
{
	if (spotted)
		execute_slr_transition(eng, m, "STIM_LOS_TRUE");
	else if (m->alert_timer > MSTR_SCAN_TIMEOUT)
		execute_slr_transition(eng, m, "STIM_ALERT_TIMEOUT");
}

void	run_grammar_transitions(t_monster_rt *m, t_engine *eng,
			double d2, int spotted)
{
	if (m->state == MSTR_STATE_PATROL)
		patrol_transitions(m, eng, spotted);
	else if (m->state == MSTR_STATE_CHASE)
		chase_transitions(m, eng, d2, spotted);
	else if (m->state == MSTR_STATE_ATTACK)
	{
		if (d2 > MSTR_ATTACK_CHASE_DIST_SQ
			|| m->alert_timer > MSTR_ATTACK_DURATION)
		{
			if (d2 < MSTR_MELEE_DIST_SQ)
				m->alert_timer = 0;
			else
				execute_slr_transition(eng, m, "ACT_CHASE_MOVE");
		}
	}
	else if (m->state == MSTR_STATE_SCAN)
		scan_transitions(m, eng, spotted);
	else if (m->state == MSTR_STATE_ALARM && spotted)
		execute_slr_transition(eng, m, "STIM_LOS_TRUE");
}
