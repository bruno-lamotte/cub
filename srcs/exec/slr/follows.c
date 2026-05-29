#include "cub.h"
#include <stdlib.h>

int	does_list_contains_this_symbol(t_list *list, char *name);

void	add_dollar_to_start_symbol(t_slr1 *data)
{
	t_list		*new_list;
	t_symbol	*symbol;
	char		*dup;
	t_rule		*rule;

	rule = (t_rule *)data->rules->content;
	symbol = get_symbol_from_name(data, rule->left_symbol);
	dup = ft_strdup("$");
	if (!dup)
		return ;
	new_list = ft_lstnew(dup);
	if (!new_list)
	{
		free(dup);
		return ;
	}
	ft_lstadd_back(&symbol->follows, new_list);
}

static int	add_follow_node(t_symbol **symbol, char *content)
{
	t_list	*node;
	char	*dup;

	if (does_list_contains_this_symbol((*symbol)->follows, content))
		return (0);
	dup = ft_strdup(content);
	if (!dup)
		return (0);
	node = ft_lstnew(dup);
	if (!node)
	{
		free(dup);
		return (0);
	}
	ft_lstadd_back(&(*symbol)->follows, node);
	return (1);
}

int	add_follows_from_follows(t_symbol **symbol, t_symbol *next_symbol)
{
	t_list	*list;
	int		added;

	added = 0;
	list = next_symbol->follows;
	while (list)
	{
		added += add_follow_node(symbol, (char *)list->content);
		list = list->next;
	}
	return (added);
}

int	add_follows_from_firsts(t_symbol **symbol, t_symbol *next_symbol)
{
	t_list	*list;
	int		added;

	added = 0;
	list = next_symbol->firsts;
	while (list)
	{
		if (ft_strcmp((char *)list->content, "EMPTY"))
			added += add_follow_node(symbol, (char *)list->content);
		list = list->next;
	}
	return (added);
}

int	contains_empty_in_firsts(t_symbol *symbol)
{
	t_list	*current;

	current = symbol->firsts;
	while (current)
	{
		if (!ft_strcmp((char *)current->content, "EMPTY"))
			return (1);
		current = current->next;
	}
	return (0);
}

static void	update_follow_rule(t_slr1 *data, t_rule *rule, int *added)
{
	t_list		*rights;
	t_symbol	*tmp;
	t_symbol	*next_tmp;
	t_symbol	*left_sym;

	rights = rule->right_symbols;
	tmp = get_symbol_from_name(data, (char *)rights->content);
	left_sym = get_symbol_from_name(data, rule->left_symbol);
	while (rights->next)
	{
		next_tmp = get_symbol_from_name(data, (char *)rights->next->content);
		*added += add_follows_from_firsts(&tmp, next_tmp);
		if (contains_empty_in_firsts(next_tmp))
			*added += add_follows_from_follows(&tmp, left_sym);
		tmp = next_tmp;
		rights = rights->next;
	}
	if (!rights->next && ft_strcmp(tmp->name, rule->left_symbol))
		*added += add_follows_from_follows(&tmp, left_sym);
}

void	update_follows(t_slr1 *data, t_list *current_rule, int *added)
{
	while (current_rule)
	{
		update_follow_rule(data, (t_rule *)current_rule->content, added);
		current_rule = current_rule->next;
	}
}
