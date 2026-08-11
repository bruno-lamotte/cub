/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   print_blob2.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: blamotte <blamotte@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/11 02:00:00 by blamotte          #+#    #+#             */
/*   Updated: 2026/08/11 02:00:00 by blamotte         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/cub.h"

void	print_hdr_seg_map(t_blob_seg *map)
{
	printf("start : %u\n", map->start);
	printf("end   : %u\n", map->end);
	printf("used  : %u\n", map->used);
	printf("width : %u\n", map->u_data.map_data.width);
	printf("height: %u\n", map->u_data.map_data.height);
}

void	print_hdr_seg_normal(t_blob_seg *s)
{
	printf("start : %u\n", s->start);
	printf("end   : %u\n", s->end);
	printf("used  : %u\n", s->used);
	printf("count : %u\n", s->u_data.count);
}

void	print_hdr(t_blob_hdr *h)
{
	printf("-----\nBLOB HEADER :\n-----\nmap:\n");
	print_hdr_seg_map(&h->map);
	printf("-----\ndoor_rt:\nsizeof struct -> %zd\n", sizeof(t_door_rt));
	print_hdr_seg_normal(&h->door_rt);
	printf("-----\nmstr_rt:\n");
	printf("sizeof struct -> %zd\n", sizeof(t_monster_rt));
	print_hdr_seg_normal(&h->mstr_rt);
	printf("-----\nwall_def:\nsizeof struct -> %zd\n", sizeof(t_bdef_wall));
	print_hdr_seg_normal(&h->wall_def);
	printf("-----\nair_def:\nsizeof struct -> %zd\n", sizeof(t_bdef_air));
	print_hdr_seg_normal(&h->air_def);
	printf("-----\ndoor_def:\nsizeof struct -> %zd\n", sizeof(t_bdef_door));
	print_hdr_seg_normal(&h->door_def);
	printf("-----\nmstr_def:\n");
	printf("sizeof struct -> %zd\n", sizeof(t_bdef_monster));
	print_hdr_seg_normal(&h->mstr_def);
	printf("-----\nobj_def:\n");
	print_hdr_seg_normal(&h->obj_def);
	printf("-----\ntex_def:\nsizeof struct -> %zd\n", sizeof(t_bdef_tex));
	print_hdr_seg_normal(&h->tex_def);
	printf("-----\npixels_data:\n");
	print_hdr_seg_normal(&h->pixels_data);
	printf("-----\n");
	printf("end offset = %u\n", h->end_offset);
	printf("-----\n");
}

void	print_one_door_rt(t_door_rt *d, size_t width)
{
	size_t	x;
	size_t	y;

	x = d->map_id % width;
	y = d->map_id / width;
	printf("X = %zd | Y = %zd\n", x, y);
	printf("open ratio %d/255\n", d->open_ratio_255);
	printf("def id -> %d\n", d->def_id);
	printf("-------\n");
}

void	print_door_rt(void *blob)
{
	t_blob_hdr	*hdr;
	size_t		width;
	t_door_rt	*d;
	size_t		i;

	printf("\nDOOR RT :\n");
	printf("-------\n");
	hdr = get_blob_hdr(blob);
	width = get_map_width(hdr);
	d = get_door_rt(blob);
	i = 0;
	while (i < hdr->door_rt.u_data.count)
	{
		print_one_door_rt(&d[i], width);
		i++;
	}
}
