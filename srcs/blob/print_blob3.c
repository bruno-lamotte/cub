/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   print_blob3.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: blamotte <blamotte@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/11 02:00:00 by blamotte          #+#    #+#             */
/*   Updated: 2026/08/11 02:00:00 by blamotte         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/cub.h"

void	print_one_mstr_rt(t_monster_rt *d, size_t width)
{
	size_t	x;
	size_t	y;

	x = d->map_id % width;
	y = d->map_id / width;
	printf("X = %zd | Y = %zd\n", x, y);
	printf("Xf = %f | Yf = %f\n", d->pos.d.x, d->pos.d.y);
	printf("hp %d\n", d->current_hp);
	printf("def id -> %d\n", d->def_id);
	printf("-------\n");
}

void	print_mstr_rt(void *blob)
{
	t_blob_hdr		*hdr;
	size_t			width;
	t_monster_rt	*d;
	size_t			i;

	printf("\nMSTR RT :\n");
	printf("-------\n");
	hdr = get_blob_hdr(blob);
	width = get_map_width(hdr);
	d = get_monster_rt(blob);
	i = 0;
	while (i < hdr->mstr_rt.u_data.count)
	{
		print_one_mstr_rt(&d[i], width);
		i++;
	}
}

void	print_title(const char *title)
{
	printf("\n=== %s ===\n", title);
}

void	print_bdef_wall_arr(t_bdef_wall *arr, uint8_t len)
{
	uint8_t	i;

	print_title("BDEF_WALL");
	if (!arr || len == 0)
	{
		printf("(empty)\n");
		return ;
	}
	printf(" idx | north south west east\n");
	printf("-----+------------------------\n");
	i = 0;
	while (i < len)
	{
		printf(" %3u | %5u %5u %4u %4u\n",
			i, arr[i].tex_north, arr[i].tex_south,
			arr[i].tex_west, arr[i].tex_east);
		i++;
	}
}

void	print_bdef_air_arr(t_bdef_air *arr, uint8_t len)
{
	uint8_t	i;

	print_title("BDEF_AIR");
	if (!arr || len == 0)
	{
		printf("(empty)\n");
		return ;
	}
	printf(" idx | floor ceiling\n");
	printf("-----+---------------\n");
	i = 0;
	while (i < len)
	{
		printf(" %3u | %5u %7u\n",
			i, arr[i].tex_floor, arr[i].tex_ceiling);
		i++;
	}
}
