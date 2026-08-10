#ifndef CUB_SOLVER_H
# define CUB_SOLVER_H

# include "cub_types.h"

# define MAX_PATH_LEN 256

typedef struct s_path
{
	int		len;
	t_vec2	steps[MAX_PATH_LEN];
}				t_path;

typedef struct s_solver_checkpoint
{
	t_player_rt	player;
	void		*blob_copy;
	int			alarm_triggered;
	int			hacking_timer;
}				t_solver_checkpoint;

typedef struct s_map_state
{
	t_vec2		player_pos;
	uint64_t	door_mask;
	uint64_t	alarm_mask;
	uint64_t	monster_mask;
}				t_map_state;

typedef struct s_bst
{
	t_map_state			*state;
	struct s_bst		*left;
	struct s_bst		*right;
}						t_bst;

typedef struct s_qnode
{
	t_solver_checkpoint	*cp;
	int					depth;
	struct s_qnode		*next;
}						t_qnode;

typedef struct s_queue
{
	t_qnode	*front;
	t_qnode	*rear;
}			t_queue;

typedef struct s_bfs_state
{
	int	*queue;
	int	*parent;
	int	width;
	int	height;
	int	head;
	int	tail;
}				t_bfs_state;

typedef struct s_solver_ctx
{
	t_engine	*eng;
	t_worker	*worker;
	t_bst		**visited;
	t_queue		*q;
	int			depth;
}				t_solver_ctx;

/* solver_checkpoint.c */
t_solver_checkpoint	*clone_checkpoint(t_engine *eng);
void				restore_checkpoint(t_engine *eng, t_solver_checkpoint *cp);
void				free_checkpoint(t_solver_checkpoint *cp);
void				init_dummy_textures(t_data *d);

/* solver_utils.c */
int					is_monster_at_cell(t_engine *eng, t_vec2 cell);
int					is_door_blocked(t_engine *eng, t_vec2 cell);
int					is_tile_passable(t_engine *eng, t_vec2 cell);
t_vec2				find_walkable_neighbor(t_engine *eng, t_vec2 target);
int					is_adjacent_to_terminal(t_engine *eng, t_vec2 *term_pos);

/* solver_bfs_path.c */
int					bfs_find_path(t_engine *eng, t_worker *worker,
						t_vec2 st_tg[3], t_path *path);

/* solver_simulation.c */
void				simulate_engine_tick(t_engine *eng, t_worker *w);
int					simulate_walk_path(t_engine *eng, t_worker *worker,
						t_path *path);
int					simulate_move_to_target(t_engine *eng, t_worker *worker,
						t_vec2 target);

/* solver_state.c */
uint64_t			get_alarm_mask(t_engine *eng);
uint64_t			get_door_mask(t_engine *eng);
uint64_t			get_monster_mask(t_engine *eng);
int					compare_states(t_map_state *a, t_map_state *b);

/* solver_visited.c */
int					is_visited(t_bst **root, t_map_state *state);
void				insert_visited(t_bst **root, t_map_state *state);
void				free_bst(t_bst *node);

/* solver_queue.c */
void				init_queue(t_queue *q);
int					enqueue(t_queue *q, t_solver_checkpoint *cp, int depth);
t_solver_checkpoint	*dequeue(t_queue *q, int *depth);
void				clear_queue(t_queue *q);

/* solver_targets.c */
void				toggle_door_from_terminal(t_engine *eng, int door_idx);
void				toggle_alarm_from_terminal(t_engine *eng, int alarm_idx);
void				find_exit(t_data *d, t_vec2 *exit_pos);
int					try_branch_bfs(t_solver_ctx *ctx, t_vec2 target);
int					try_all_targets_bfs(t_solver_ctx *ctx, t_vec2 cur_t);

/* solver_branching.c */
int					branch_terminal_wait(t_solver_ctx *ctx);
int					branch_terminal_doors(t_solver_ctx *ctx);
int					branch_terminal_alarms(t_solver_ctx *ctx);
int					bfs_loop_step(t_solver_ctx *ctx);

/* solver_search.c */
int					is_map_solvable(t_data *d);

#endif
