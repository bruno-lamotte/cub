#include "cub.h"
#include <math.h>

static int	is_cell_assigned(t_engine *eng, t_monster_rt *self, t_vec2 cell)
{
	t_monster_rt	*mstr;
	int				i;

	mstr = get_monster_rt(eng->blob);
	i = -1;
	while (++i < eng->data->monster_rt_count)
	{
		if (&mstr[i] == self || (mstr[i].flags & MONSTER_DEAD))
			continue ;
		if (mstr[i].has_guard_pos && (int)mstr[i].guard.d.x == cell.i.x
			&& (int)mstr[i].guard.d.y == cell.i.y)
			return (1);
	}
	return (0);
}

static int	find_close_bottleneck(t_engine *eng, t_monster_rt *m,
				t_vec2 *cells, int total)
{
	int		i;
	t_vec2	d;
	double	min_dist;
	int		best_idx;

	i = -1;
	min_dist = 1e30;
	best_idx = -1;
	while (++i < total)
	{
		if (cells[i].i.x == (int)m->pos.d.x && cells[i].i.y == (int)m->pos.d.y)
			continue ;
		d.d.x = cells[i].i.x + 0.5 - m->pos.d.x;
		d.d.y = cells[i].i.y + 0.5 - m->pos.d.y;
		if (is_bottleneck_cell(eng, cells[i].i.x, cells[i].i.y)
			&& !is_cell_assigned(eng, m, cells[i])
			&& d.d.x * d.d.x + d.d.y * d.d.y < min_dist)
		{
			min_dist = d.d.x * d.d.x + d.d.y * d.d.y;
			best_idx = i;
		}
	}
	if (min_dist <= 16.0)
		return (best_idx);
	return (-1);
}

void	assign_new_patrol_target(t_monster_rt *m, t_engine *eng, t_worker *w)
{
	t_vec2		cells[1024];
	int			tot;
	int			idx;
	int			ws;

	ws = m->has_guard_pos
		&& is_bottleneck_cell(eng, (int)m->guard.d.x, (int)m->guard.d.y);
	tot = get_reachable_cells(eng, (t_vec2){.i = {(int)m->pos.d.x,
				(int)m->pos.d.y}}, cells, w);
	idx = -1;
	if (!m->has_guard_pos || ws)
		idx = find_close_bottleneck(eng, m, cells, tot);
	if (idx == -1)
		idx = select_patrol_cell(eng, m, cells, tot);
	if (idx != -1)
	{
		m->guard.d.x = cells[idx].i.x + 0.5;
		m->guard.d.y = cells[idx].i.y + 0.5;
		m->has_guard_pos = 1;
	}
}

void	init_monster_targets(t_engine *eng)
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
			mstr[i].dir.d.x = 1.0;
			mstr[i].dir.d.y = 0.0;
			assign_new_patrol_target(&mstr[i], eng, NULL);
		}
	}
}
