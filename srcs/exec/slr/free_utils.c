#include "cub.h"
#include <stdlib.h>

void	dummy_free(void *content);

static void	remove_self_first(t_symbol *symbol)
{
	t_list	*tmp;
	t_list	*prev;

	tmp = symbol->firsts;
	prev = NULL;
	while (tmp)
	{
		if (tmp->content == symbol->name)
		{
			if (prev)
				prev->next = tmp->next;
			else
				symbol->firsts = tmp->next;
			free(tmp);
			break ;
		}
		prev = tmp;
		tmp = tmp->next;
	}
}

void	free_symbols(t_list *symbols)
{
	t_list		*current;
	t_symbol	*symbol;

	current = symbols;
	while (current)
	{
		symbol = (t_symbol *)current->content;
		remove_self_first(symbol);
		free(symbol->name);
		ft_lstclear(&symbol->firsts, free);
		ft_lstclear(&symbol->follows, free);
		free(symbol);
		current = current->next;
	}
	ft_lstclear(&symbols, dummy_free);
}

void    free_rules(t_list *rules)
{
	t_list  *current;
	t_rule  *rule;

	current = rules;
	while (current)
	{
		rule = (t_rule *)current->content;
		free(rule->left_symbol);
		ft_lstclear(&rule->right_symbols, free);
		free(rule);
		current = current->next;
	}
	ft_lstclear(&rules, dummy_free);
}

void    free_table(int **table)
{
	if (table)
	{
		if (table[0])
			free(table[0]);
		free(table);
	}
}

void    free_all(t_slr1 *data, int **table)
{
	free_table(table);
	free_rules(data->rules);
	free_symbols(data->symbols);
	free_states(data->states);
	if (data->state_behaviors)
		free(data->state_behaviors);
	free(data);
}
