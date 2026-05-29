#include "cub.h"
#include <stdlib.h>

static void	add_closure_items(t_state *state, t_list *rules, char *name)
{
	t_item	*new;

	while (rules)
	{
		if (ft_strcmp(((t_rule *)rules->content)->left_symbol, name))
			break ;
		new = create_new_item((t_rule *)rules->content, 0);
		if (!new)
			return ;
		if (!does_state_contains_this_item(state->items, new))
			ft_lstadd_back(&state->items, ft_lstnew(new));
		else
			free(new);
		rules = rules->next;
	}
}

void	closure(t_slr1 *data, t_state *state)
{
	t_list		*curr;
	t_symbol	*sym;

	curr = state->items;
	while (curr)
	{
		sym = get_non_terminal_symbol_after_dot(data,
				(t_item *)curr->content);
		if (sym)
			add_closure_items(state, get_rule_from_symbolname(data, sym->name),
				sym->name);
		curr = curr->next;
	}
}
