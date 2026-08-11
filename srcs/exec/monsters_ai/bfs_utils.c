/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   bfs_utils.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: blamotte <blamotte@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/11 00:33:49 by blamotte          #+#    #+#             */
/*   Updated: 2026/08/11 00:33:49 by blamotte         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub.h"
#include <stdlib.h>

void	init_bfs_arrays(int *parent, int size)
{
	int	i;

	i = 0;
	while (i < size)
	{
		parent[i] = -1;
		i++;
	}
}

void	push_neighbor(t_bfs_reach *b, int x, int y)
{
	int	idx;

	if (x < 0 || x >= b->size.i.x || y < 0 || y >= b->size.i.y)
		return ;
	if (!is_walkable_for_routing(x, y, b->eng->blob))
		return ;
	idx = y * b->size.i.x + x;
	if (!b->visited[idx])
	{
		b->visited[idx] = 1;
		b->queue[b->tail++] = idx;
	}
}

int	init_bfs_fallback(t_bfs_reach *b)
{
	b->visited = malloc(sizeof(int) * b->size.i.x * b->size.i.y);
	b->queue = malloc(sizeof(int) * b->size.i.x * b->size.i.y);
	return (b->visited != NULL && b->queue != NULL);
}

int	init_bfs(t_bfs_reach *b, t_engine *eng, t_vec2 start,
		t_worker *w)
{
	b->eng = eng;
	b->size.i.x = get_map_width(get_blob_hdr(eng->blob));
	b->size.i.y = get_map_height(get_blob_hdr(eng->blob));
	if (w)
	{
		b->visited = w->bfs_visited;
		b->queue = w->bfs_queue;
	}
	else if (!init_bfs_fallback(b))
		return (0);
	ft_bzero(b->visited, sizeof(int) * b->size.i.x * b->size.i.y);
	b->head = 0;
	b->tail = 0;
	b->count = 0;
	b->visited[start.i.y * b->size.i.x + start.i.x] = 1;
	b->queue[b->tail++] = start.i.y * b->size.i.x + start.i.x;
	return (1);
}

void	free_bfs_fallback(t_bfs_reach *b, t_worker *w)
{
	if (!w)
	{
		free(b->visited);
		free(b->queue);
	}
}
