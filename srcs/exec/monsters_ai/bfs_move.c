#include "cub.h"
#include <math.h>

t_vec2	find_next_step(t_monster_rt *m, t_engine *eng, t_vec2 target, t_worker *w)
{
	t_vec2	start;
	t_vec2	tgt;
	int		wdt;
	int		next_idx;

	start = (t_vec2){.i = {(int)m->pos.d.x, (int)m->pos.d.y}};
	tgt = (t_vec2){.i = {(int)target.d.x, (int)target.d.y}};
	wdt = get_map_width(get_blob_hdr(eng->blob));
	init_bfs_arrays(w->bfs_parent, wdt * get_map_height(get_blob_hdr(eng->blob)));
	if (bfs_run(start, tgt, eng, w))
	{
		next_idx = bfs_backtrack(start.i.y * wdt + start.i.x,
				tgt.i.y * wdt + tgt.i.x, w->bfs_parent);
		start.d.x = (next_idx % wdt) + 0.5;
		start.d.y = (next_idx / wdt) + 0.5;
	}
	else
		start = target;
	return (start);
}

void	mstr_move_towards(t_monster_rt *m, t_engine *eng,
				t_vec2 target)
{
	t_vec2	d;
	float	len2;
	int		self_idx;

	self_idx = m - get_monster_rt(eng->blob);
	d.d.x = target.d.x - m->pos.d.x;
	d.d.y = target.d.y - m->pos.d.y;
	len2 = (float)(d.d.x * d.d.x + d.d.y * d.d.y);
	if (len2 > 0.01f)
	{
		m->dir.d.x = d.d.x * fast_inv_sqrt(len2);
		m->dir.d.y = d.d.y * fast_inv_sqrt(len2);
	}
	d.d.x = m->pos.d.x + m->dir.d.x * 0.035;
	d.d.y = m->pos.d.y + m->dir.d.y * 0.035;
	if (is_valid_position(d.d.x, m->pos.d.y, eng->blob)
		&& check_monster_collision(d.d.x, m->pos.d.y, self_idx, eng))
		m->pos.d.x = d.d.x;
	if (is_valid_position(m->pos.d.x, d.d.y, eng->blob)
		&& check_monster_collision(m->pos.d.x, d.d.y, self_idx, eng))
		m->pos.d.y = d.d.y;
}
