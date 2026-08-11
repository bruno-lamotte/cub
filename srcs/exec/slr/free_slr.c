/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free_slr.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: blamotte <blamotte@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/11 01:39:07 by blamotte          #+#    #+#             */
/*   Updated: 2026/08/11 02:00:00 by blamotte         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub.h"
#include <stdlib.h>

void	dummy_free(void *content)
{
	(void)content;
}

static void	free_item(void *content)
{
	if (!content)
		return ;
	free(content);
}

static void	free_transition(void *content)
{
	t_transition	*transition;

	if (!content)
		return ;
	transition = (t_transition *)content;
	free(transition->symbol);
	free(transition);
}

void	free_states(t_list *states)
{
	t_list	*current;
	t_state	*state;

	current = states;
	while (current)
	{
		state = (t_state *)current->content;
		ft_lstclear(&state->items, free_item);
		ft_lstclear(&state->transitions, free_transition);
		free(state);
		current = current->next;
	}
	ft_lstclear(&states, dummy_free);
}
