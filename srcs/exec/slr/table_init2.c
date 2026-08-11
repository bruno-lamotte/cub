/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   table_init2.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: blamotte <blamotte@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/11 02:00:00 by blamotte          #+#    #+#             */
/*   Updated: 2026/08/11 02:00:00 by blamotte         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub.h"
#include <stdlib.h>

void	initialize_data(t_slr1 *data);
int		**create_parsing_table(t_slr1 *data);
void	print_table(int **table, t_slr1 *data);

int	slr_main(void)
{
	t_slr1	*data;
	int		**table;

	data = malloc(sizeof(t_slr1));
	if (!data)
		return (0);
	initialize_data(data);
	table = create_parsing_table(data);
	if (table)
	{
		print_table(table, data);
		free_all(data, table);
	}
	else
		free(data);
	return (0);
}
