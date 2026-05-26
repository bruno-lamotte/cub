/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   monsters_ai.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: blamotte <blamotte@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/01 20:55:38 by marvin            #+#    #+#             */
/*   Updated: 2026/05/26 09:46:15 by blamotte         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub.h"

static void	find_next_step(t_monster_rt *m, t_engine *eng, t_vec2_i size,
				double ax, double ay, double *tx, double *ty);
static void	mstr_move_towards(t_monster_rt *m, int self_idx, t_engine *eng,
				t_vec2_i size, double tx, double ty);
static void	assign_new_patrol_target(t_monster_rt *m, int self_idx,
				t_engine *eng);
static int	is_bottleneck_cell(t_engine *eng, int x, int y);
static void	execute_slr_transition(t_engine *eng, t_monster_rt *m,
				char *stim);
static void	execute_slr_transition_by_id(t_engine *eng, t_monster_rt *m,
				int sym_nbr);

static int	is_player_in_light(t_player_rt *p, t_engine *eng)
{
	return (compute_light_at_point(p->pos.x, p->pos.y, NULL, NULL,
			eng->blob, eng) > 0.05f);
}

static int	is_player_in_external_light(t_player_rt *p, t_engine *eng)
{
	int		i;
	t_light	*l;
	double	d2;

	i = -1;
	while (++i < eng->static_light_count)
	{
		l = &eng->static_lights[i];
		d2 = (p->pos.x - l->x) * (p->pos.x - l->x)
			+ (p->pos.y - l->y) * (p->pos.y - l->y);
		if (d2 >= l->radius * l->radius)
			continue ;
		if (l->is_alarm && l->is_triggered
			&& check_los(l->x, l->y, p->pos.x, p->pos.y, eng->blob))
			return (1);
		if (!l->is_alarm && l->is_active
			&& check_los(l->x, l->y, p->pos.x, p->pos.y, eng->blob))
			return (1);
	}
	return (0);
}

static int	check_los_and_fov(t_monster_rt *m, t_player_rt *p,
				double cos_a, void *blob)
{
	return (cos_a >= 0.866 && check_los(m->pos.x, m->pos.y,
			p->pos.x, p->pos.y, blob));
}

static int	detect_player(t_monster_rt *m, t_engine *eng)
{
	double		d[2];
	float		d2;
	double		cos_a;
	t_player_rt	*p;

	p = eng->player;
	d[0] = p->pos.x - m->pos.x;
	d[1] = p->pos.y - m->pos.y;
	d2 = (float)(d[0] * d[0] + d[1] * d[1]);
	if (d2 < 0.01f)
		return (1);
	cos_a = (d[0] * m->dir.x + d[1] * m->dir.y) * fast_inv_sqrt(d2);
	if (is_player_in_external_light(p, eng)
		&& check_los_and_fov(m, p, cos_a, eng->blob))
		return (1);
	if (is_player_in_light(p, eng) ? d2 >= 36.0f : d2 >= 4.0f)
		return (0);
	if (d2 < 1.00f && check_los(m->pos.x, m->pos.y, p->pos.x, p->pos.y,
			eng->blob))
		return (1);
	return (check_los_and_fov(m, p, cos_a, eng->blob));
}

static int	check_monster_collision(double x, double y, int self_idx,
				t_engine *eng)
{
	t_monster_rt	*mstr;
	int				i;
	double			dist2;

	dist2 = (x - eng->player->pos.x) * (x - eng->player->pos.x)
		+ (y - eng->player->pos.y) * (y - eng->player->pos.y);
	if (dist2 < 0.36)
		return (0);
	mstr = get_monster_rt(eng->blob);
	i = -1;
	while (++i < eng->data->monster_rt_count)
	{
		if (i == self_idx)
			continue ;
		if (mstr[i].flags & MONSTER_DEAD)
			continue ;
		dist2 = (x - mstr[i].pos.x) * (x - mstr[i].pos.x)
			+ (y - mstr[i].pos.y) * (y - mstr[i].pos.y);
		if (dist2 < 0.36)
			return (0);
	}
	return (1);
}

static int	check_monster_self_collision(t_monster_rt *self, t_engine *eng)
{
	t_monster_rt	*mstr;
	int				i;
	double			d2;

	mstr = get_monster_rt(eng->blob);
	i = -1;
	while (++i < eng->data->monster_rt_count)
	{
		if (&mstr[i] == self || (mstr[i].flags & MONSTER_DEAD))
			continue ;
		d2 = (self->pos.x - mstr[i].pos.x) * (self->pos.x - mstr[i].pos.x)
			+ (self->pos.y - mstr[i].pos.y) * (self->pos.y - mstr[i].pos.y);
		if (d2 < 0.36)
			return (1);
	}
	return (0);
}

static int	is_bottleneck(int x, int y, int w, int h, void *blob)
{
	uint8_t	*flags;

	flags = get_map_flags(blob);
	if (!is_walkable(x, y, w, h, blob))
		return (0);
	if (flags[y * w + x] & CELL_HAS_DOOR)
		return (1);
	if (!is_walkable(x - 1, y, w, h, blob)
		&& !is_walkable(x + 1, y, w, h, blob)
		&& is_walkable(x, y - 1, w, h, blob)
		&& is_walkable(x, y + 1, w, h, blob))
		return (1);
	if (!is_walkable(x, y - 1, w, h, blob)
		&& !is_walkable(x, y + 1, w, h, blob)
		&& is_walkable(x - 1, y, w, h, blob)
		&& is_walkable(x + 1, y, w, h, blob))
		return (1);
	return (0);
}

typedef struct s_bfs
{
	t_engine	*eng;
	t_vec2_i	size;
	int			*visited;
	int			*queue;
	int			head;
	int			tail;
	int			count;
}	t_bfs;

static void	push_neighbor(t_bfs *b, int x, int y)
{
	int	idx;

	if (x < 0 || x >= b->size.x || y < 0 || y >= b->size.y)
		return ;
	if (!is_walkable(x, y, b->size.x, b->size.y, b->eng->blob))
		return ;
	idx = y * b->size.x + x;
	if (!b->visited[idx])
	{
		b->visited[idx] = 1;
		b->queue[b->tail++] = idx;
	}
}

static int	init_bfs(t_bfs *b, t_engine *eng, t_vec2_i start)
{
	b->eng = eng;
	b->size.x = get_map_width(get_blob_hdr(eng->blob));
	b->size.y = get_map_height(get_blob_hdr(eng->blob));
	b->visited = malloc(sizeof(int) * b->size.x * b->size.y);
	b->queue = malloc(sizeof(int) * b->size.x * b->size.y);
	if (!b->visited || !b->queue)
	{
		free(b->visited);
		free(b->queue);
		return (0);
	}
	ft_bzero(b->visited, sizeof(int) * b->size.x * b->size.y);
	b->head = 0;
	b->tail = 0;
	b->count = 0;
	b->visited[start.y * b->size.x + start.x] = 1;
	b->queue[b->tail++] = start.y * b->size.x + start.x;
	return (1);
}

static int	get_reachable_cells(t_engine *eng, t_vec2_i start,
				t_vec2_i *cells, int max_cells)
{
	t_bfs	b;
	int		curr;
	int		cx;
	int		cy;

	if (!init_bfs(&b, eng, start))
		return (0);
	while (b.head < b.tail && b.count < max_cells)
	{
		curr = b.queue[b.head++];
		cx = curr % b.size.x;
		cy = curr / b.size.x;
		cells[b.count].x = cx;
		cells[b.count].y = cy;
		b.count++;
		push_neighbor(&b, cx + 1, cy);
		push_neighbor(&b, cx - 1, cy);
		push_neighbor(&b, cx, cy + 1);
		push_neighbor(&b, cx, cy - 1);
	}
	free(b.visited);
	free(b.queue);
	return (b.count);
}

static void	init_monster_targets(t_engine *eng)
{
	t_monster_rt	*mstr;
	int				i;

	mstr = get_monster_rt(eng->blob);
	i = -1;
	while (++i < eng->data->monster_rt_count)
	{
		if (!(mstr[i].flags & MONSTER_DEAD))
		{
			mstr[i].state_stack[0] = 0;
			mstr[i].state_stack_top = 1;
			mstr[i].state = MSTR_STATE_PATROL;
			mstr[i].dir.x = 1.0;
			mstr[i].dir.y = 0.0;
			assign_new_patrol_target(&mstr[i], i, eng);
		}
	}
}

static void	mstr_go_to_guard(t_monster_rt *m, int self_idx, t_engine *eng,
				t_vec2_i size)
{
	double	dx;
	double	dy;
	double	tx;
	double	ty;
	double	old;

	dx = m->guard_x - m->pos.x;
	dy = m->guard_y - m->pos.y;
	if (dx * dx + dy * dy < 0.02)
	{
		m->pos.x = m->guard_x;
		m->pos.y = m->guard_y;
		old = m->dir.x;
		m->dir.x = m->dir.x * cos(0.04) - m->dir.y * sin(0.04);
		m->dir.y = old * sin(0.04) + m->dir.y * cos(0.04);
		return ;
	}
	if (m->alert_timer > 300)
	{
		assign_new_patrol_target(m, self_idx, eng);
		m->alert_timer = 0;
		return ;
	}
	find_next_step(m, eng, size, m->guard_x, m->guard_y, &tx, &ty);
	mstr_move_towards(m, self_idx, eng, size, tx, ty);
}

static void	mstr_chase(t_monster_rt *m, int self_idx, t_engine *eng,
				t_vec2_i size)
{
	double	dx;
	double	dy;
	float	len2;

	dx = eng->player->pos.x - m->pos.x;
	dy = eng->player->pos.y - m->pos.y;
	len2 = (float)(dx * dx + dy * dy);
	if (len2 > 0.01f)
	{
		m->dir.x = dx * fast_inv_sqrt(len2);
		m->dir.y = dy * fast_inv_sqrt(len2);
	}
	dx = m->pos.x + m->dir.x * 0.035;
	dy = m->pos.y + m->dir.y * 0.035;
	if (is_valid_position(dx, m->pos.y, size.x, size.y, eng->blob)
		&& check_monster_collision(dx, m->pos.y, self_idx, eng))
		m->pos.x = dx;
	if (is_valid_position(m->pos.x, dy, size.x, size.y, eng->blob)
		&& check_monster_collision(m->pos.x, dy, self_idx, eng))
		m->pos.y = dy;
}

static void	find_closest_alarm(t_monster_rt *m, t_engine *eng,
				double *ax, double *ay)
{
	int		i;
	double	closest;
	double	dx;
	double	dy;

	*ax = -1.0;
	*ay = -1.0;
	closest = 1e9;
	i = -1;
	while (++i < eng->static_light_count)
	{
		if (eng->static_lights[i].is_alarm
			&& eng->static_lights[i].is_triggered)
		{
			dx = eng->static_lights[i].x - m->pos.x;
			dy = eng->static_lights[i].y - m->pos.y;
			if (dx * dx + dy * dy < closest)
			{
				closest = dx * dx + dy * dy;
				*ax = eng->static_lights[i].x;
				*ay = eng->static_lights[i].y;
			}
		}
	}
}

static void	init_bfs_arrays(int *parent, int size)
{
	int	i;

	i = 0;
	while (i < size)
	{
		parent[i] = -1;
		i++;
	}
}

static void	bfs_check_neighbor(int nx, int ny, int curr, t_engine *eng,
				int *parent, int *queue, int *tail, t_vec2_i size)
{
	int	idx;

	if (nx < 0 || nx >= size.x || ny < 0 || ny >= size.y)
		return ;
	if (!is_walkable(nx, ny, size.x, size.y, eng->blob))
		return ;
	idx = ny * size.x + nx;
	if (parent[idx] == -1)
	{
		parent[idx] = curr;
		queue[*tail] = idx;
		(*tail)++;
	}
}

static void	bfs_expand_neighbors(int curr, t_engine *eng, int *parent,
				int *queue, int *tail, t_vec2_i size)
{
	int	cx;
	int	cy;

	cx = curr % size.x;
	cy = curr / size.x;
	bfs_check_neighbor(cx + 1, cy, curr, eng, parent, queue, tail, size);
	bfs_check_neighbor(cx - 1, cy, curr, eng, parent, queue, tail, size);
	bfs_check_neighbor(cx, cy + 1, curr, eng, parent, queue, tail, size);
	bfs_check_neighbor(cx, cy - 1, curr, eng, parent, queue, tail, size);
}

static int	bfs_run(t_vec2_i start, t_vec2_i target, t_engine *eng,
				t_vec2_i size, int *parent, int *queue)
{
	int	head;
	int	tail;
	int	curr;
	int	target_idx;

	head = 0;
	tail = 0;
	target_idx = target.y * size.x + target.x;
	queue[tail++] = start.y * size.x + start.x;
	parent[start.y * size.x + start.x] = -2;
	while (head < tail)
	{
		curr = queue[head++];
		if (curr == target_idx)
			return (1);
		bfs_expand_neighbors(curr, eng, parent, queue, &tail, size);
	}
	return (0);
}

static int	bfs_backtrack(int start_idx, int target_idx, int *parent)
{
	int	curr;

	curr = target_idx;
	while (parent[curr] != -2 && parent[curr] != -1)
	{
		if (parent[curr] == start_idx)
			return (curr);
		curr = parent[curr];
	}
	return (curr);
}

static void	assign_coords(double *tx, double *ty, double ax, double ay)
{
	*tx = ax;
	*ty = ay;
}

static void	get_backtrack_coords(t_vec2_i start, t_vec2_i target,
				t_vec2_i size, int *parent, double *tx, double *ty)
{
	int	next_idx;

	next_idx = bfs_backtrack(start.y * size.x + start.x,
			target.y * size.x + target.x, parent);
	*tx = (next_idx % size.x) + 0.5;
	*ty = (next_idx / size.x) + 0.5;
}

static void	find_next_step(t_monster_rt *m, t_engine *eng, t_vec2_i size,
				double ax, double ay, double *tx, double *ty)
{
	int			*parent;
	int			*queue;
	t_vec2_i	start;
	t_vec2_i	target;

	start.x = (int)m->pos.x;
	start.y = (int)m->pos.y;
	target.x = (int)ax;
	target.y = (int)ay;
	parent = malloc(sizeof(int) * size.x * size.y);
	queue = malloc(sizeof(int) * size.x * size.y);
	if (!parent || !queue)
	{
		free(parent);
		free(queue);
		assign_coords(tx, ty, ax, ay);
		return ;
	}
	init_bfs_arrays(parent, size.x * size.y);
	if (bfs_run(start, target, eng, size, parent, queue))
		get_backtrack_coords(start, target, size, parent, tx, ty);
	else
		assign_coords(tx, ty, ax, ay);
	free(parent);
	free(queue);
}

static void	mstr_move_towards(t_monster_rt *m, int self_idx, t_engine *eng,
				t_vec2_i size, double tx, double ty)
{
	double	dx;
	double	dy;
	float	len2;

	dx = tx - m->pos.x;
	dy = ty - m->pos.y;
	len2 = (float)(dx * dx + dy * dy);
	if (len2 > 0.01f)
	{
		m->dir.x = dx * fast_inv_sqrt(len2);
		m->dir.y = dy * fast_inv_sqrt(len2);
	}
	dx = m->pos.x + m->dir.x * 0.035;
	dy = m->pos.y + m->dir.y * 0.035;
	if (is_valid_position(dx, m->pos.y, size.x, size.y, eng->blob)
		&& check_monster_collision(dx, m->pos.y, self_idx, eng))
		m->pos.x = dx;
	if (is_valid_position(m->pos.x, dy, size.x, size.y, eng->blob)
		&& check_monster_collision(m->pos.x, dy, self_idx, eng))
		m->pos.y = dy;
}

static void	mstr_go_to_alarm(t_monster_rt *m, int self_idx, t_engine *eng,
				t_vec2_i size, double ax, double ay)
{
	double	tx;
	double	ty;

	find_next_step(m, eng, size, ax, ay, &tx, &ty);
	mstr_move_towards(m, self_idx, eng, size, tx, ty);
}

static int	is_cell_walkable(t_engine *eng, int x, int y)
{
	return (is_walkable(x, y,
			get_map_width(get_blob_hdr(eng->blob)),
			get_map_height(get_blob_hdr(eng->blob)),
			eng->blob));
}

static int	is_bottleneck_cell(t_engine *eng, int x, int y)
{
	return (is_bottleneck(x, y,
			get_map_width(get_blob_hdr(eng->blob)),
			get_map_height(get_blob_hdr(eng->blob)),
			eng->blob));
}

static int	is_cell_assigned(t_engine *eng, t_monster_rt *self, t_vec2_i cell)
{
	t_monster_rt	*mstr;
	int				i;

	mstr = get_monster_rt(eng->blob);
	i = -1;
	while (++i < eng->data->monster_rt_count)
	{
		if (&mstr[i] == self || (mstr[i].flags & MONSTER_DEAD))
			continue ;
		if (mstr[i].has_guard_pos && (int)mstr[i].guard_x == cell.x
			&& (int)mstr[i].guard_y == cell.y)
			return (1);
	}
	return (0);
}

static int	score_patrol_cell(t_engine *eng, t_monster_rt *m, t_vec2_i cell)
{
	int		neighbors;
	double	dx;
	double	dy;
	double	dist;
	int		score;

	if (is_bottleneck_cell(eng, cell.x, cell.y))
		return (2);
	dx = cell.x + 0.5 - m->pos.x;
	dy = cell.y + 0.5 - m->pos.y;
	dist = sqrt(dx * dx + dy * dy);
	if (dist < 2.0 || dist > 10.0)
		return (3);
	neighbors = is_cell_walkable(eng, cell.x + 1, cell.y)
		+ is_cell_walkable(eng, cell.x - 1, cell.y)
		+ is_cell_walkable(eng, cell.x, cell.y + 1)
		+ is_cell_walkable(eng, cell.x, cell.y - 1);
	score = 10 + neighbors * 5;
	if (dist <= 8.0)
		score += 5;
	return (score);
}

static int	select_patrol_cell(t_engine *eng, t_monster_rt *m,
				t_vec2_i *cells, int total)
{
	int	best_score;
	int	best_idx;
	int	i;
	int	idx;
	int	seed;

	best_score = -1;
	best_idx = -1;
	seed = eng->pool.current_frame + (int)(m->pos.x * 17 + m->pos.y * 31);
	i = -1;
	while (++i < total)
	{
		idx = (seed + i) % (total ? total : 1);
		if (cells[idx].x == (int)m->pos.x && cells[idx].y == (int)m->pos.y)
			continue ;
		if (!is_cell_assigned(eng, m, cells[idx]))
		{
			if (score_patrol_cell(eng, m, cells[idx]) > best_score)
			{
				best_score = score_patrol_cell(eng, m, cells[idx]);
				best_idx = idx;
			}
		}
	}
	return (best_idx);
}

static int	find_close_bottleneck(t_engine *eng, t_monster_rt *m,
				t_vec2_i *cells, int total)
{
	int		i;
	double	dx;
	double	dy;
	double	min_dist;
	int		best_idx;

	i = -1;
	min_dist = 1e30;
	best_idx = -1;
	while (++i < total)
	{
		if (cells[i].x == (int)m->pos.x && cells[i].y == (int)m->pos.y)
			continue ;
		if (is_bottleneck_cell(eng, cells[i].x, cells[i].y)
			&& !is_cell_assigned(eng, m, cells[i]))
		{
			dx = cells[i].x + 0.5 - m->pos.x;
			dy = cells[i].y + 0.5 - m->pos.y;
			if (dx * dx + dy * dy < min_dist)
			{
				min_dist = dx * dx + dy * dy;
				best_idx = i;
			}
		}
	}
	if (min_dist <= 16.0)
		return (best_idx);
	return (-1);
}

static void	assign_new_patrol_target(t_monster_rt *m, int self_idx,
				t_engine *eng)
{
	t_vec2_i	*cells;
	int			tot;
	int			idx;
	int			ws;

	(void)self_idx;
	ws = m->has_guard_pos
		&& is_bottleneck_cell(eng, (int)m->guard_x, (int)m->guard_y);
	cells = malloc(sizeof(t_vec2_i) * 1024);
	if (!cells)
		return ;
	tot = get_reachable_cells(eng,
			(t_vec2_i){(int)m->pos.x, (int)m->pos.y}, cells, 1024);
	idx = -1;
	if (!m->has_guard_pos || ws)
		idx = find_close_bottleneck(eng, m, cells, tot);
	if (idx == -1)
		idx = select_patrol_cell(eng, m, cells, tot);
	if (idx != -1)
	{
		m->guard_x = cells[idx].x + 0.5;
		m->guard_y = cells[idx].y + 0.5;
		m->has_guard_pos = 1;
	}
	free(cells);
}

static int	state_name_to_id(char *name)
{
	if (ft_strcmp(name, "patrol_state") == 0
		|| ft_strcmp(name, "patrol_loop") == 0
		|| ft_strcmp(name, "patrol_action") == 0)
		return (MSTR_STATE_PATROL);
	if (ft_strcmp(name, "chase_state") == 0
		|| ft_strcmp(name, "chase_session") == 0
		|| ft_strcmp(name, "chase_loop") == 0
		|| ft_strcmp(name, "chase_action") == 0
		|| ft_strcmp(name, "ACT_SET_CHASE") == 0)
		return (MSTR_STATE_CHASE);
	if (ft_strcmp(name, "attack_state") == 0
		|| ft_strcmp(name, "ACT_SET_ATTACK") == 0)
		return (MSTR_STATE_ATTACK);
	if (ft_strcmp(name, "scan_state") == 0
		|| ft_strcmp(name, "ACT_SET_SCAN") == 0)
		return (MSTR_STATE_SCAN);
	if (ft_strcmp(name, "reloc_state") == 0
		|| ft_strcmp(name, "ACT_SET_RELOC") == 0)
		return (MSTR_STATE_RELOC);
	if (ft_strcmp(name, "alarm_subroutine") == 0
		|| ft_strcmp(name, "alarm_session") == 0
		|| ft_strcmp(name, "alarm_loop") == 0
		|| ft_strcmp(name, "alarm_action") == 0)
		return (MSTR_STATE_ALARM);
	return (-1);
}

static int	get_sym_state(t_engine *eng, t_list *item_node)
{
	t_symbol	*next_sym;

	while (item_node)
	{
		next_sym = get_symbol_after_dot(eng->slr,
				(t_item *)item_node->content);
		if (next_sym && state_name_to_id(next_sym->name) != -1)
			return (state_name_to_id(next_sym->name));
		item_node = item_node->next;
	}
	return (-1);
}

static int	get_lhs_state(t_list *item_node)
{
	int	id;

	while (item_node)
	{
		id = state_name_to_id(((t_item *)item_node->content)
				->rule_of_item->left_symbol);
		if (id != -1)
			return (id);
		item_node = item_node->next;
	}
	return (-1);
}

static int	get_fsm_state_from_slr_state(t_engine *eng, int slr_state_id)
{
	t_list	*state_node;
	t_state	*s;
	int		id;

	state_node = eng->slr->states;
	while (state_node)
	{
		s = (t_state *)state_node->content;
		if (s->id == slr_state_id)
		{
			id = get_sym_state(eng, s->items);
			if (id != -1)
				return (id);
			id = get_lhs_state(s->items);
			if (id != -1)
				return (id);
		}
		state_node = state_node->next;
	}
	return (MSTR_STATE_PATROL);
}

static void	reset_stack(t_monster_rt *m)
{
	m->state_stack[0] = 0;
	m->state_stack_top = 1;
	m->state = MSTR_STATE_PATROL;
}

static void	handle_slr_reduction(t_engine *eng, t_monster_rt *m, int rule_id)
{
	t_list		*r_node;
	t_symbol	*left;
	int			curr;
	int			val;

	r_node = eng->slr->rules;
	while (r_node)
	{
		if (((t_rule *)r_node->content)->id == rule_id)
		{
			m->state_stack_top -= ((t_rule *)r_node->content)->nb_items;
			if (m->state_stack_top < 1)
				m->state_stack_top = 1;
			curr = m->state_stack[m->state_stack_top - 1];
			left = get_symbol_from_name(eng->slr,
					((t_rule *)r_node->content)->left_symbol);
			if (left && eng->slr->table[curr][left->nbr] > 0)
			{
				val = eng->slr->table[curr][left->nbr];
				m->state_stack[m->state_stack_top++] = val;
				m->state = get_fsm_state_from_slr_state(eng, val);
			}
			return ;
		}
		r_node = r_node->next;
	}
}

static void	apply_slr_shift(t_engine *eng, t_monster_rt *m, int action)
{
	if (m->state_stack_top < 32)
	{
		m->state_stack[m->state_stack_top++] = action;
		m->state = get_fsm_state_from_slr_state(eng, action);
	}
}

static void	execute_slr_transition_by_id(t_engine *eng, t_monster_rt *m,
				int sym_nbr)
{
	int	action;

	while (1)
	{
		if (m->state_stack_top < 1 || m->state_stack_top > 32)
			reset_stack(m);
		action = eng->slr->table[m->state_stack[m->state_stack_top - 1]][sym_nbr];
		if (action == ACCEPTED)
		{
			reset_stack(m);
			break ;
		}
		if (action == 0)
			break ;
		if (action > 0)
		{
			apply_slr_shift(eng, m, action);
			break ;
		}
		handle_slr_reduction(eng, m, -action);
	}
}

static void	execute_slr_transition(t_engine *eng, t_monster_rt *m,
				char *stim)
{
	t_symbol	*sym;

	sym = get_symbol_from_name(eng->slr, stim);
	if (!sym)
		return ;
	execute_slr_transition_by_id(eng, m, sym->nbr);
}

static int	is_dest_reached(t_monster_rt *m)
{
	double	dx;
	double	dy;

	dx = m->guard_x - m->pos.x;
	dy = m->guard_y - m->pos.y;
	return (dx * dx + dy * dy < 0.02);
}

static void	patrol_transitions(t_monster_rt *m, t_engine *eng, int spotted)
{
	if (spotted)
		execute_slr_transition(eng, m, "STIM_LOS_TRUE");
	else if (check_monster_self_collision(m, eng))
		execute_slr_transition(eng, m, "STIM_COLLISION");
	else if (is_dest_reached(m))
	{
		if (is_bottleneck_cell(eng, (int)m->guard_x, (int)m->guard_y))
		{
			if (m->alert_timer > 180)
				execute_slr_transition(eng, m, "STIM_STATION_TIMEOUT");
		}
		else if (m->alert_timer > 120)
			execute_slr_transition(eng, m, "STIM_ALERT_TIMEOUT");
	}
}

static void	chase_transitions(t_monster_rt *m, t_engine *eng, double d2,
				int spotted)
{
	if (d2 < 0.81)
		execute_slr_transition(eng, m, "ACT_MELEE_STRIKE");
	else if (!spotted && m->alert_timer > 30)
		execute_slr_transition(eng, m, "STIM_LOS_FALSE");
}

static void	scan_transitions(t_monster_rt *m, t_engine *eng, int spotted)
{
	if (spotted)
		execute_slr_transition(eng, m, "STIM_LOS_TRUE");
	else if (m->alert_timer > 120)
		execute_slr_transition(eng, m, "STIM_ALERT_TIMEOUT");
}

static void	bind_behavior_to_state(t_slr1 *slr, int state_id, t_behavior_fn func)
{
	slr->state_behaviors[state_id] = func;
}

void	mstr_patrol_behavior(t_monster_rt *m, t_engine *eng)
{
	t_vec2_i	size;
	int			self_idx;

	size.x = get_map_width(get_blob_hdr(eng->blob));
	size.y = get_map_height(get_blob_hdr(eng->blob));
	self_idx = m - get_monster_rt(eng->blob);
	mstr_go_to_guard(m, self_idx, eng, (m->alert_timer++, size));
}

void	mstr_chase_behavior(t_monster_rt *m, t_engine *eng)
{
	t_vec2_i	size;
	int			self_idx;

	size.x = get_map_width(get_blob_hdr(eng->blob));
	size.y = get_map_height(get_blob_hdr(eng->blob));
	self_idx = m - get_monster_rt(eng->blob);
	if (!detect_player(m, eng))
		m->alert_timer++;
	mstr_chase(m, self_idx, eng, size);
}

void	mstr_scan_behavior(t_monster_rt *m, t_engine *eng)
{
	double	old;

	(void)eng;
	m->alert_timer++;
	old = m->dir.x;
	m->dir.x = m->dir.x * cos(0.04) - m->dir.y * sin(0.04);
	m->dir.y = old * sin(0.04) + m->dir.y * cos(0.04);
}

void	mstr_reloc_behavior(t_monster_rt *m, t_engine *eng)
{
	int	self_idx;

	self_idx = m - get_monster_rt(eng->blob);
	assign_new_patrol_target(m, self_idx, eng);
	m->alert_timer = 0;
	execute_slr_transition(eng, m, "ACT_RECALCULATE_PATROL");
}

void	mstr_alarm_behavior(t_monster_rt *m, t_engine *eng)
{
	t_vec2_i	size;
	int			self_idx;
	double		ax;
	double		ay;

	size.x = get_map_width(get_blob_hdr(eng->blob));
	size.y = get_map_height(get_blob_hdr(eng->blob));
	self_idx = m - get_monster_rt(eng->blob);
	find_closest_alarm(m, eng, &ax, &ay);
	if (ax >= 0.0)
		mstr_go_to_alarm(m, self_idx, eng, size, ax, ay);
}

static void	run_grammar_transitions(t_monster_rt *m, t_engine *eng,
				double d2, int spotted)
{
	if (m->state == MSTR_STATE_PATROL)
		patrol_transitions(m, eng, spotted);
	else if (m->state == MSTR_STATE_CHASE)
		chase_transitions(m, eng, d2, spotted);
	else if (m->state == MSTR_STATE_ATTACK && d2 > 1.44)
		execute_slr_transition(eng, m, "ACT_CHASE_MOVE");
	else if (m->state == MSTR_STATE_SCAN)
		scan_transitions(m, eng, spotted);
	else if (m->state == MSTR_STATE_ALARM && spotted)
		execute_slr_transition(eng, m, "STIM_LOS_TRUE");
}

static void	bind_state_behavior(t_engine *eng, t_state *s)
{
	int	fsm_state;

	fsm_state = get_fsm_state_from_slr_state(eng, s->id);
	if (fsm_state == MSTR_STATE_PATROL)
		bind_behavior_to_state(eng->slr, s->id, &mstr_patrol_behavior);
	else if (fsm_state == MSTR_STATE_CHASE)
		bind_behavior_to_state(eng->slr, s->id, &mstr_chase_behavior);
	else if (fsm_state == MSTR_STATE_SCAN)
		bind_behavior_to_state(eng->slr, s->id, &mstr_scan_behavior);
	else if (fsm_state == MSTR_STATE_RELOC)
		bind_behavior_to_state(eng->slr, s->id, &mstr_reloc_behavior);
	else if (fsm_state == MSTR_STATE_ALARM)
		bind_behavior_to_state(eng->slr, s->id, &mstr_alarm_behavior);
}

static void	resolve_auto_act_nbrs(t_slr1 *slr)
{
	static char	*acts[AUTO_ACTS_COUNT] = {
		"ACT_START_CHASE", "ACT_RESUME_PATROL",
		"ACT_GO_TO_ALARM", "ACT_RESUME_PREVIOUS",
		"ACT_SET_RELOC", "ACT_SET_ATTACK", "ACT_SET_CHASE",
		"ACT_MELEE_STRIKE", "ACT_CHASE_MOVE", "ACT_RECALCULATE_PATROL"
	};
	int			i;
	t_symbol	*sym;

	i = 0;
	while (i < AUTO_ACTS_COUNT)
	{
		sym = get_symbol_from_name(slr, acts[i]);
		if (sym)
			slr->auto_act_nbrs[i] = sym->nbr;
		else
			slr->auto_act_nbrs[i] = -1;
		i++;
	}
	sym = get_symbol_from_name(slr, "STIM_ALARM_HEARD");
	slr->stim_alarm_heard_nbr = sym ? sym->nbr : -1;
	sym = get_symbol_from_name(slr, "STIM_ALARM_OFF");
	slr->stim_alarm_off_nbr = sym ? sym->nbr : -1;
}

void	init_all_ai_behaviors(t_slr1 *slr, t_engine *eng)
{
	t_list	*state_node;
	int		nb_states;

	resolve_auto_act_nbrs(slr);
	nb_states = ft_lstsize(slr->states);
	slr->state_behaviors = malloc(sizeof(t_behavior_fn) * nb_states);
	if (!slr->state_behaviors)
		return ;
	ft_bzero(slr->state_behaviors, sizeof(t_behavior_fn) * nb_states);
	state_node = slr->states;
	while (state_node)
	{
		bind_state_behavior(eng, (t_state *)state_node->content);
		state_node = state_node->next;
	}
}

static void	handle_auto_actions(t_engine *eng, t_monster_rt *m)
{
	int	curr;
	int	i;
	int	act_nbr;
	int	action;

	i = -1;
	while (++i < AUTO_ACTS_COUNT)
	{
		act_nbr = eng->slr->auto_act_nbrs[i];
		if (act_nbr >= 0)
		{
			curr = m->state_stack[m->state_stack_top - 1];
			action = eng->slr->table[curr][act_nbr];
			if (action != 0 && action != ACCEPTED)
				execute_slr_transition_by_id(eng, m, act_nbr);
		}
	}
}

static void	update_single_monster(t_monster_rt *m, int self_idx,
				t_engine *eng)
{
	double			d2;
	int				is_in_alarm;
	t_behavior_fn	behavior;

	(void)self_idx;
	d2 = (eng->player->pos.x - m->pos.x) * (eng->player->pos.x - m->pos.x)
		+ (eng->player->pos.y - m->pos.y) * (eng->player->pos.y - m->pos.y);
	is_in_alarm = (m->state == MSTR_STATE_ALARM);
	if (eng->alarm_triggered && !is_in_alarm)
		execute_slr_transition_by_id(eng, m, eng->slr->stim_alarm_heard_nbr);
	else if (!eng->alarm_triggered && is_in_alarm)
		execute_slr_transition_by_id(eng, m, eng->slr->stim_alarm_off_nbr);
	handle_auto_actions(eng, m);
	run_grammar_transitions(m, eng, d2, detect_player(m, eng));
	behavior = eng->slr->state_behaviors[m->state_stack[m->state_stack_top - 1]];
	if (behavior)
		behavior(m, eng);
}

static void	print_monsters_debug(t_monster_rt *mstr, int count)
{
	int	k;

	k = -1;
	while (++k < count)
	{
		printf("Monster %d: pos=(%.2f, %.2f), guard=(%.2f, %.2f), ",
			k, mstr[k].pos.x, mstr[k].pos.y, mstr[k].guard_x, mstr[k].guard_y);
		printf("state=%d, stack_top=%d, timer=%d\n",
			mstr[k].state, mstr[k].state_stack_top, mstr[k].alert_timer);
	}
}

void	update_monsters(t_engine *eng)
{
	t_monster_rt	*mstr;
	int				i;
	static int		frame = 0;

	if (eng->data->monster_rt_count <= 0)
		return ;
	mstr = get_monster_rt(eng->blob);
	if (!mstr[0].has_guard_pos)
		init_monster_targets(eng);
	i = -1;
	while (++i < eng->data->monster_rt_count)
	{
		if (!(mstr[i].flags & MONSTER_DEAD))
			update_single_monster(&mstr[i], i, eng);
	}
	frame++;
	if (frame % 60 == 0)
		print_monsters_debug(mstr, eng->data->monster_rt_count);
}
