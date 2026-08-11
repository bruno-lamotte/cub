/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   print.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: blamotte <blamotte@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/11 01:39:07 by blamotte          #+#    #+#             */
/*   Updated: 2026/08/11 02:00:00 by blamotte         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub.h"
#include <stdlib.h>

static void	print_table_header(t_list *symbols)
{
	t_symbol	*symbol;

	printf("=== SLR(1) PARSING TABLE ===\n");
	printf("State\t");
	while (symbols)
	{
		symbol = (t_symbol *)symbols->content;
		printf("%s\t", symbol->name);
		symbols = symbols->next;
	}
	printf("\n");
}

static void	print_table_row(int *row, int nb_symbols)
{
	int	j;

	j = 0;
	while (j < nb_symbols)
	{
		if (row[j] == ACCEPTED)
			printf("acc\t");
		else if (row[j] > 0)
			printf("s%d\t", row[j]);
		else if (row[j] < 0)
			printf("r%d\t", -row[j]);
		else
			printf(".\t");
		j++;
	}
	printf("\n");
}

void	print_table(int **table, t_slr1 *data)
{
	int	i;
	int	nb_states;
	int	nb_symbols;

	nb_states = ft_lstsize(data->states);
	nb_symbols = ft_lstsize(data->symbols);
	print_table_header(data->symbols);
	i = 0;
	while (i < nb_states)
	{
		printf("%d\t", i);
		print_table_row(table[i], nb_symbols);
		i++;
	}
}

void	print_table_in_c(int **table, int nb_states, int nb_tokens)
{
	int	i;
	int	j;

	printf("int g_parsing_table[%d][%d] = {\n", nb_states, nb_tokens);
	i = 0;
	while (i < nb_states)
	{
		printf("\t{");
		j = 0;
		while (j < nb_tokens)
		{
			printf("%d", table[i][j]);
			if (j < nb_tokens - 1)
				printf(", ");
			j++;
		}
		printf("}");
		if (i < nb_states - 1)
			printf(",\n");
		else
			printf("\n");
		i++;
	}
	printf("};\n");
}
