/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   table_init.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: blamotte <blamotte@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/11 01:39:07 by blamotte          #+#    #+#             */
/*   Updated: 2026/08/11 02:00:00 by blamotte         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub.h"
#include <stdlib.h>

void	fill_parsing_table(t_slr1 *data, int ***table);

t_state	*initialize_first_state(t_slr1 *data)
{
	t_state	*first_state;
	t_item	*first_item;

	first_state = malloc(sizeof(t_state));
	if (!first_state)
		return (NULL);
	first_state->id = 0;
	first_state->items = NULL;
	first_state->transitions = NULL;
	first_item = create_new_item((t_rule *)data->rules->content, 0);
	if (!first_item)
		return (free(first_state), NULL);
	ft_lstadd_back(&first_state->items, ft_lstnew(first_item));
	closure(data, first_state);
	return (first_state);
}

void	get_states(t_slr1 *data)
{
	t_state	*first_state;
	t_list	*current;

	first_state = initialize_first_state(data);
	if (!first_state)
		return ;
	ft_lstadd_back(&data->states, ft_lstnew(first_state));
	current = data->states;
	while (current)
	{
		go_to(data, (t_state *)current->content);
		current = current->next;
	}
}

void	initialize_data(t_slr1 *data)
{
	data->rules = NULL;
	data->symbols = NULL;
	data->states = NULL;
	data->table = NULL;
	data->state_behaviors = NULL;
}

static int	**allocate_parsing_table(int size, int nb_symbols)
{
	int	**table;
	int	*data_block;
	int	i;

	table = malloc(sizeof(int *) * (size + 1));
	if (!table)
		return (NULL);
	data_block = malloc(sizeof(int) * size * nb_symbols);
	if (!data_block)
		return (free(table), NULL);
	ft_bzero(data_block, sizeof(int) * size * nb_symbols);
	i = 0;
	while (i < size)
	{
		table[i] = &data_block[i * nb_symbols];
		i++;
	}
	table[size] = NULL;
	return (table);
}

int	**create_parsing_table(t_slr1 *data)
{
	int	**table;

	if (!parse_grammar(data))
		return (NULL);
	get_symbols(data);
	get_firsts(data);
	get_follows(data);
	get_states(data);
	table = allocate_parsing_table(ft_lstsize(data->states),
			ft_lstsize(data->symbols));
	if (!table)
		return (NULL);
	fill_parsing_table(data, &table);
	return (table);
}
