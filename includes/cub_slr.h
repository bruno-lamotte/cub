#ifndef CUB_SLR_H
# define CUB_SLR_H

# include "cub_types.h"

t_item			*create_new_item(t_rule *rule, int dot_pos);
t_symbol		*get_symbol_from_list(t_slr1 *data, t_list *right_symbols,
					int dot_pos);
t_symbol		*get_symbol_after_dot(t_slr1 *data, t_item *item);
t_symbol		*get_non_terminal_symbol_after_dot(t_slr1 *data, t_item *item);
int				does_state_contains_this_item(t_list *items, t_item *item);
void			closure(t_slr1 *data, t_state *state);
void			dummy_free(void *content);

void			free_states(t_list *states);
void			free_symbols(t_list *symbols);
void			free_rules(t_list *rules);
void			free_table(int **table);
void			free_all(t_slr1 *data, int **table);

int				does_list_contains_this_symbol(t_list *list, char *name);
int				add_firsts_if_not_token(t_slr1 *data, t_rule *next_rule,
					t_symbol *left_symbol);
int				add_first_if_token(t_symbol *left_symbol,
					t_symbol *right_symbol);
int				should_look_for_next_right_symbol(t_symbol *left_symbol,
					t_rule *rule);
void			get_first_dfs(t_slr1 *data, t_rule *rule, t_symbol *left_symbol,
					t_symbol *right_symbol);
void			get_firsts(t_slr1 *data);

void			add_dollar_to_start_symbol(t_slr1 *data);
int				add_follows_from_follows(t_symbol **symbol,
					t_symbol *next_symbol);
int				add_follows_from_firsts(t_symbol **symbol,
					t_symbol *next_symbol);
int				contains_empty_in_firsts(t_symbol *symbol);
void			update_follows(t_slr1 *data, t_list *current_rule, int *added);
void			get_follows(t_slr1 *data);

void			get_leftsymbol_from_grammar(t_slr1 *data, t_rule **new_rule,
					char *line);
void			get_id_from_grammar(t_slr1 *data, t_rule **new_rule);
void			get_rightsymbols_from_grammar(t_rule **new_rule, char *line);
void			get_nbitems_from_grammar(t_rule **new_rule);
void			get_rule_from_grammar(t_slr1 *data, t_rule **new_rule,
					char *line);
int				parse_grammar(t_slr1 *data);

t_list			*get_rule_from_symbolname(t_slr1 *data, char *symbol);
t_symbol		*get_symbol_from_name(t_slr1 *data, char *left_symbol);
int				symbol_is_token(char *name);
void			complete_symbol_name(t_symbol **symbol, char *name);
int				symbol_already_parsed(t_list *symbols, char *left_symbol);
void			add_symbol_if_not_exists(t_slr1 *data, char *name, int *nbr);
void			get_symbols(t_slr1 *data);

int				get_transition_from_symbol(t_state *state, char *symbol_name);
void			fill_table_when_reduce(t_slr1 *data, int ***table,
					t_list *state, t_list *item);
void			fill_parsing_table(t_slr1 *data, int ***table);
t_state			*initialize_first_state(t_slr1 *data);
void			get_states(t_slr1 *data);
void			initialize_data(t_slr1 *data);
int				**create_parsing_table(t_slr1 *data);
int				slr_main(void);

void			free_tmp_state(t_state *state);
t_transition	*create_transition(char *symbol_name, t_state *dest_state);
t_state			*create_new_state(t_slr1 *data);
void			add_item_to_list(t_state **new_state, t_item *item);
int				are_items_equal(t_item *i1, t_item *i2);
t_state			*find_state(t_slr1 *data, t_state *new_state);
void			add_transition_to_state(t_slr1 *data, t_state *current_state,
					t_state *new_state, t_symbol *symbol);
void			get_new_state(t_slr1 *data, t_state *current_state,
					t_state *new_state, t_symbol *target_symbol);
void			go_to(t_slr1 *data, t_state *state);

void			print_table(int **table, t_slr1 *data);
void			print_table_in_c(int **table, int nb_states, int nb_tokens);
void			print_rules_in_c(t_list *rules_list);
void			print_symbols_in_c(t_list *symbols_list);

#endif
