#include "cub.h"

static void	bfs_init(t_bfs_state *state, t_vec2 start)
{
	int	i;
	int	map_size;

	map_size = state->width * state->height;
	i = 0;
	while (i < map_size)
		state->parent[i++] = -2;
	state->queue[0] = start.i.y * state->width + start.i.x;
	state->parent[start.i.y * state->width + start.i.x] = -1;
}

static int	bfs_backtrack_path(t_bfs_state *state, int curr, t_path *path)
{
	int	p_curr;
	int	len;
	int	idx;

	p_curr = curr;
	len = 0;
	while (p_curr != -1)
	{
		len++;
		p_curr = state->parent[p_curr];
	}
	path->len = len;
	p_curr = curr;
	idx = len - 1;
	while (p_curr != -1)
	{
		path->steps[idx].i.x = p_curr % state->width;
		path->steps[idx].i.y = p_curr / state->width;
		idx--;
		p_curr = state->parent[p_curr];
	}
	return (1);
}

static void	bfs_explore_neighbors(t_engine *eng, t_bfs_state *st,
				int curr, int avoid_monsters)
{
	int		d;
	t_vec2	n;
	int		nidx;
	int		dx[4] = {0, 0, -1, 1};
	int		dy[4] = {-1, 1, 0, 0};

	d = -1;
	while (++d < 4)
	{
		n.i.x = (curr % st->width) + dx[d];
		n.i.y = (curr / st->width) + dy[d];
		if (n.i.x >= 0 && n.i.x < st->width && n.i.y >= 0 && n.i.y < st->height)
		{
			nidx = n.i.y * st->width + n.i.x;
			if (st->parent[nidx] == -2 && is_tile_passable(eng, n))
			{
				if (!avoid_monsters || !is_monster_at_cell(eng, n))
				{
					st->parent[nidx] = curr;
					st->queue[st->tail++] = nidx;
				}
			}
		}
	}
}

int	bfs_find_path(t_engine *eng, t_worker *worker, t_vec2 st_tg[3],
		t_path *path)
{
	t_bfs_state	st;
	int			curr;

	st.width = get_map_width(get_blob_hdr(eng->blob));
	st.height = get_map_height(get_blob_hdr(eng->blob));
	st.queue = worker->bfs_queue;
	st.parent = worker->bfs_parent;
	bfs_init(&st, st_tg[0]);
	st.head = 0;
	st.tail = 1;
	while (st.head < st.tail)
	{
		curr = st.queue[st.head++];
		if (curr % st.width == st_tg[1].i.x && curr / st.width == st_tg[1].i.y)
			return (bfs_backtrack_path(&st, curr, path));
		bfs_explore_neighbors(eng, &st, curr, st_tg[2].i.x);
	}
	return (0);
}
