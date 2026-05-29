#include "cub.h"

static void	handle_auto_actions(t_engine *eng, t_monster_rt *m)
{
	(void)eng;
	(void)m;
}

void	update_single_monster(t_monster_rt *m, t_engine *eng, t_worker *w)
{
	double			d2;
	int				is_in_alarm;
	t_behavior_fn	behavior;

	d2 = (eng->player->pos.d.x - m->pos.d.x) * (eng->player->pos.d.x - m->pos.d.x)
		+ (eng->player->pos.d.y - m->pos.d.y) * (eng->player->pos.d.y - m->pos.d.y);
	is_in_alarm = (m->state == MSTR_STATE_ALARM);
	if (eng->alarm_triggered && !is_in_alarm)
		execute_slr_transition_by_id(eng, m, eng->slr->stim_alarm_heard_nbr);
	else if (!eng->alarm_triggered && is_in_alarm)
		execute_slr_transition_by_id(eng, m, eng->slr->stim_alarm_off_nbr);
	handle_auto_actions(eng, m);
	run_grammar_transitions(m, eng, d2, detect_player(m, eng));
	behavior = eng->slr->state_behaviors[m->state_stack[m->state_stack_top - 1]];
	if (behavior)
		behavior(m, eng, w);
}

void	print_monsters_debug(t_monster_rt *mstr, int count)
{
	int	k;

	k = -1;
	while (++k < count)
	{
		printf("Monster %d: pos=(%.2f, %.2f), guard=(%.2f, %.2f), ",
			k, mstr[k].pos.d.x, mstr[k].pos.d.y, mstr[k].guard.d.x, mstr[k].guard.d.y);
		printf("state=%d, stack_top=%d, timer=%d\n",
			mstr[k].state, mstr[k].state_stack_top, mstr[k].alert_timer);
	}
}

static void	disarm_single_alarm(t_engine *eng, t_light *l)
{
	if (is_any_monster_near(eng, l))
	{
		l->check_timer++;
		if (l->check_timer >= ALARM_DISARM_TIME)
		{
			l->is_triggered = 0;
			l->check_timer = 0;
			update_global_alarm_state(eng);
		}
	}
	else
		l->check_timer = 0;
}

void	update_alarm_disarm(t_engine *eng)
{
	int		i;
	t_light	*l;

	i = -1;
	while (++i < eng->static_light_count)
	{
		l = &eng->static_lights[i];
		if (l->is_alarm && l->is_triggered)
			disarm_single_alarm(eng, l);
		else
			l->check_timer = 0;
	}
}
