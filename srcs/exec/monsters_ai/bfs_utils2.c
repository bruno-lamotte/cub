/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   bfs_utils2.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: blamotte <blamotte@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/11 00:33:49 by blamotte          #+#    #+#             */
/*   Updated: 2026/08/11 00:33:49 by blamotte         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub.h"

int	get_reachable_cells(t_engine *eng, t_vec2 start,
		t_vec2 *cells, t_worker *w)
{
	t_bfs_reach	b;
	int			curr;

	if (!init_bfs(&b, eng, start, w))
		return (0);
	while (b.head < b.tail && b.count < 1024)
	{
		curr = b.queue[b.head++];
		cells[b.count].i.x = curr % b.size.i.x;
		cells[b.count].i.y = curr / b.size.i.x;
		b.count++;
		push_neighbor(&b, cells[b.count - 1].i.x + 1, cells[b.count - 1].i.y);
		push_neighbor(&b, cells[b.count - 1].i.x - 1, cells[b.count - 1].i.y);
		push_neighbor(&b, cells[b.count - 1].i.x, cells[b.count - 1].i.y + 1);
		push_neighbor(&b, cells[b.count - 1].i.x, cells[b.count - 1].i.y - 1);
	}
	free_bfs_fallback(&b, w);
	return (b.count);
}
