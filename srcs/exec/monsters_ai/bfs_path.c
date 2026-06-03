#include "cub.h"
#include <stdlib.h>

static void	bfs_check_neighbor(t_bfs *b, int nx, int ny, int curr)
{
	int	idx;

	if (nx < 0 || nx >= b->width || ny < 0 || ny >= b->height)
		return ;
	if (!is_walkable_for_routing(nx, ny, b->blob))
		return ;
	idx = ny * b->width + nx;
	if (b->parent[idx] == -1)
	{
		b->parent[idx] = curr;
		b->queue[b->tail] = idx;
		b->tail++;
	}
}

static void	bfs_expand_neighbors(t_bfs *b, int curr)
{
	int	cx;
	int	cy;

	cx = curr % b->width;
	cy = curr / b->width;
	bfs_check_neighbor(b, cx + 1, cy, curr);
	bfs_check_neighbor(b, cx - 1, cy, curr);
	bfs_check_neighbor(b, cx, cy + 1, curr);
	bfs_check_neighbor(b, cx, cy - 1, curr);
}

static int	init_bfs_run_struct(t_bfs *b, t_engine *eng, t_vec2 start,
				t_worker *w)
{
	b->width = get_map_width(get_blob_hdr(eng->blob));
	b->height = get_map_height(get_blob_hdr(eng->blob));
	b->blob = eng->blob;
	b->tail = 0;
	if (w)
	{
		b->parent = w->bfs_parent;
		b->queue = w->bfs_queue;
	}
	else
	{
		b->parent = malloc(sizeof(int) * b->width * b->height);
		b->queue = malloc(sizeof(int) * b->width * b->height);
	}
	if (!b->parent || !b->queue)
		return (0);
	init_bfs_arrays(b->parent, b->width * b->height);
	b->queue[b->tail++] = start.i.y * b->width + start.i.x;
	b->parent[start.i.y * b->width + start.i.x] = -2;
	return (1);
}

static void	free_bfs_run_fallback(t_bfs *b, t_worker *w)
{
	if (!w)
	{
		free(b->parent);
		free(b->queue);
	}
}

int	bfs_run(t_vec2 start, t_vec2 target, t_engine *eng, t_worker *w)
{
	t_bfs	b;
	int		head;
	int		curr;
	int		target_idx;

	if (!init_bfs_run_struct(&b, eng, start, w))
		return (0);
	head = 0;
	target_idx = target.i.y * b.width + target.i.x;
	while (head < b.tail)
	{
		curr = b.queue[head++];
		if (curr == target_idx)
		{
			free_bfs_run_fallback(&b, w);
			return (1);
		}
		bfs_expand_neighbors(&b, curr);
	}
	free_bfs_run_fallback(&b, w);
	return (0);
}

int	bfs_backtrack(int start_idx, int target_idx, int *parent)
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
