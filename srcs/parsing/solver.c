#include "../../includes/cub.h"
#include <stdlib.h>

typedef struct s_solver_door
{
	int	x;
	int	y;
}				t_solver_door;

typedef struct s_solver_monster
{
	int	x;
	int	y;
	int	dir_x;
	int	dir_y;
	int	alarm_x;
	int	alarm_y;
}				t_solver_monster;

typedef struct s_solver_term
{
	int	x;
	int	y;
}				t_solver_term;

typedef struct s_solver_alarm
{
	int	x;
	int	y;
}				t_solver_alarm;

typedef struct s_solver_context
{
	t_data				*data;
	int					exit_x;
	int					exit_y;
	t_solver_door		doors[64];
	int					door_count;
	t_solver_monster	monsters[64];
	int					monster_count;
	t_solver_term		terms[64];
	int					term_count;
	t_solver_alarm		alarms[64];
	int					alarm_count;
}						t_solver_context;

typedef struct s_map_state
{
	uint16_t			player_x;
	uint16_t			player_y;
	uint64_t			door_mask;
	uint64_t			monster_mask;
	int					state_index;
	int					steps;
	struct s_map_state	*parent;
}						t_map_state;

typedef struct s_bst
{
	t_map_state			*state;
	struct s_bst		*left;
	struct s_bst		*right;
}						t_bst;

typedef struct s_qnode
{
	t_map_state			*state;
	struct s_qnode		*next;
}						t_qnode;

typedef struct s_queue
{
	t_qnode				*front;
	t_qnode				*rear;
}						t_queue;

typedef struct s_bfs_var
{
	t_bst				*tree;
	t_queue				*q;
	int					*counter;
}						t_bfs_var;

static void	find_exit(t_data *d, int *ex, int *ey)
{
	int	r;
	int	c;

	*ex = -1;
	*ey = -1;
	r = 0;
	while (r < d->m_height)
	{
		c = 0;
		while (c < d->m_max_width && d->m_map[r][c])
		{
			if (d->have_bonus && d->b_map[r][c] == 'A')
				return ((void)(*ex = c, *ey = r));
			if (d->m_map[r][c] == 'A')
				return ((void)(*ex = c, *ey = r));
			c++;
		}
		r++;
	}
}

static void	scan_obj(t_solver_context *ctx, char ch, int r, int c)
{
	char	sym;

	sym = ctx->data->obj_defs[get_def_id(ctx->data->obj_ids, ch)].symbol;
	if (sym == 'T' && ctx->term_count < 64)
	{
		ctx->terms[ctx->term_count].x = c;
		ctx->terms[ctx->term_count++].y = r;
	}
	else if (sym == 'L' && ctx->alarm_count < 64)
	{
		ctx->alarms[ctx->alarm_count].x = c;
		ctx->alarms[ctx->alarm_count++].y = r;
	}
}

static void	scan_cell(t_solver_context *ctx, int r, int c)
{
	char	ch;

	ch = ctx->data->m_map[r][c];
	if (ft_isset(ch, ctx->data->door_ids) && ctx->door_count < 64)
	{
		ctx->doors[ctx->door_count].x = c;
		ctx->doors[ctx->door_count++].y = r;
	}
	else if (ft_isset(ch, ctx->data->monster_ids) && ctx->monster_count < 64)
	{
		ctx->monsters[ctx->monster_count].x = c;
		ctx->monsters[ctx->monster_count].y = r;
		ctx->monsters[ctx->monster_count].dir_x = 1;
		ctx->monsters[ctx->monster_count].dir_y = 0;
		ctx->monsters[ctx->monster_count].alarm_x = c;
		ctx->monsters[ctx->monster_count++].alarm_y = r;
	}
	else if (ft_isset(ch, ctx->data->obj_ids))
		scan_obj(ctx, ch, r, c);
}

static void	scan_map(t_solver_context *ctx)
{
	int	r;
	int	c;

	r = 0;
	while (r < ctx->data->m_height)
	{
		c = 0;
		while (c < ctx->data->m_max_width && ctx->data->m_map[r][c])
		{
			scan_cell(ctx, r, c);
			c++;
		}
		r++;
	}
}

static int	find_door_idx(t_solver_context *ctx, int x, int y)
{
	int	i;

	i = 0;
	while (i < ctx->door_count)
	{
		if (ctx->doors[i].x == x && ctx->doors[i].y == y)
			return (i);
		i++;
	}
	return (-1);
}

static int	is_walkable_tile(t_solver_context *ctx, int x, int y, uint64_t door_mask)
{
	char	ch;
	int		door_idx;

	if (x < 0 || x >= ctx->data->m_max_width || y < 0 || y >= ctx->data->m_height)
		return (0);
	ch = ctx->data->m_map[y][x];
	if (ch == ' ' || ch == '\0')
		return (0);
	if (ft_isset(ch, ctx->data->wall_ids))
		return (0);
	if (ft_isset(ch, ctx->data->door_ids))
	{
		door_idx = find_door_idx(ctx, x, y);
		if (door_idx >= 0 && (door_mask & (1ULL << door_idx)))
			return (0);
	}
	return (1);
}

static int	monster_spots(t_solver_context *ctx, double pos[2], double dir[2],
				t_map_state *state)
{
	int	rx;
	int	ry;
	int	step;

	rx = (int)pos[0];
	ry = (int)pos[1];
	step = 0;
	while (++step <= 6)
	{
		rx += (int)dir[0];
		ry += (int)dir[1];
		if (rx < 0 || rx >= ctx->data->m_max_width || ry < 0
			|| ry >= ctx->data->m_height)
			break ;
		if (ft_isset(ctx->data->m_map[ry][rx], ctx->data->wall_ids))
			break ;
		if (ft_isset(ctx->data->m_map[ry][rx], ctx->data->door_ids)
			&& (state->door_mask & (1ULL << find_door_idx(ctx, rx, ry))))
			break ;
		if (rx == state->player_x && ry == state->player_y)
			return (1);
	}
	return (0);
}

static void	get_patrolling_monster_pos(t_solver_context *ctx,
				t_solver_monster *m, int steps, double pos[2], double dir[2],
				uint64_t door_mask)
{
	int		i;
	double	nx;
	double	ny;

	pos[0] = m->x + 0.5;
	pos[1] = m->y + 0.5;
	dir[0] = m->dir_x;
	dir[1] = m->dir_y;
	i = -1;
	while (++i < steps)
	{
		nx = pos[0] + dir[0] * 0.4;
		ny = pos[1] + dir[1] * 0.4;
		if (is_walkable_tile(ctx, (int)nx, (int)ny, door_mask))
		{
			pos[0] = nx;
			pos[1] = ny;
		}
		else
		{
			dir[0] = -dir[0];
			dir[1] = -dir[1];
			nx = pos[0] + dir[0] * 0.4;
			ny = pos[1] + dir[1] * 0.4;
			if (is_walkable_tile(ctx, (int)nx, (int)ny, door_mask))
			{
				pos[0] = nx;
				pos[1] = ny;
			}
		}
	}
}

static void	get_mstr_pos(t_map_state *state, t_solver_monster *m,
				double pos[2], double dir[2], t_solver_context *ctx)
{
	int			steps;
	int			dist;
	t_map_state	*tmp;
	int			m_steps;

	if (!(state->monster_mask & (1ULL << 63)))
	{
		get_patrolling_monster_pos(ctx, m, state->steps, pos, dir,
			state->door_mask);
		return ;
	}
	steps = 0;
	tmp = state;
	while (tmp && (tmp->monster_mask & (1ULL << 63)) && (++steps))
		tmp = tmp->parent;
	m_steps = (int)(steps * 0.7);
	dist = abs(m->x - m->alarm_x) + abs(m->y - m->alarm_y);
	dist = (dist <= 0) ? 1 : dist;
	pos[0] = m->x + 0.5 + (double)(m->alarm_x - m->x) * m_steps / dist;
	pos[1] = m->y + 0.5 + (double)(m->alarm_y - m->y) * m_steps / dist;
	dir[0] = (double)(m->alarm_x - m->x) / dist;
	dir[1] = (double)(m->alarm_y - m->y) / dist;
	if (m_steps >= dist)
	{
		pos[0] = m->alarm_x + 0.5;
		pos[1] = m->alarm_y + 0.5;
		dir[0] = 0;
		dir[1] = 0;
	}
}

static int	is_spotted(t_map_state *state, t_solver_context *ctx)
{
	int		i;
	double	pos[2];
	double	dir[2];

	i = -1;
	while (++i < ctx->monster_count)
	{
		if ((state->monster_mask & (1ULL << i)) == 0)
		{
			get_mstr_pos(state, &ctx->monsters[i], pos, dir, ctx);
			if (state->monster_mask & (1ULL << 63))
			{
				if (abs((int)state->player_x - (int)pos[0]) <= 1
					&& abs((int)state->player_y - (int)pos[1]) <= 1)
					return (1);
			}
			else
			{
				if ((int)state->player_x == (int)pos[0]
					&& (int)state->player_y == (int)pos[1])
					return (1);
				if (monster_spots(ctx, pos, dir, state))
					return (1);
			}
		}
	}
	return (0);
}

static t_map_state	*new_state(void)
{
	t_map_state	*new;

	new = malloc(sizeof(t_map_state));
	if (!new)
		return (NULL);
	new->state_index = 0;
	new->player_x = 0;
	new->player_y = 0;
	new->door_mask = 0;
	new->monster_mask = 0;
	new->steps = 0;
	new->parent = NULL;
	return (new);
}

static t_map_state	*create_futur_state(t_map_state *actual, int x, int y)
{
	t_map_state	*futur;

	futur = new_state();
	if (!futur)
		return (NULL);
	futur->player_x = x;
	futur->player_y = y;
	futur->door_mask = actual->door_mask;
	futur->monster_mask = actual->monster_mask;
	futur->steps = actual->steps + 1;
	futur->parent = actual;
	return (futur);
}

static t_map_state	*move_cardinal(t_map_state *actual, int dir, t_solver_context *ctx)
{
	int	nx;
	int	ny;

	nx = actual->player_x;
	ny = actual->player_y;
	if (dir == 1)
		ny -= 1;
	else if (dir == 2)
		nx += 1;
	else if (dir == 3)
		ny += 1;
	else if (dir == 4)
		nx -= 1;
	if (is_walkable_tile(ctx, nx, ny, actual->door_mask))
		return (create_futur_state(actual, nx, ny));
	return (NULL);
}

static t_map_state	*try_activate_alarm(t_map_state *actual, t_solver_context *ctx)
{
	int			i;
	t_map_state	*futur;

	i = -1;
	while (++i < ctx->term_count)
	{
		if (abs(actual->player_x - ctx->terms[i].x)
			+ abs(actual->player_y - ctx->terms[i].y) == 1)
		{
			if ((actual->monster_mask & (1ULL << 63)) == 0)
			{
				futur = create_futur_state(actual, actual->player_x, actual->player_y);
				if (futur)
					futur->monster_mask |= (1ULL << 63);
				return (futur);
			}
		}
	}
	return (NULL);
}

static void	neutralise_monsters(t_map_state *futur, t_solver_context *ctx, int dx, int dy)
{
	int	k;
	int	mx;
	int	my;

	k = 0;
	while (k < ctx->monster_count)
	{
		if (futur->monster_mask & (1ULL << 63))
		{
			mx = ctx->monsters[k].alarm_x;
			my = ctx->monsters[k].alarm_y;
		}
		else
		{
			mx = ctx->monsters[k].x;
			my = ctx->monsters[k].y;
		}
		if (mx == dx && my == dy)
			futur->monster_mask |= (1ULL << k);
		k++;
	}
}

static t_map_state	*try_close_door(t_map_state *actual, t_solver_context *ctx)
{
	int			i;
	t_map_state	*futur;

	i = -1;
	while (++i < ctx->door_count)
	{
		if (abs(actual->player_x - ctx->doors[i].x)
			+ abs(actual->player_y - ctx->doors[i].y) == 1
			&& (actual->door_mask & (1ULL << i)) == 0)
		{
			futur = create_futur_state(actual, actual->player_x, actual->player_y);
			if (futur)
			{
				futur->door_mask |= (1ULL << i);
				neutralise_monsters(futur, ctx, ctx->doors[i].x, ctx->doors[i].y);
			}
			return (futur);
		}
	}
	return (NULL);
}

static t_map_state	*move(t_map_state *actual, int i, t_solver_context *ctx)
{
	if (i >= 1 && i <= 4)
		return (move_cardinal(actual, i, ctx));
	if (i == 5)
		return (try_activate_alarm(actual, ctx));
	if (i == 6)
		return (try_close_door(actual, ctx));
	return (NULL);
}

static int	add_to_queue(t_queue *q, t_map_state *futur)
{
	t_qnode	*new;

	new = malloc(sizeof(t_qnode));
	if (!new)
		return (0);
	new->state = futur;
	new->next = NULL;
	if (q->front == NULL)
	{
		q->front = new;
		q->rear = new;
	}
	else
	{
		q->rear->next = new;
		q->rear = new;
	}
	return (1);
}

static t_map_state	*get_from_queue(t_queue *q)
{
	t_qnode		*temp;
	t_map_state	*state;

	temp = q->front;
	state = temp->state;
	q->front = q->front->next;
	free(temp);
	return (state);
}

static void	free_queue(t_queue *q)
{
	t_qnode	*temp;

	if (!q)
		return ;
	while (q->front)
	{
		temp = q->front;
		q->front = q->front->next;
		free(temp);
	}
	q->rear = NULL;
}

static int	compare_states(t_map_state *a, t_map_state *b)
{
	if (a->player_x != b->player_x)
		return (a->player_x - b->player_x);
	if (a->player_y != b->player_y)
		return (a->player_y - b->player_y);
	if (a->door_mask != b->door_mask)
		return (a->door_mask < b->door_mask ? -1 : 1);
	if (a->monster_mask != b->monster_mask)
		return (a->monster_mask < b->monster_mask ? -1 : 1);
	if ((a->steps % 60) != (b->steps % 60))
		return ((a->steps % 60) - (b->steps % 60));
	return (0);
}

static t_map_state	*bst_search(t_bst *tree, t_map_state *futur)
{
	int	direction;

	while (tree)
	{
		direction = compare_states(tree->state, futur);
		if (!direction)
			return (tree->state);
		if (direction > 0)
			tree = tree->left;
		else
			tree = tree->right;
	}
	return (NULL);
}

static void	free_bst(t_bst *tree)
{
	if (!tree)
		return ;
	if (tree->left)
		free_bst(tree->left);
	if (tree->right)
		free_bst(tree->right);
	if (tree->state)
		free(tree->state);
	free(tree);
}

static int	bst_insert(t_bst *tree, t_map_state *new_state)
{
	t_bst	*node;

	node = malloc(sizeof(t_bst));
	if (!node)
		return (0);
	node->state = new_state;
	node->left = NULL;
	node->right = NULL;
	while (tree)
	{
		if (compare_states(tree->state, new_state) > 0 && !tree->left)
			return (tree->left = node, 1);
		else if (compare_states(tree->state, new_state) > 0)
			tree = tree->left;
		else if (!tree->right)
			return (tree->right = node, 1);
		else
			tree = tree->right;
	}
	return (0);
}

static int	process_futur(t_map_state *actual, t_map_state *futur, t_bfs_var *var)
{
	t_map_state	*existing;

	(void)actual;
	existing = bst_search(var->tree, futur);
	if (!existing)
	{
		futur->state_index = (*var->counter)++;
		if (!bst_insert(var->tree, futur))
			return (0);
		if (!add_to_queue(var->q, futur))
			return (0);
	}
	else
	{
		free(futur);
	}
	return (1);
}

static int	bfs(t_queue *q, t_bst *tree, t_solver_context *ctx)
{
	t_map_state	*act;
	t_map_state	*fut;
	int			i;
	int			cnt;
	t_bfs_var	var;

	cnt = 1;
	var = (t_bfs_var){tree, q, &cnt};
	while (q->front)
	{
		act = get_from_queue(q);
		if (act->player_x == ctx->exit_x && act->player_y == ctx->exit_y)
			return (free_queue(q), 1);
		i = 0;
		while (!is_spotted(act, ctx) && ++i <= 6)
		{
			fut = move(act, i, ctx);
			if (fut && !process_futur(act, fut, &var))
				return (free_queue(q), 0);
		}
	}
	return (free_queue(q), 0);
}

static t_bst	*init_solver(t_data *d, t_queue *q)
{
	t_map_state	*start;
	t_bst		*tree;

	start = new_state();
	if (!start)
		return (NULL);
	start->player_x = d->player_col;
	start->player_y = d->player_row;
	tree = malloc(sizeof(t_bst));
	if (!tree)
		return (free(start), NULL);
	tree->state = start;
	tree->left = NULL;
	tree->right = NULL;
	q->front = NULL;
	q->rear = NULL;
	if (!add_to_queue(q, start))
		return (free_bst(tree), NULL);
	return (tree);
}

#include <unistd.h>

static void	print_solver_loading(void)
{
	int	i;
	int	j;

	printf("\n\033[1;32m[SECURITY] Initiating map solvability validator...\033[0m\n");
	usleep(150000);
	printf("\033[1;36m[SYSTEM] Decompressing world SOA layout & entities...\033[0m\n");
	usleep(150000);
	printf("\033[1;33m[DECRYPTING] Running state-space BFS & BST solver:\033[0m\n");
	i = -1;
	while (++i <= 20)
	{
		printf("\r\033[1;32m[HACKING] [");
		j = -1;
		while (++j < 20)
		{
			if (j < i)
				printf("=");
			else
				printf(".");
		}
		printf("] %d%%\033[0m", i * 5);
		fflush(stdout);
		usleep(25000);
	}
	printf("\n");
}

static void	precompute_closest_alarms(t_solver_context *ctx)
{
	int	m;
	int	closest_idx;
	int	min_dist;
	int	a;
	int	dist;

	m = 0;
	while (m < ctx->monster_count)
	{
		closest_idx = -1;
		min_dist = 1000000;
		a = -1;
		while (++a < ctx->alarm_count)
		{
			dist = abs(ctx->monsters[m].x - ctx->alarms[a].x)
				+ abs(ctx->monsters[m].y - ctx->alarms[a].y);
			if (dist < min_dist)
			{
				min_dist = dist;
				closest_idx = a;
			}
		}
		if (closest_idx != -1)
		{
			ctx->monsters[m].alarm_x = ctx->alarms[closest_idx].x;
			ctx->monsters[m].alarm_y = ctx->alarms[closest_idx].y;
		}
		m++;
	}
}

bool	is_map_solvable(t_data *d)
{
	t_solver_context	ctx;
	t_queue				q;
	t_bst				*tree;
	int					res;

	print_solver_loading();
	ft_bzero(&ctx, sizeof(t_solver_context));
	ctx.data = d;
	find_exit(d, &ctx.exit_x, &ctx.exit_y);
	scan_map(&ctx);
	precompute_closest_alarms(&ctx);
	tree = init_solver(d, &q);
	if (!tree)
		return (false);
	res = bfs(&q, tree, &ctx);
	free_bst(tree);
	return (res != 0);
}
