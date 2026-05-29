#include "cub.h"

int		get_fsm_state_from_slr_state(t_engine *eng, int slr_state_id);
void	reset_stack(t_monster_rt *m);

static void	do_reduction(t_engine *eng, t_monster_rt *m, t_rule *r)
{
	t_symbol	*left;
	int			curr;
	int			val;
	int			old;

	m->state_stack_top -= r->nb_items;
	if (m->state_stack_top < 1)
		m->state_stack_top = 1;
	curr = m->state_stack[m->state_stack_top - 1];
	left = get_symbol_from_name(eng->slr, r->left_symbol);
	if (left && eng->slr->table[curr][left->nbr] > 0)
	{
		old = m->state;
		val = eng->slr->table[curr][left->nbr];
		m->state_stack[m->state_stack_top++] = val;
		m->state = get_fsm_state_from_slr_state(eng, val);
		if (m->state != old)
			m->alert_timer = 0;
	}
}

static void	handle_slr_reduction(t_engine *eng, t_monster_rt *m, int rule_id)
{
	t_list	*r_node;
	t_rule	*r;

	r_node = eng->slr->rules;
	while (r_node)
	{
		r = (t_rule *)r_node->content;
		if (r->id == rule_id)
		{
			do_reduction(eng, m, r);
			return ;
		}
		r_node = r_node->next;
	}
}

static void	apply_slr_shift(t_engine *eng, t_monster_rt *m, int action)
{
	int	old;

	if (m->state_stack_top < 32)
	{
		old = m->state;
		m->state_stack[m->state_stack_top++] = action;
		m->state = get_fsm_state_from_slr_state(eng, action);
		if (m->state != old)
			m->alert_timer = 0;
	}
}

void	execute_slr_transition_by_id(t_engine *eng, t_monster_rt *m,
				int sym_nbr)
{
	int	action;

	while (1)
	{
		if (m->state_stack_top < 1 || m->state_stack_top > 32)
			reset_stack(m);
		action = eng->slr->table[m->state_stack[m->state_stack_top - 1]][sym_nbr];
		if (action == ACCEPTED)
		{
			reset_stack(m);
			break ;
		}
		if (action == 0)
			break ;
		if (action > 0)
		{
			apply_slr_shift(eng, m, action);
			break ;
		}
		handle_slr_reduction(eng, m, -action);
	}
}

void	execute_slr_transition(t_engine *eng, t_monster_rt *m,
				char *stim)
{
	t_symbol	*sym;

	sym = get_symbol_from_name(eng->slr, stim);
	if (!sym)
		return ;
	execute_slr_transition_by_id(eng, m, sym->nbr);
}
