#ifndef CUB_TYPES_SLR_H
# define CUB_TYPES_SLR_H

struct s_rule;
struct s_engine;
struct s_monster_rt;

struct s_worker;
typedef void (*t_behavior_fn)(struct s_monster_rt *, struct s_engine *, struct s_worker *);

typedef struct s_item
{
	int				id;
	struct s_rule	*rule_of_item;
	int				dot_pos;
}				t_item;

typedef struct s_symbol
{
	char			*name;
	int				nbr;
	t_list			*firsts;
	t_list			*follows;
}				t_symbol;

typedef struct s_rule
{
	int				id;
	char			*left_symbol;
	t_list			*right_symbols;
	int				nb_items;
}				t_rule;

typedef struct s_state
{
	int				id;
	t_list			*items;
	t_list			*transitions;
}				t_state;

typedef struct s_transition
{
	char			*symbol;
	t_state			*dest_state;
}				t_transition;

typedef struct s_slr1
{
	t_list			*rules;
	t_list			*symbols;
	t_list			*states;
	int				**table;
	t_behavior_fn	*state_behaviors;
	int				stim_alarm_heard_nbr;
	int				stim_alarm_off_nbr;
}				t_slr1;

typedef struct s_reduce_rule
{
	char			*left_symbol;
	int				nb_items;
}				t_reduce_rule;

#endif
