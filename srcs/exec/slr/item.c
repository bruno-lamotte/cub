/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   item.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: blamotte <blamotte@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/11 01:39:07 by blamotte          #+#    #+#             */
/*   Updated: 2026/08/11 02:00:00 by blamotte         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub.h"
#include <stdlib.h>

t_item	*create_new_item(t_rule *rule, int dot_pos)
{
	t_item		*new_item;
	static int	id = 0;

	new_item = malloc(sizeof(t_item));
	if (!new_item)
		return (NULL);
	new_item->id = ++id;
	new_item->rule_of_item = rule;
	new_item->dot_pos = dot_pos;
	return (new_item);
}

t_symbol	*get_symbol_from_list(t_slr1 *data, t_list *right_symbols,
				int dot_pos)
{
	t_list	*current;

	current = right_symbols;
	while (dot_pos--)
	{
		if (!current)
			return (NULL);
		current = current->next;
	}
	return (get_symbol_from_name(data, (char *)current->content));
}

t_symbol	*get_symbol_after_dot(t_slr1 *data, t_item *item)
{
	t_symbol	*symbol;

	if (item->dot_pos >= item->rule_of_item->nb_items)
		return (NULL);
	symbol = get_symbol_from_list(data,
			item->rule_of_item->right_symbols, item->dot_pos);
	return (symbol);
}

t_symbol	*get_non_terminal_symbol_after_dot(t_slr1 *data, t_item *item)
{
	t_symbol	*symbol;

	symbol = get_symbol_after_dot(data, item);
	if (symbol && !symbol_is_token(symbol->name))
		return (symbol);
	symbol = NULL;
	return (symbol);
}

int	does_state_contains_this_item(t_list *items, t_item *item)
{
	t_list	*current;
	t_item	*current_item;

	current = items;
	while (current)
	{
		current_item = (t_item *)current->content;
		if (current_item->rule_of_item == item->rule_of_item
			&& current_item->dot_pos == item->dot_pos)
			return (1);
		current = current->next;
	}
	return (0);
}
