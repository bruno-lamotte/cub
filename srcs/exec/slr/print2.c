/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   print2.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: blamotte <blamotte@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/11 02:00:00 by blamotte          #+#    #+#             */
/*   Updated: 2026/08/11 02:00:00 by blamotte         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub.h"

void	print_rules_in_c(t_list *rules_list)
{
	t_rule	*rule;
	t_list	*current;

	current = rules_list;
	printf("t_reduce_rule g_reduce_rules[%d] = {\n", ft_lstsize(rules_list));
	while (current)
	{
		rule = (t_rule *)current->content;
		printf("\t{\"%s\", %d}", rule->left_symbol, rule->nb_items);
		if (current->next)
			printf(",\n");
		else
			printf("\n");
		current = current->next;
	}
	printf("};\n");
}

void	print_symbols_in_c(t_list *symbols_list)
{
	t_symbol	*symbol;
	t_list		*current;

	current = symbols_list;
	while (current)
	{
		symbol = (t_symbol *)current->content;
		if (symbol_is_token(symbol->name))
			printf("#define %s %d\n", symbol->name, symbol->nbr);
		current = current->next;
	}
}
