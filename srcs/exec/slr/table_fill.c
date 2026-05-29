#include "cub.h"
#include <stdlib.h>

int	get_transition_from_symbol(t_state *state, char *symbol_name)
{
	t_list          *transitions;
	t_transition    *transition;

	transitions = state->transitions;
	while (transitions)
	{
		transition = (t_transition *)transitions->content;
		if (!ft_strcmp(transition->symbol, symbol_name))
			return (transition->dest_state->id);
		transitions = transitions->next;
	}
	return (0);
}

void	fill_table_when_reduce(t_slr1 *data, int ***table, t_list *state_node,
			t_list *item_node)
{
	t_rule		*rule;
	t_symbol	*symbol;
	t_list		*sym_node;
	int			sid;

	rule = ((t_item *)item_node->content)->rule_of_item;
	sid = ((t_state *)state_node->content)->id;
	sym_node = data->symbols;
	while (sym_node)
	{
		symbol = (t_symbol *)sym_node->content;
		if (rule->id == 1 && !ft_strcmp(symbol->name, "$"))
			(*table)[sid][symbol->nbr] = ACCEPTED;
		else if ((*table)[sid][symbol->nbr] == 0)
			(*table)[sid][symbol->nbr] = -(rule->id);
		sym_node = sym_node->next;
	}
}

void	fill_parsing_table(t_slr1 *data, int ***table)
{
	t_list		*states;
	t_list		*items;
	t_state		*s;
	t_item		*it;
	t_symbol	*sym;

	states = data->states;
	while (states)
	{
		s = (t_state *)states->content;
		items = s->items;
		while (items)
		{
			it = (t_item *)items->content;
			sym = get_symbol_after_dot(data, it);
			if (sym)
				(*table)[s->id][sym->nbr] = get_transition_from_symbol(s, sym->name);
			else
				fill_table_when_reduce(data, table, states, items);
			items = items->next;
		}
		states = states->next;
	}
}
