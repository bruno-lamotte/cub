/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   follows.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: blamotte <blamotte@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/11 01:39:07 by blamotte          #+#    #+#             */
/*   Updated: 2026/08/11 02:00:00 by blamotte         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub.h"
#include <stdlib.h>

int	does_list_contains_this_symbol(t_list *list, char *name);

void	add_dollar_to_start_symbol(t_slr1 *data)
{
	t_list		*new_list;
	t_symbol	*symbol;
	char		*dup;
	t_rule		*rule;

	rule = (t_rule *)data->rules->content;
	symbol = get_symbol_from_name(data, rule->left_symbol);
	dup = ft_strdup("$");
	if (!dup)
		return ;
	new_list = ft_lstnew(dup);
	if (!new_list)
	{
		free(dup);
		return ;
	}
	ft_lstadd_back(&symbol->follows, new_list);
}

static int	add_follow_node(t_symbol **symbol, char *content)
{
	t_list	*node;
	char	*dup;

	if (does_list_contains_this_symbol((*symbol)->follows, content))
		return (0);
	dup = ft_strdup(content);
	if (!dup)
		return (0);
	node = ft_lstnew(dup);
	if (!node)
	{
		free(dup);
		return (0);
	}
	ft_lstadd_back(&(*symbol)->follows, node);
	return (1);
}

int	add_follows_from_follows(t_symbol **symbol, t_symbol *next_symbol)
{
	t_list	*list;
	int		added;

	added = 0;
	list = next_symbol->follows;
	while (list)
	{
		added += add_follow_node(symbol, (char *)list->content);
		list = list->next;
	}
	return (added);
}

int	add_follows_from_firsts(t_symbol **symbol, t_symbol *next_symbol)
{
	t_list	*list;
	int		added;

	added = 0;
	list = next_symbol->firsts;
	while (list)
	{
		if (ft_strcmp((char *)list->content, "EMPTY"))
			added += add_follow_node(symbol, (char *)list->content);
		list = list->next;
	}
	return (added);
}

int	contains_empty_in_firsts(t_symbol *symbol)
{
	t_list	*current;

	current = symbol->firsts;
	while (current)
	{
		if (!ft_strcmp((char *)current->content, "EMPTY"))
			return (1);
		current = current->next;
	}
	return (0);
}
