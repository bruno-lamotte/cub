/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   slr_parser.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: blamotte <blamotte@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/01 20:55:38 by marvin            #+#    #+#             */
/*   Updated: 2026/05/22 15:48:12 by blamotte         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub.h"

static void	dummy_free(void *content)
{
	(void)content;
}

/* ************************************************************************** */
/*                               closure.c                                    */
/* ************************************************************************** */

t_item  *create_new_item(t_rule *rule, int dot_pos)
{
	t_item      *new_item;
	static int  id = 0;

	new_item = malloc(sizeof(t_item));
	if (!new_item)
		return (NULL);
	new_item->id = ++id;
	new_item->rule_of_item = rule;
	new_item->dot_pos = dot_pos;
	return (new_item);
}

t_symbol *get_symbol_from_list(t_slr1 *data, t_list *right_symbols, int dot_pos)
{
	t_list  *current;

	current = right_symbols;
	while (dot_pos--)
	{
		if (!current)
			return (NULL);
		current = current->next;
	}
	return (get_symbol_from_name(data, (char *)current->content));
}

t_symbol	*get_symbol_after_dot(t_slr1 *data, t_item *item)
{
	t_symbol	*symbol;

	if (item->dot_pos >= item->rule_of_item->nb_items)
		return (NULL);
	symbol = get_symbol_from_list(data,
			item->rule_of_item->right_symbols, item->dot_pos);
	return (symbol);
}

t_symbol	*get_non_terminal_symbol_after_dot(t_slr1 *data, t_item *item)
{
	t_symbol	*symbol;

	symbol = get_symbol_after_dot(data, item);
	if (symbol && !symbol_is_token(symbol->name))
		return (symbol);
	symbol = NULL;
	return (symbol);
}

int	does_state_contains_this_item(t_list *items, t_item *item)
{
	t_list	*current;
	t_item	*current_item;

	current = items;
	while (current)
	{
		current_item = (t_item *)current->content;
		if (current_item->rule_of_item == item->rule_of_item
			&& current_item->dot_pos == item->dot_pos)
			return (1);
		current = current->next;
	}
	return (0);
}

static void	add_closure_items(t_state *state, t_list *rules, char *name)
{
	t_item	*new;

	while (rules)
	{
		if (ft_strcmp(((t_rule *)rules->content)->left_symbol, name))
			break ;
		new = create_new_item((t_rule *)rules->content, 0);
		if (!new)
			return ;
		if (!does_state_contains_this_item(state->items, new))
			ft_lstadd_back(&state->items, ft_lstnew(new));
		else
			free(new);
		rules = rules->next;
	}
}

void	closure(t_slr1 *data, t_state *state)
{
	t_list		*curr;
	t_symbol	*sym;

	curr = state->items;
	while (curr)
	{
		sym = get_non_terminal_symbol_after_dot(data,
				(t_item *)curr->content);
		if (sym)
			add_closure_items(state, get_rule_from_symbolname(data, sym->name),
				sym->name);
		curr = curr->next;
	}
}

/* ************************************************************************** */
/*                               free_all.c                                   */
/* ************************************************************************** */

static void	free_item(void *content)
{
	if (!content)
		return ;
	free(content);
}

static void	free_transition(void *content)
{
	t_transition        *transition;

	if (!content)
		return ;
	transition = (t_transition *)content;
	free(transition->symbol);
	free(transition);
}

void	free_states(t_list *states)
{
	t_list  *current;
	t_state *state;

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

/* ************************************************************************** */
/*                               get_firsts.c                                 */
/* ************************************************************************** */

int	does_list_contains_this_symbol(t_list *list, char *name)
{
	while (list)
	{
		if (!ft_strcmp((char *)list->content, name))
			return (1);
		list = list->next;
	}
	return (0);
}

int	add_firsts_if_not_token(t_slr1 *data, t_rule *next_rule,
		t_symbol *left_symbol)
{
	t_list	*new_list;
	t_list	*new_node;
	char	*dup;

	new_list = get_symbol_from_name(data, next_rule->left_symbol)->firsts;
	if (!new_list)
		return (0);
	while (new_list)
	{
		if (!does_list_contains_this_symbol(left_symbol->firsts,
				(char *)new_list->content))
		{
			dup = ft_strdup((char *)new_list->content);
			if (!dup)
				return (0);
			new_node = ft_lstnew(dup);
			if (!new_node)
				return (free(dup), 0);
			ft_lstadd_back(&left_symbol->firsts, new_node);
		}
		new_list = new_list->next;
	}
	return (1);
}

int	add_first_if_token(t_symbol *left_symbol, t_symbol *right_symbol)
{
	t_list	*new_list;
	char	*dup;

	dup = ft_strdup(right_symbol->name);
	if (!dup)
		return (0);
	new_list = ft_lstnew(dup);
	if (!new_list)
	{
		free(dup);
		return (0);
	}
	ft_lstadd_back(&left_symbol->firsts, new_list);
	return (1);
}

int	should_look_for_next_right_symbol(t_symbol *left_symbol, t_rule *rule)
{
	if (!rule->right_symbols->next)
		return (0);
	return (does_list_contains_this_symbol(left_symbol->firsts, "EMPTY")
		&& rule->right_symbols->next->content);
}

static int	dfs_non_token(t_slr1 *data, t_rule *rule, t_symbol *left,
		t_symbol *right)
{
	t_list		*node;
	t_rule		*next;
	t_symbol	*next_sym;

	node = get_rule_from_symbolname(data, right->name);
	next = (t_rule *)node->content;
	if (rule != next)
	{
		next_sym = get_symbol_from_name(data,
				(char *)next->right_symbols->content);
		get_first_dfs(data, next, left, next_sym);
	}
	return (add_firsts_if_not_token(data, next, left));
}

void	get_first_dfs(t_slr1 *data, t_rule *rule, t_symbol *left_symbol,
			t_symbol *right_symbol)
{
	while (1)
	{
		if (!does_list_contains_this_symbol(left_symbol->firsts,
				right_symbol->name))
		{
			if (symbol_is_token(right_symbol->name))
			{
				if (!add_first_if_token(left_symbol, right_symbol))
					return ;
			}
			else if (!dfs_non_token(data, rule, left_symbol, right_symbol))
				return ;
		}
		if (should_look_for_next_right_symbol(left_symbol, rule))
			right_symbol = get_symbol_from_name(data,
					(char *)rule->right_symbols->next->content);
		else
			return ;
	}
}

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

/* ************************************************************************** */
/*                               get_follows.c                                */
/* ************************************************************************** */

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

void    get_follows(t_slr1 *data)
{
	t_list  *current_rule;
	int     added;

	added = 1;
	current_rule = data->rules;
	add_dollar_to_start_symbol(data);
	while (added)
	{
		added = 0;
		update_follows(data, current_rule, &added);
	}
}

/* ************************************************************************** */
/*                               get_rules.c                                  */
/* ************************************************************************** */

void	get_leftsymbol_from_grammar(t_slr1 *data, t_rule **new_rule,
				char *line)
{
	int		i;
	t_rule	*last;

	i = 0;
	if (line[i] == ' ' || line[i] == '|')
	{
		last = (t_rule *)ft_lstlast(data->rules)->content;
		(*new_rule)->left_symbol = ft_strdup(last->left_symbol);
	}
	else
	{
		while (line[i] != ' ' && line[i] != ':')
			i++;
		(*new_rule)->left_symbol = ft_substr(line, 0, i);
	}
}

void	get_id_from_grammar(t_slr1 *data, t_rule **new_rule)
{
	t_rule	*last;

	if (data->rules == NULL)
		(*new_rule)->id = 1;
	else
	{
		last = (t_rule *)ft_lstlast(data->rules)->content;
		(*new_rule)->id = last->id + 1;
	}
}

void	get_rightsymbols_from_grammar(t_rule **new_rule,
				char *line)
{
	int		i;
	char	*symbol;
	t_list	*new_list;

	while (*line && *line != '\n' && *line != '\0' && *line != '\r')
	{
		i = 0;
		while (*line == ' ')
			line++;
		while (line[i] && line[i] != ' ' && line[i] != '\n'
			&& line[i] != '\r')
			i++;
		symbol = ft_substr(line, 0, i);
		new_list = ft_lstnew(symbol);
		ft_lstadd_back(&(*new_rule)->right_symbols, new_list);
		line += i;
	}
}

void	get_nbitems_from_grammar(t_rule **new_rule)
{
	int		i;
	t_list	*tmp;

	tmp = (*new_rule)->right_symbols;
	i = 1;
	while (tmp->next)
	{
		tmp = tmp->next;
		i++;
	}
	(*new_rule)->nb_items = i;
}

void	get_rule_from_grammar(t_slr1 *data, t_rule **new_rule, char *line)
{
	get_id_from_grammar(data, new_rule);
	get_leftsymbol_from_grammar(data, new_rule, line);
	while (*line && *line != '|' && *line != ':' && *line != '\n'
		&& *line != '\r')
		line++;
	get_rightsymbols_from_grammar(new_rule, ++line);
	get_nbitems_from_grammar(new_rule);
}

int	parse_grammar(t_slr1 *data)
{
	int		fd;
	char	*line;
	t_rule	*new_rule;

	fd = open("grammars/grammar_bonus++.txt", O_RDONLY);
	if (fd == -1)
		return (0);
	line = get_next_line(fd);
	while (line && *line != '\n' && *line != '\0' && *line != '\r')
	{
		new_rule = malloc(sizeof(t_rule));
		if (!new_rule)
			return (close(fd), free(line), 0);
		ft_bzero(new_rule, sizeof(t_rule));
		get_rule_from_grammar(data, &new_rule, line);
		ft_lstadd_back(&data->rules, ft_lstnew(new_rule));
		free(line);
		line = get_next_line(fd);
	}
	if (line)
		free(line);
	return (close(fd), 1);
}

/* ************************************************************************** */
/*                               get_symbols.c                                */
/* ************************************************************************** */

t_list	*get_rule_from_symbolname(t_slr1 *data, char *symbol)
{
	t_list	*current;

	current = data->rules;
	while (current)
	{
		if (!ft_strcmp(((t_rule *)current->content)->left_symbol, symbol))
			return (current);
		current = current->next;
	}
	return (NULL);
}

t_symbol	*get_symbol_from_name(t_slr1 *data, char *left_symbol)
{
	t_list	*current;

	current = data->symbols;
	while (current)
	{
		if (!ft_strcmp(((t_symbol *)current->content)->name, left_symbol))
			return ((t_symbol *)current->content);
		current = current->next;
	}
	return (NULL);
}

int	symbol_is_token(char *name)
{
	return (*name < 97);
}

void	complete_symbol_name(t_symbol **symbol, char *name)
{
	(*symbol)->name = ft_strdup(name);
	if (!(*symbol)->name)
		return ;
}

int	symbol_already_parsed(t_list *symbols, char *left_symbol)
{
	while (symbols)
	{
		if (!ft_strcmp(((t_symbol *)symbols->content)->name, left_symbol))
			return (1);
		symbols = symbols->next;
	}
	return (0);
}

void	add_symbol_if_not_exists(t_slr1 *data, char *name, int *nbr)
{
	t_symbol	*new_symbol;

	if (!name || symbol_already_parsed(data->symbols, name))
		return ;
	new_symbol = malloc(sizeof(t_symbol));
	if (!new_symbol)
		return ;
	ft_bzero(new_symbol, sizeof(t_symbol));
	complete_symbol_name(&new_symbol, name);
	if (symbol_is_token(new_symbol->name))
		ft_lstadd_back(&new_symbol->firsts, ft_lstnew(new_symbol->name));
	new_symbol->nbr = (*nbr)++;
	ft_lstadd_back(&data->symbols, ft_lstnew(new_symbol));
}

void	get_symbols(t_slr1 *data)
{
	t_list		*curr;
	t_list		*right;
	t_rule		*rule;
	int			nbr;

	nbr = 0;
	add_symbol_if_not_exists(data, "$", &nbr);
	curr = data->rules;
	while (curr)
	{
		rule = (t_rule *)curr->content;
		add_symbol_if_not_exists(data, rule->left_symbol, &nbr);
		right = rule->right_symbols;
		while (right)
		{
			add_symbol_if_not_exists(data, (char *)right->content, &nbr);
			right = right->next;
		}
		curr = curr->next;
	}
}

/* ************************************************************************** */
/*                               get_table.c                                  */
/* ************************************************************************** */

int	get_transition_from_symbol(t_state *state, char *symbol_name)
{
	t_list			*transition;
	t_transition	*t;

	transition = state->transitions;
	while (transition)
	{
		t = (t_transition *)transition->content;
		if (!ft_strcmp(t->symbol, symbol_name))
			return (t->dest_state->id);
		transition = transition->next;
	}
	return (0);
}

void	fill_table_when_reduce(t_slr1 *data, int ***table, t_list *state,
			t_list *item)
{
	t_rule		*rule;
	t_symbol	*symbol;
	t_list		*sym_node;
	int			sid;

	rule = ((t_item *)item->content)->rule_of_item;
	sid = ((t_state *)state->content)->id;
	sym_node = data->symbols;
	while (sym_node)
	{
		symbol = (t_symbol *)sym_node->content;
		if (rule->id == 1 && !ft_strcmp(symbol->name, "$"))
			(*table)[sid][symbol->nbr] = ACCEPTED;
		else if ((*table)[sid][symbol->nbr] == 0)
			(*table)[sid][symbol->nbr] = -(rule->id);
		sym_node = sym_node->next;
	}
}

void	fill_parsing_table(t_slr1 *data, int ***table)
{
	t_symbol	*symbol;
	t_list		*state;
	t_list		*item;
	t_state		*st;

	state = data->states;
	while (state)
	{
		st = (t_state *)state->content;
		item = st->items;
		while (item)
		{
			symbol = get_symbol_after_dot(data, (t_item *)item->content);
			if (symbol)
				(*table)[st->id][symbol->nbr] =
					get_transition_from_symbol(st, symbol->name);
			else
				fill_table_when_reduce(data, table, state, item);
			item = item->next;
		}
		state = state->next;
	}
}

t_state	*initialize_first_state(t_slr1 *data)
{
	t_state	*first_state;
	t_item	*first_item;

	first_state = malloc(sizeof(t_state));
	if (!first_state)
		return (NULL);
	ft_bzero(first_state, sizeof(t_state));
	first_state->id = 0;
	first_item = create_new_item((t_rule *)data->rules->content, 0);
	if (!first_item)
	{
		free(first_state);
		return (NULL);
	}
	ft_lstadd_back(&first_state->items, ft_lstnew(first_item));
	first_state->transitions = NULL;
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
	closure(data, first_state);
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
}

static void	build_slr_states(t_slr1 *data)
{
	parse_grammar(data);
	get_symbols(data);
	get_firsts(data);
	get_follows(data);
	get_states(data);
}

int	**create_parsing_table(t_slr1 *data)
{
	int		**table;
	int		*data_block;
	int		size;
	int		nb_symbols;
	int		i;

	i = 0;
	build_slr_states(data);
	size = ft_lstsize(data->states);
	nb_symbols = ft_lstsize(data->symbols);
	table = malloc(sizeof(int *) * (size + 1));
	if (!table)
		return (NULL);
	data_block = malloc(sizeof(int) * size * nb_symbols);
	if (!data_block)
		return (free(table), NULL);
	ft_bzero(data_block, sizeof(int) * size * nb_symbols);
	while (i < size)
	{
		table[i] = &data_block[i * nb_symbols];
		i++;
	}
	table[size] = NULL;
	fill_parsing_table(data, &table);
	return (table);
}

/* ************************************************************************** */
/*                               go_to.c                                      */
/* ************************************************************************** */

void	free_tmp_state(t_state *state)
{
	t_list			*current;
	t_transition	*transition;

	if (!state)
		return ;
	ft_lstclear(&state->items, free);
	current = state->transitions;
	while (current)
	{
		transition = (t_transition *)current->content;
		if (transition)
		{
			free(transition->symbol);
			free(transition);
		}
		current = current->next;
	}
	ft_lstclear(&state->transitions, dummy_free);
	free(state);
}

t_transition	*create_transition(char *symbol_name, t_state *dest_state)
{
	t_transition	*transition;

	transition = malloc(sizeof(t_transition));
	if (!transition)
		return (NULL);
	transition->symbol = ft_strdup(symbol_name);
	if (!transition->symbol)
	{
		free(transition);
		return (NULL);
	}
	transition->dest_state = dest_state;
	return (transition);
}

t_state	*create_new_state(t_slr1 *data)
{
	t_state	*new_state;

	new_state = malloc(sizeof(t_state));
	if (!new_state)
		return (NULL);
	ft_bzero(new_state, sizeof(t_state));
	if (!data->states)
		new_state->id = 0;
	else
		new_state->id = ((t_state *)ft_lstlast(data->states)->content)->id
			+ 1;
	return (new_state);
}

void	add_item_to_list(t_state **new_state, t_item *item)
{
	t_item	*new_item;

	new_item = create_new_item(item->rule_of_item, item->dot_pos + 1);
	if (!new_item)
		return ;
	ft_lstadd_back(&(*new_state)->items, ft_lstnew(new_item));
}

int	are_items_equal(t_item *i1, t_item *i2)
{
	return (i1->rule_of_item == i2->rule_of_item && i1->dot_pos == i2->dot_pos);
}

static int	state_items_match(t_state *s1, t_state *s2)
{
	t_list	*it1;
	t_list	*it2;

	if (ft_lstsize(s1->items) != ft_lstsize(s2->items))
		return (0);
	it1 = s1->items;
	it2 = s2->items;
	while (it1 && it2)
	{
		if (!are_items_equal((t_item *)it1->content, (t_item *)it2->content))
			return (0);
		it1 = it1->next;
		it2 = it2->next;
	}
	return (1);
}

t_state	*find_state(t_slr1 *data, t_state *new_state)
{
	t_list	*curr;

	curr = data->states;
	while (curr)
	{
		if (state_items_match((t_state *)curr->content, new_state))
			return ((t_state *)curr->content);
		curr = curr->next;
	}
	return (NULL);
}

void	add_transition_to_state(t_slr1 *data, t_state *current_state,
			t_state *new_state, t_symbol *symbol)
{
	t_state			*transition_state;
	t_transition	*transition;

	transition_state = find_state(data, new_state);
	if (!transition_state)
	{
		ft_lstadd_back(&data->states, ft_lstnew(new_state));
		transition = create_transition(symbol->name, new_state);
	}
	else
	{
		transition = create_transition(symbol->name, transition_state);
		free_tmp_state(new_state);
	}
	ft_lstadd_back(&current_state->transitions, ft_lstnew(transition));
}

void	get_new_state(t_slr1 *data, t_state *current_state,
			t_state *new_state, t_symbol *target_symbol)
{
	t_list		*current_item;
	t_symbol	*symbol;

	current_item = current_state->items;
	while (current_item)
	{
		symbol = get_symbol_after_dot(data, (t_item *)current_item->content);
		if (symbol && !ft_strcmp(symbol->name, target_symbol->name))
		{
			add_item_to_list(&new_state, (t_item *)current_item->content);
		}
		current_item = current_item->next;
	}
	closure(data, new_state);
	add_transition_to_state(data, current_state, new_state, target_symbol);
}

static int	process_goto_symbol(t_slr1 *data, t_state *state,
		t_symbol *symbol, t_list **seen)
{
	t_state	*new_state;

	if (does_list_contains_this_symbol(*seen, symbol->name))
		return (1);
	ft_lstadd_back(seen, ft_lstnew(symbol->name));
	new_state = create_new_state(data);
	if (!new_state)
		return (0);
	get_new_state(data, state, new_state, symbol);
	return (1);
}

void	go_to(t_slr1 *data, t_state *state)
{
	t_list		*curr;
	t_symbol	*sym;
	t_list		*seen;

	seen = NULL;
	curr = state->items;
	while (curr)
	{
		sym = get_symbol_after_dot(data, (t_item *)curr->content);
		if (sym)
		{
			if (!process_goto_symbol(data, state, sym, &seen))
			{
				ft_lstclear(&seen, dummy_free);
				return ;
			}
		}
		curr = curr->next;
	}
	ft_lstclear(&seen, dummy_free);
}

/* ************************************************************************** */
/*                               print_table.c                                */
/* ************************************************************************** */

static void	print_part_header(int i)
{
	if (i != 0)
	{
		printf("\tinit_table_part_%d(t);\n", i / 12);
		printf("%c\n\n", 125);
	}
	printf("void\tinit_table_part_%d(int **t)\n", i / 12);
	printf("%c\n", 123);
}

static void	print_table_header(t_list *sym)
{
	printf("%-6s|", "ETAT");
	while (sym)
	{
		printf(" %-16s|", ((t_symbol *)sym->content)->name);
		sym = sym->next;
	}
	printf("\n");
}

void	print_table(int **table, t_slr1 *data)
{
	int		i;
	int		j;
	int		nb_sym;

	nb_sym = ft_lstsize(data->symbols);
	print_table_header(data->symbols);
	i = -1;
	while (table[++i])
	{
		printf("%-6d|", i);
		j = -1;
		while (++j < nb_sym)
		{
			if (table[i][j] == ACCEPTED)
				printf(" %-16s|", "ACC");
			else if (table[i][j] == 0)
				printf(" %-16s|", ".");
			else
				printf(" %-16d|", table[i][j]);
		}
		printf("\n");
	}
}

void	print_table_in_c(int **table, int nb_states, int nb_tokens)
{
	int	i;
	int	j;

	printf("#include \"cub.h\"\n\n");
	i = -1;
	while (++i < nb_states)
	{
		if (i % 12 == 0)
			print_part_header(i);
		printf("\tset_row(t, %d, \"", i);
		j = -1;
		while (++j < nb_tokens)
		{
			printf("%d", table[i][j]);
			if (j < nb_tokens - 1)
				printf(" ");
			if ((j + 1) % 20 == 0 && j < nb_tokens - 1)
				printf("\"\n\t\t\"");
		}
		printf("\");\n");
	}
	if (nb_states > 0)
		printf("}\n");
}

static void	print_rules_header(int i)
{
	if (i != 0)
	{
		printf("\tinit_rules_part_%d(rules);\n", i / 12);
		printf("%c\n\n", 125);
	}
	printf("void\tinit_rules_part_%d", i / 12);
	printf("(t_reduce_rule *rules)\n");
	printf("%c\n", 123);
}

void	print_rules_in_c(t_list *rules_list)
{
	t_rule	*rule;
	int		i;

	i = 0;
	while (rules_list)
	{
		rule = (t_rule *)rules_list->content;
		if (i % 12 == 0)
			print_rules_header(i);
		printf("\trules[%d].left_symbol = \"%s\";\n",
			rule->id, rule->left_symbol);
		printf("\trules[%d].nb_items = %d;\n", rule->id, rule->nb_items);
		rules_list = rules_list->next;
		i++;
	}
	if (i > 0)
		printf("%c\n", 125);
}

void	print_symbols_in_c(t_list *symbols_list)
{
	t_symbol	*symbol;
	int			i;

	i = 0;
	while (symbols_list)
	{
		symbol = (t_symbol *)symbols_list->content;
		if (i % 24 == 0)
		{
			if (i != 0)
			{
				printf("\tinit_symbols_part_%d(symbols);\n", i / 24);
				printf("%c\n\n", 125);
			}
			printf("void\tinit_symbols_part_%d"
				"(char **symbols)\n", i / 24);
			printf("%c\n", 123);
		}
		printf("\tsymbols[%d] = \"%s\";\n", i, symbol->name);
		symbols_list = symbols_list->next;
		i++;
	}
	if (i > 0)
		printf("\tsymbols[%d] = NULL;\n%c\n", i, 125);
}

int	slr_main(void)
{
	int		**table;
	t_slr1	*data;

	data = malloc(sizeof(t_slr1));
	if (!data)
		return (1);
	initialize_data(data);
	table = create_parsing_table(data);
	if (!table)
	{
		free(data);
		return (1);
	}
	print_table_in_c(table, ft_lstsize(data->states),
		ft_lstsize(data->symbols));
	print_rules_in_c(data->rules);
	print_symbols_in_c(data->symbols);
	free_all(data, table);
	return (0);
}
