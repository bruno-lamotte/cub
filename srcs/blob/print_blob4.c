/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   print_blob4.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: blamotte <blamotte@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/11 02:00:00 by blamotte          #+#    #+#             */
/*   Updated: 2026/08/11 02:00:00 by blamotte         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/cub.h"

void	print_bdef_door_arr(t_bdef_door *arr, uint8_t len)
{
	uint8_t	i;

	print_title("BDEF_DOOR");
	if (!arr || len == 0)
	{
		printf("(empty)\n");
		return ;
	}
	printf(" idx | front back side\n");
	printf("-----+----------------\n");
	i = 0;
	while (i < len)
	{
		printf(" %3u | %5u %4u %4u\n",
			i, arr[i].tex_front, arr[i].tex_back, arr[i].tex_side);
		i++;
	}
}

void	print_bdef_monster_arr(t_bdef_monster *arr, uint8_t len)
{
	uint8_t	i;

	print_title("BDEF_MONSTER");
	if (!arr || len == 0)
	{
		printf("(empty)\n");
		return ;
	}
	printf(" idx | tex hp size_pc\n");
	printf("-----+---------------\n");
	i = 0;
	while (i < len)
	{
		printf(" %3u | %3u %3u %7u\n",
			i, arr[i].tex, arr[i].initial_hp, arr[i].size_pc);
		i++;
	}
}

void	print_bdef_obj_arr(t_bdef_obj *arr, uint8_t len)
{
	uint8_t	i;

	print_title("BDEF_OBJ");
	if (!arr || len == 0)
	{
		printf("(empty)\n");
		return ;
	}
	printf(" idx | tex size_pc\n");
	printf("-----+------------\n");
	i = 0;
	while (i < len)
	{
		printf(" %3u | %3u %7u\n",
			i, arr[i].tex, arr[i].size_pc);
		i++;
	}
}

void	print_bdef_tex_arr(t_bdef_tex *arr, uint8_t len)
{
	uint8_t	i;

	print_title("BDEF_TEX");
	if (!arr || len == 0)
	{
		printf("(empty)\n");
		return ;
	}
	printf(" idx | width height | kind   | value\n");
	printf("-----+--------------+--------+------------\n");
	i = 0;
	while (i < len)
	{
		if (arr[i].width == 0 && arr[i].height == 0)
			printf(" %3u | %5u %6u | color  | 0x%08X\n",
				i, arr[i].width, arr[i].height, arr[i].u_val.color);
		else
			printf(" %3u | %5u %6u | offset | %10u\n",
				i, arr[i].width, arr[i].height, arr[i].u_val.offset);
		i++;
	}
}

void	print_bdefs(void *blob)
{
	t_blob_hdr	*h;

	h = get_blob_hdr(blob);
	print_bdef_wall_arr(get_wall_bdef(blob), h->wall_def.u_data.count);
	print_bdef_air_arr(get_air_bdef(blob), h->air_def.u_data.count);
	print_bdef_door_arr(get_door_bdef(blob), h->door_def.u_data.count);
	print_bdef_monster_arr(get_mstr_bdef(blob), h->mstr_def.u_data.count);
	print_bdef_obj_arr(get_obj_bdef(blob), h->obj_def.u_data.count);
	print_bdef_tex_arr(get_tex_bdef(blob), h->tex_def.u_data.count);
}
