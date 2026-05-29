#include "cub.h"
#include <stdio.h>
#include <stdlib.h>

t_slr1	*modular_data(void)
{
	t_slr1 *data = malloc(sizeof(t_slr1));
	initialize_data(data);
	return data;
}

int main(void)
{
	t_slr1 *data;
	int **table;

	data = modular_data();
	table = create_parsing_table(data);
	if (!table)
	{
		printf("Failed to generate table\n");
		return 1;
	}
	print_table_in_c(table, ft_lstsize(data->states), ft_lstsize(data->symbols));
	print_rules_in_c(data->rules);
	print_symbols_in_c(data->symbols);
	free_all(data, table);
	return 0;
}
