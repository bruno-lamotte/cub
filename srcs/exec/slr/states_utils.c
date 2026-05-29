#include "cub.h"
#include <stdlib.h>

void	free_tmp_state(t_state *state)
{
	ft_lstclear(&state->items, free);
	free(state);
}

t_transition	*create_transition(char *symbol_name, t_state *dest_state)
{
	t_transition	*new_transition;

	new_transition = malloc(sizeof(t_transition));
	if (!new_transition)
		return (NULL);
	new_transition->symbol = ft_strdup(symbol_name);
	if (!new_transition->symbol)
		return (free(new_transition), NULL);
	new_transition->dest_state = dest_state;
	return (new_transition);
}

t_state	*create_new_state(t_slr1 *data)
{
	t_state		*new_state;
	t_list		*states;
	int			max_id;

	new_state = malloc(sizeof(t_state));
	if (!new_state)
		return (NULL);
	states = data->states;
	max_id = 0;
	while (states)
	{
		if (((t_state *)states->content)->id > max_id)
			max_id = ((t_state *)states->content)->id;
		states = states->next;
	}
	new_state->id = max_id + 1;
	new_state->items = NULL;
	new_state->transitions = NULL;
	return (new_state);
}

void	add_item_to_list(t_state **new_state, t_item *item)
{
	t_item	*new_item;

	new_item = create_new_item(item->rule_of_item, item->dot_pos + 1);
	if (!new_item)
		return ;
	ft_lstadd_back(&(*new_state)->items, ft_lstnew(new_item));
}

int	are_items_equal(t_item *i1, t_item *i2)
{
	if (i1->rule_of_item == i2->rule_of_item && i1->dot_pos == i2->dot_pos)
		return (1);
	return (0);
}
