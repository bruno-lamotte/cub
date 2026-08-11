/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   grammar.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: blamotte <blamotte@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/11 01:39:07 by blamotte          #+#    #+#             */
/*   Updated: 2026/08/11 02:00:00 by blamotte         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub.h"
#include <stdlib.h>

void	get_leftsymbol_from_grammar(t_slr1 *data, t_rule **new_rule,
			char *line)
{
	int		i;
	t_rule	*last;

	i = 0;
	if (line[i] == ' ' || line[i] == '|')
	{
		last = (t_rule *)ft_lstlast(data->rules)->content;
		(*new_rule)->left_symbol = ft_strdup(last->left_symbol);
	}
	else
	{
		while (line[i] != ' ' && line[i] != ':')
			i++;
		(*new_rule)->left_symbol = ft_substr(line, 0, i);
	}
}

void	get_id_from_grammar(t_slr1 *data, t_rule **new_rule)
{
	t_rule	*last;

	if (data->rules == NULL)
		(*new_rule)->id = 1;
	else
	{
		last = (t_rule *)ft_lstlast(data->rules)->content;
		(*new_rule)->id = last->id + 1;
	}
}

void	get_rightsymbols_from_grammar(t_rule **new_rule, char *line)
{
	int		i;
	char	*symbol;
	t_list	*new_list;

	while (*line && *line != '\n' && *line != '\0' && *line != '\r')
	{
		i = 0;
		while (*line == ' ')
			line++;
		while (line[i] && line[i] != ' ' && line[i] != '\n'
			&& line[i] != '\r')
			i++;
		symbol = ft_substr(line, 0, i);
		new_list = ft_lstnew(symbol);
		ft_lstadd_back(&(*new_rule)->right_symbols, new_list);
		line += i;
	}
}

void	get_nbitems_from_grammar(t_rule **new_rule)
{
	int		i;
	t_list	*tmp;

	tmp = (*new_rule)->right_symbols;
	i = 1;
	while (tmp->next)
	{
		tmp = tmp->next;
		i++;
	}
	(*new_rule)->nb_items = i;
}

void	get_rule_from_grammar(t_slr1 *data, t_rule **new_rule, char *line)
{
	get_id_from_grammar(data, new_rule);
	get_leftsymbol_from_grammar(data, new_rule, line);
	while (*line && *line != '|' && *line != ':' && *line != '\n'
		&& *line != '\r')
		line++;
	get_rightsymbols_from_grammar(new_rule, ++line);
	get_nbitems_from_grammar(new_rule);
}
