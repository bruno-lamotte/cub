/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   symbols_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: blamotte <blamotte@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/11 01:39:07 by blamotte          #+#    #+#             */
/*   Updated: 2026/08/11 02:00:00 by blamotte         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub.h"
#include <stdlib.h>

t_list	*get_rule_from_symbolname(t_slr1 *data, char *symbol)
{
	t_list	*rules;

	rules = data->rules;
	while (rules)
	{
		if (!ft_strcmp(((t_rule *)rules->content)->left_symbol, symbol))
			return (rules);
		rules = rules->next;
	}
	return (NULL);
}

t_symbol	*get_symbol_from_name(t_slr1 *data, char *left_symbol)
{
	t_list	*symbols;

	symbols = data->symbols;
	while (symbols)
	{
		if (!ft_strcmp(((t_symbol *)symbols->content)->name, left_symbol))
			return ((t_symbol *)symbols->content);
		symbols = symbols->next;
	}
	return (NULL);
}

int	symbol_is_token(char *name)
{
	return (*name < 97);
}

void	complete_symbol_name(t_symbol **symbol, char *name)
{
	(*symbol)->name = ft_strdup(name);
}

int	symbol_already_parsed(t_list *symbols, char *left_symbol)
{
	while (symbols)
	{
		if (!ft_strcmp(((t_symbol *)symbols->content)->name, left_symbol))
			return (1);
		symbols = symbols->next;
	}
	return (0);
}
