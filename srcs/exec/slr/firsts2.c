/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   firsts2.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: blamotte <blamotte@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/11 02:00:00 by blamotte          #+#    #+#             */
/*   Updated: 2026/08/11 02:00:00 by blamotte         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub.h"

void	get_first_dfs(t_slr1 *data, t_rule *rule, t_symbol *left_symbol,
			t_symbol *right_symbol);

void	get_firsts(t_slr1 *data)
{
	t_symbol	*left;
	t_symbol	*right;
	t_list		*curr;
	t_rule		*rule;
	int			loops;

	loops = 0;
	while (loops++ < 2)
	{
		curr = data->rules;
		while (curr)
		{
			rule = (t_rule *)curr->content;
			left = get_symbol_from_name(data, rule->left_symbol);
			right = get_symbol_from_name(data,
					(char *)rule->right_symbols->content);
			get_first_dfs(data, rule, left, right);
			curr = curr->next;
		}
	}
}
