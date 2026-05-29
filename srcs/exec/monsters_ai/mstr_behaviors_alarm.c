#include "cub.h"

void	mstr_alarm_behavior(t_monster_rt *m, t_engine *eng, t_worker *w)
{
	t_vec2	alarm_pos;

	find_closest_alarm(m, eng, &alarm_pos);
	if (alarm_pos.d.x >= 0.0)
		mstr_go_to_alarm(m, eng, alarm_pos, w);
}

static void	bind_behavior_to_state(t_slr1 *slr, int state_id, t_behavior_fn func)
{
	slr->state_behaviors[state_id] = func;
}

static void	bind_state_behavior(t_engine *eng, t_state *s)
{
	int	fsm_state;

	fsm_state = get_fsm_state_from_slr_state(eng, s->id);
	if (fsm_state == MSTR_STATE_PATROL)
		bind_behavior_to_state(eng->slr, s->id, &mstr_patrol_behavior);
	else if (fsm_state == MSTR_STATE_CHASE)
		bind_behavior_to_state(eng->slr, s->id, &mstr_chase_behavior);
	else if (fsm_state == MSTR_STATE_SCAN)
		bind_behavior_to_state(eng->slr, s->id, &mstr_scan_behavior);
	else if (fsm_state == MSTR_STATE_RELOC)
		bind_behavior_to_state(eng->slr, s->id, &mstr_reloc_behavior);
	else if (fsm_state == MSTR_STATE_ALARM)
		bind_behavior_to_state(eng->slr, s->id, &mstr_alarm_behavior);
}

static void	resolve_alarm_sym_nbrs(t_slr1 *slr)
{
	t_symbol	*sym;

	sym = get_symbol_from_name(slr, "STIM_ALARM_HEARD");
	if (sym)
		slr->stim_alarm_heard_nbr = sym->nbr;
	else
		slr->stim_alarm_heard_nbr = -1;
	sym = get_symbol_from_name(slr, "STIM_ALARM_OFF");
	if (sym)
		slr->stim_alarm_off_nbr = sym->nbr;
	else
		slr->stim_alarm_off_nbr = -1;
}

void	init_all_ai_behaviors(t_slr1 *slr, t_engine *eng)
{
	t_list	*state_node;
	int		nb_states;

	resolve_alarm_sym_nbrs(slr);
	nb_states = ft_lstsize(slr->states);
	slr->state_behaviors = malloc(sizeof(t_behavior_fn) * nb_states);
	if (!slr->state_behaviors)
		return ;
	ft_bzero(slr->state_behaviors, sizeof(t_behavior_fn) * nb_states);
	state_node = slr->states;
	while (state_node)
	{
		bind_state_behavior(eng, (t_state *)state_node->content);
		state_node = state_node->next;
	}
}
