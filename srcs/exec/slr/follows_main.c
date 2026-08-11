/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   follows_main.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: blamotte <blamotte@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/11 01:39:07 by blamotte          #+#    #+#             */
/*   Updated: 2026/08/11 02:00:00 by blamotte         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub.h"

void	add_dollar_to_start_symbol(t_slr1 *data);
int		add_follows_from_follows(t_symbol **symbol, t_symbol *next_symbol);
int		add_follows_from_firsts(t_symbol **symbol, t_symbol *next_symbol);
int		contains_empty_in_firsts(t_symbol *symbol);

static void	update_follow_rule(t_slr1 *data, t_rule *rule, int *added)
{
	t_list		*rights;
	t_symbol	*tmp;
	t_symbol	*next_tmp;
	t_symbol	*left_sym;

	rights = rule->right_symbols;
	tmp = get_symbol_from_name(data, (char *)rights->content);
	left_sym = get_symbol_from_name(data, rule->left_symbol);
	while (rights->next)
	{
		next_tmp = get_symbol_from_name(data, (char *)rights->next->content);
		*added += add_follows_from_firsts(&tmp, next_tmp);
		if (contains_empty_in_firsts(next_tmp))
			*added += add_follows_from_follows(&tmp, left_sym);
		tmp = next_tmp;
		rights = rights->next;
	}
	if (!rights->next && ft_strcmp(tmp->name, rule->left_symbol))
		*added += add_follows_from_follows(&tmp, left_sym);
}

void	update_follows(t_slr1 *data, t_list *current_rule, int *added)
{
	while (current_rule)
	{
		update_follow_rule(data, (t_rule *)current_rule->content, added);
		current_rule = current_rule->next;
	}
}

void	get_follows(t_slr1 *data)
{
	t_list	*current_rule;
	int		added;

	added = 1;
	current_rule = data->rules;
	add_dollar_to_start_symbol(data);
	while (added)
	{
		added = 0;
		update_follows(data, current_rule, &added);
	}
}
