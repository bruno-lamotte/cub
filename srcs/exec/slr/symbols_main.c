/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   symbols_main.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: blamotte <blamotte@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/11 01:39:07 by blamotte          #+#    #+#             */
/*   Updated: 2026/08/11 01:39:07 by blamotte         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub.h"
#include <stdlib.h>

void	complete_symbol_name(t_symbol **symbol, char *name);
int		symbol_already_parsed(t_list *symbols, char *left_symbol);

int	does_list_contains_this_symbol(t_list *list, char *name)
{
	while (list)
	{
		if (!ft_strcmp((char *)list->content, name))
			return (1);
		list = list->next;
	}
	return (0);
}

void	add_symbol_if_not_exists(t_slr1 *data, char *name, int *nbr)
{
	t_symbol	*new_symbol;

	if (!name || symbol_already_parsed(data->symbols, name))
		return ;
	new_symbol = malloc(sizeof(t_symbol));
	if (!new_symbol)
		return ;
	ft_bzero(new_symbol, sizeof(t_symbol));
	complete_symbol_name(&new_symbol, name);
	if (symbol_is_token(new_symbol->name))
		ft_lstadd_back(&new_symbol->firsts, ft_lstnew(new_symbol->name));
	new_symbol->nbr = (*nbr)++;
	ft_lstadd_back(&data->symbols, ft_lstnew(new_symbol));
}

void	get_symbols(t_slr1 *data)
{
	t_list		*curr;
	t_list		*right;
	t_rule		*rule;
	int			nbr;

	nbr = 0;
	add_symbol_if_not_exists(data, "$", &nbr);
	curr = data->rules;
	while (curr)
	{
		rule = (t_rule *)curr->content;
		add_symbol_if_not_exists(data, rule->left_symbol, &nbr);
		right = rule->right_symbols;
		while (right)
		{
			add_symbol_if_not_exists(data, (char *)right->content, &nbr);
			right = right->next;
		}
		curr = curr->next;
	}
}
