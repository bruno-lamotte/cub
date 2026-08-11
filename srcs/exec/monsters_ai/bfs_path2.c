/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   bfs_path2.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: blamotte <blamotte@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/11 00:33:49 by blamotte          #+#    #+#             */
/*   Updated: 2026/08/11 00:33:49 by blamotte         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub.h"

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
