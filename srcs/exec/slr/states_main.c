/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   states_main.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: blamotte <blamotte@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/11 01:39:07 by blamotte          #+#    #+#             */
/*   Updated: 2026/08/11 02:00:00 by blamotte         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub.h"
#include <stdlib.h>

void			free_tmp_state(t_state *state);
t_transition	*create_transition(char *symbol_name, t_state *dest_state);
t_state			*create_new_state(t_slr1 *data);
void			add_item_to_list(t_state **new_state, t_item *item);
int				are_items_equal(t_item *i1, t_item *i2);

t_state	*find_state(t_slr1 *data, t_state *new_state)
{
	t_list	*states;
	t_list	*items;
	t_list	*items_new_state;

	states = data->states;
	while (states)
	{
		items = ((t_state *)states->content)->items;
		items_new_state = new_state->items;
		if (ft_lstsize(items) == ft_lstsize(items_new_state))
		{
			while (items && items_new_state)
			{
				if (!are_items_equal((t_item *)items->content,
						(t_item *)items_new_state->content))
					break ;
				items = items->next;
				items_new_state = items_new_state->next;
			}
			if (!items && !items_new_state)
				return ((t_state *)states->content);
		}
		states = states->next;
	}
	return (NULL);
}

void	add_transition_to_state(t_slr1 *data, t_state *current_state,
			t_state *new_state, t_symbol *symbol)
{
	t_state			*transition_state;
	t_transition	*transition;

	transition_state = find_state(data, new_state);
	if (!transition_state)
	{
		ft_lstadd_back(&data->states, ft_lstnew(new_state));
		transition = create_transition(symbol->name, new_state);
	}
	else
	{
		transition = create_transition(symbol->name, transition_state);
		free_tmp_state(new_state);
	}
	ft_lstadd_back(&current_state->transitions, ft_lstnew(transition));
}

void	get_new_state(t_slr1 *data, t_state *current_state,
			t_state *new_state, t_symbol *target_symbol)
{
	t_list		*items;
	t_item		*item;
	t_symbol	*symbol;

	items = current_state->items;
	while (items)
	{
		item = (t_item *)items->content;
		symbol = get_symbol_after_dot(data, item);
		if (symbol && !ft_strcmp(symbol->name, target_symbol->name))
		{
			add_item_to_list(&new_state, item);
		}
		items = items->next;
	}
	closure(data, new_state);
	add_transition_to_state(data, current_state, new_state, target_symbol);
}

void	go_to(t_slr1 *data, t_state *state)
{
	t_list		*items;
	t_item		*item;
	t_symbol	*symbol;
	t_list		*symbols_already_parsed;
	t_state		*new_state;

	items = state->items;
	symbols_already_parsed = NULL;
	while (items)
	{
		item = (t_item *)items->content;
		symbol = get_symbol_after_dot(data, item);
		if (symbol && !does_list_contains_this_symbol(symbols_already_parsed,
				symbol->name))
		{
			ft_lstadd_back(&symbols_already_parsed, ft_lstnew(symbol->name));
			new_state = create_new_state(data);
			if (new_state)
				get_new_state(data, state, new_state, symbol);
		}
		items = items->next;
	}
	ft_lstclear(&symbols_already_parsed, dummy_free);
}
