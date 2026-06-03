#include "cub.h"
#include <math.h>

static int	is_cell_walkable(t_engine *eng, int x, int y)
{
	return (is_walkable_for_routing(x, y, eng->blob));
}

int	is_bottleneck_cell(t_engine *eng, int x, int y)
{
	return (is_bottleneck(x, y, eng->blob));
}

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

int	score_patrol_cell(t_engine *eng, t_monster_rt *m, t_vec2 cell)
{
	int		neighbors;
	t_vec2	d;
	double	dist;
	int		score;

	if (is_bottleneck_cell(eng, cell.i.x, cell.i.y))
		return (MSTR_SCORE_BOTTLENECK);
	d.d.x = cell.i.x + 0.5 - m->pos.d.x;
	d.d.y = cell.i.y + 0.5 - m->pos.d.y;
	dist = sqrt(d.d.x * d.d.x + d.d.y * d.d.y);
	if (dist < MSTR_PATROL_MIN_DIST || dist > MSTR_PATROL_MAX_DIST)
		return (MSTR_SCORE_BAD_DIST);
	neighbors = is_cell_walkable(eng, cell.i.x + 1, cell.i.y)
		+ is_cell_walkable(eng, cell.i.x - 1, cell.i.y)
		+ is_cell_walkable(eng, cell.i.x, cell.i.y + 1)
		+ is_cell_walkable(eng, cell.i.x, cell.i.y - 1);
	score = MSTR_SCORE_BASE + neighbors * MSTR_SCORE_NEIGHBOR_MULT;
	if (dist <= MSTR_PATROL_CLOSE_LIMIT)
		score += MSTR_SCORE_CLOSE_BONUS;
	return (score);
}

int	select_patrol_cell(t_engine *eng, t_monster_rt *m,
				t_vec2 *cells, int total)
{
	int	best[2];
	int	i;
	int	idx;
	int	divisor;

	best[0] = -1;
	best[1] = -1;
	divisor = total;
	if (divisor <= 0)
		divisor = 1;
	i = -1;
	while (++i < total)
	{
		idx = (eng->pool.current_frame + (int)(m->pos.d.x * MSTR_PATROL_SEED_X + m->pos.d.y * MSTR_PATROL_SEED_Y) + i) % divisor;
		if (cells[idx].i.x == (int)m->pos.d.x && cells[idx].i.y == (int)m->pos.d.y)
			continue ;
		if (!is_cell_assigned(eng, m, cells[idx])
			&& score_patrol_cell(eng, m, cells[idx]) > best[0])
		{
			best[0] = score_patrol_cell(eng, m, cells[idx]);
			best[1] = idx;
		}
	}
	return (best[1]);
}
