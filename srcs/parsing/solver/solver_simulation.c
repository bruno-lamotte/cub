#include "cub.h"
#include <stdlib.h>

static void	simulate_monsters(t_engine *eng, t_worker *w)
{
	t_monster_rt	*mstr;
	int				i;

	mstr = get_monster_rt(eng->blob);
	i = 0;
	while (i < eng->data->monster_rt_count)
	{
		if (!(mstr[i].flags & MONSTER_DEAD))
			update_single_monster(&mstr[i], eng, w);
		i++;
	}
}

void	simulate_engine_tick(t_engine *eng, t_worker *w)
{
	t_monster_rt	*mstr;

	mstr = get_monster_rt(eng->blob);
	if (eng->data->monster_rt_count > 0 && !mstr[0].has_guard_pos)
		init_monster_targets(eng);
	update_doors(eng);
	update_interaction(eng);
	if (eng->data->monster_rt_count > 0)
		simulate_monsters(eng, w);
	update_alarm_disarm(eng);
}

static int	wait_for_monster(t_engine *eng, t_worker *worker, t_vec2 cell)
{
	int	wait_ticks;

	wait_ticks = 0;
	while (is_monster_at_cell(eng, cell))
	{
		if (wait_ticks > 300)
			return (0);
		simulate_engine_tick(eng, worker);
		if (eng->player->hp <= 0)
			return (0);
		wait_ticks++;
	}
	return (1);
}

static int	move_player_substeps(t_engine *eng, t_worker *worker, t_vec2 cell)
{
	t_vec2	d_pos;
	t_vec2	step;
	int		f;

	d_pos.d.x = cell.i.x + 0.5;
	d_pos.d.y = cell.i.y + 0.5;
	step.d.x = (d_pos.d.x - eng->player->pos.d.x) / 20.0;
	step.d.y = (d_pos.d.y - eng->player->pos.d.y) / 20.0;
	f = -1;
	while (++f < 20)
	{
		eng->player->pos.d.x += step.d.x;
		eng->player->pos.d.y += step.d.y;
		simulate_engine_tick(eng, worker);
		if (eng->player->hp <= 0)
			return (0);
	}
	eng->player->pos.d.x = d_pos.d.x;
	eng->player->pos.d.y = d_pos.d.y;
	return (1);
}

int	simulate_walk_path(t_engine *eng, t_worker *worker, t_path *path)
{
	int		step;
	t_vec2	cell;

	step = 0;
	while (++step < path->len)
	{
		cell.i.x = path->steps[step].i.x;
		cell.i.y = path->steps[step].i.y;
		if (!wait_for_monster(eng, worker, cell))
			return (0);
		if (!move_player_substeps(eng, worker, cell))
			return (0);
	}
	return (1);
}
