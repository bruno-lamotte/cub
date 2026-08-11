/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   firsts.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: blamotte <blamotte@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/11 01:39:07 by blamotte          #+#    #+#             */
/*   Updated: 2026/08/11 02:00:00 by blamotte         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub.h"
#include <stdlib.h>

int		does_list_contains_this_symbol(t_list *list, char *name);
void	get_first_dfs(t_slr1 *data, t_rule *rule, t_symbol *left_symbol,
			t_symbol *right_symbol);

int	add_firsts_if_not_token(t_slr1 *data, t_rule *next_rule,
		t_symbol *left_symbol)
{
	t_list	*new_list;
	t_list	*new_node;
	char	*dup;

	new_list = get_symbol_from_name(data, next_rule->left_symbol)->firsts;
	if (!new_list)
		return (0);
	while (new_list)
	{
		if (!does_list_contains_this_symbol(left_symbol->firsts,
				(char *)new_list->content))
		{
			dup = ft_strdup((char *)new_list->content);
			if (!dup)
				return (0);
			new_node = ft_lstnew(dup);
			if (!new_node)
				return (free(dup), 0);
			ft_lstadd_back(&left_symbol->firsts, new_node);
		}
		new_list = new_list->next;
	}
	return (1);
}

int	add_first_if_token(t_symbol *left_symbol, t_symbol *right_symbol)
{
	t_list	*new_list;
	char	*dup;

	dup = ft_strdup(right_symbol->name);
	if (!dup)
		return (0);
	new_list = ft_lstnew(dup);
	if (!new_list)
	{
		free(dup);
		return (0);
	}
	ft_lstadd_back(&left_symbol->firsts, new_list);
	return (1);
}

int	should_look_for_next_right_symbol(t_symbol *left_symbol, t_rule *rule)
{
	if (!rule->right_symbols->next)
		return (0);
	return (does_list_contains_this_symbol(left_symbol->firsts, "EMPTY")
		&& rule->right_symbols->next->content);
}

static int	dfs_non_token(t_slr1 *data, t_rule *rule, t_symbol *left,
		t_symbol *right)
{
	t_list		*node;
	t_rule		*next;
	t_symbol	*next_sym;

	node = get_rule_from_symbolname(data, right->name);
	next = (t_rule *)node->content;
	if (rule != next)
	{
		next_sym = get_symbol_from_name(data,
				(char *)next->right_symbols->content);
		get_first_dfs(data, next, left, next_sym);
	}
	return (add_firsts_if_not_token(data, next, left));
}

void	get_first_dfs(t_slr1 *data, t_rule *rule, t_symbol *left_symbol,
			t_symbol *right_symbol)
{
	while (1)
	{
		if (!does_list_contains_this_symbol(left_symbol->firsts,
				right_symbol->name))
		{
			if (symbol_is_token(right_symbol->name))
			{
				if (!add_first_if_token(left_symbol, right_symbol))
					return ;
			}
			else if (!dfs_non_token(data, rule, left_symbol, right_symbol))
				return ;
		}
		if (should_look_for_next_right_symbol(left_symbol, rule))
			right_symbol = get_symbol_from_name(data,
					(char *)rule->right_symbols->next->content);
		else
			return ;
	}
}
