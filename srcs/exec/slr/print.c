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
