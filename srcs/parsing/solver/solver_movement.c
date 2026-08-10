#include "cub.h"

static int	try_walk_bfs_path(t_engine *eng, t_worker *worker,
				t_vec2 st_tg[3], t_path *path)
{
	t_solver_checkpoint	*cp;

	if (bfs_find_path(eng, worker, st_tg, path))
	{
		cp = clone_checkpoint(eng);
		if (simulate_walk_path(eng, worker, path))
		{
			free_checkpoint(cp);
			return (1);
		}
		restore_checkpoint(eng, cp);
		free_checkpoint(cp);
	}
	return (0);
}

int	simulate_move_to_target(t_engine *eng, t_worker *worker, t_vec2 target)
{
	t_vec2	st_tg[3];
	t_path	path;

	st_tg[0].i.x = (int)eng->player->pos.d.x;
	st_tg[0].i.y = (int)eng->player->pos.d.y;
	st_tg[1] = target;
	if (st_tg[0].i.x == target.i.x && st_tg[0].i.y == target.i.y)
		return (1);
	st_tg[2].i.x = 1;
	if (try_walk_bfs_path(eng, worker, st_tg, &path))
		return (1);
	st_tg[2].i.x = 0;
	if (try_walk_bfs_path(eng, worker, st_tg, &path))
		return (1);
	return (0);
}
