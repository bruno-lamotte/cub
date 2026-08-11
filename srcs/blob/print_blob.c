/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   print_blob.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: blamotte <blamotte@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/11 00:33:49 by blamotte          #+#    #+#             */
/*   Updated: 2026/08/11 02:00:00 by blamotte         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/cub.h"

void	print_sep_line(size_t width)
{
	size_t	i;

	printf("    +");
	i = 0;
	while (i < width)
	{
		printf("-----");
		i++;
	}
	printf("+\n");
}

void	print_one_grid(const char *title, uint8_t *arr,
	size_t width, size_t height)
{
	ssize_t	l;
	ssize_t	c;
	size_t	idx;

	printf("\n%s\n", title);
	print_sep_line(width);
	l = -1;
	while (++l < (ssize_t)height)
	{
		printf("%3zu |", l);
		c = -1;
		while (++c < (ssize_t)width)
		{
			idx = l * width + c;
			printf(" %3u ", arr[idx]);
		}
		printf("|\n");
	}
	print_sep_line(width);
	printf("      ");
	c = -1;
	while (++c < (ssize_t)width)
		printf("%3zu  ", c);
	printf("\n");
}

char	get_flag_char(uint8_t flags)
{
	if (flags == 0)
		return ('.');
	if (flags & CELL_HAS_WALL)
		return ('W');
	if (flags & CELL_HAS_DOOR)
		return ('D');
	if (flags & CELL_HAS_OBJ)
		return ('O');
	if (flags & CELL_HAS_AIR)
		return ('A');
	return ('?');
}

void	print_map_flags_pretty(t_map_soa map, size_t width, size_t height)
{
	ssize_t	l;
	ssize_t	c;
	size_t	idx;

	printf("\nFLAG VIEW\n");
	print_sep_line(width);
	l = -1;
	while (++l < (ssize_t)height)
	{
		printf("%3zu |", l);
		c = -1;
		while (++c < (ssize_t)width)
		{
			idx = l * width + c;
			printf(" %3c ", get_flag_char(map.flags[idx]));
		}
		printf("|\n");
	}
	print_sep_line(width);
	printf("      ");
	c = -1;
	while (++c < (ssize_t)width)
		printf("%3zu  ", c);
	printf("\n");
}

void	print_map_soa(t_map_soa map, size_t width, size_t height)
{
	print_map_flags_pretty(map, width, height);
	print_one_grid("FLAGS", map.flags, width, height);
	print_one_grid("BLOCK_ID", map.block_id, width, height);
	print_one_grid("OCC_ID", map.occ_id, width, height);
}
