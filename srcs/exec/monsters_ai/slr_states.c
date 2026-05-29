#include "cub.h"

int	state_name_to_id(char *name)
{
	if (ft_strcmp(name, "patrol_state") == 0
		|| ft_strcmp(name, "patrol_loop") == 0
		|| ft_strcmp(name, "patrol_action") == 0)
		return (MSTR_STATE_PATROL);
	if (ft_strcmp(name, "chase_state") == 0
		|| ft_strcmp(name, "chase_session") == 0
		|| ft_strcmp(name, "chase_loop") == 0
		|| ft_strcmp(name, "chase_action") == 0
		|| ft_strcmp(name, "ACT_SET_CHASE") == 0)
		return (MSTR_STATE_CHASE);
	if (ft_strcmp(name, "attack_state") == 0
		|| ft_strcmp(name, "ACT_SET_ATTACK") == 0)
		return (MSTR_STATE_ATTACK);
	if (ft_strcmp(name, "scan_state") == 0
		|| ft_strcmp(name, "ACT_SET_SCAN") == 0)
		return (MSTR_STATE_SCAN);
	if (ft_strcmp(name, "reloc_state") == 0
		|| ft_strcmp(name, "ACT_SET_RELOC") == 0)
		return (MSTR_STATE_RELOC);
	if (ft_strcmp(name, "alarm_subroutine") == 0
		|| ft_strcmp(name, "alarm_session") == 0
		|| ft_strcmp(name, "alarm_loop") == 0
		|| ft_strcmp(name, "alarm_action") == 0)
		return (MSTR_STATE_ALARM);
	return (-1);
}

static int	get_sym_state(t_engine *eng, t_list *item_node)
{
	t_symbol	*next_sym;

	while (item_node)
	{
		next_sym = get_symbol_after_dot(eng->slr,
				(t_item *)item_node->content);
		if (next_sym && state_name_to_id(next_sym->name) != -1)
			return (state_name_to_id(next_sym->name));
		item_node = item_node->next;
	}
	return (-1);
}

static int	get_lhs_state(t_list *item_node)
{
	int	id;

	while (item_node)
	{
		id = state_name_to_id(((t_item *)item_node->content)
				->rule_of_item->left_symbol);
		if (id != -1)
			return (id);
		item_node = item_node->next;
	}
	return (-1);
}

int	get_fsm_state_from_slr_state(t_engine *eng, int slr_state_id)
{
	t_list	*state_node;
	t_state	*s;
	int		id;

	state_node = eng->slr->states;
	while (state_node)
	{
		s = (t_state *)state_node->content;
		if (s->id == slr_state_id)
		{
			id = get_sym_state(eng, s->items);
			if (id != -1)
				return (id);
			id = get_lhs_state(s->items);
			if (id != -1)
				return (id);
		}
		state_node = state_node->next;
	}
	return (MSTR_STATE_PATROL);
}

void	reset_stack(t_monster_rt *m)
{
	m->state_stack[0] = 0;
	m->state_stack_top = 1;
	m->state = MSTR_STATE_PATROL;
	m->alert_timer = 0;
}
