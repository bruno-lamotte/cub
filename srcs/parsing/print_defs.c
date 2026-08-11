/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   print_defs.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: blamotte <blamotte@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/11 02:00:00 by blamotte          #+#    #+#             */
/*   Updated: 2026/08/11 02:00:00 by blamotte         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/cub.h"

void	print_map_details(char id, t_data *d)
{
	if (id == 'm')
	{
		printf("m height = %u\n", d->m_height);
		printf("m width  = %u\n", d->m_max_width);
	}
	else if (id == 'b')
	{
		printf("b height = %u\n", d->b_height);
		printf("b width  = %u\n", d->b_max_width);
	}
}

void	print_textures_def(t_texture_def *tex, size_t len)
{
	size_t	i;

	i = 0;
	if (!tex)
		return ;
	while (i < len)
	{
		if (tex[i].name)
			printf("name = %15s | ", tex[i].name);
		if (!tex[i].is_color && tex[i].path)
			printf("path = %s\n", tex[i].path);
		else if (tex[i].is_color)
			printf("hexa = %#08x\n", tex[i].color);
		i++;
	}
	printf("------------------\n");
}

void	print_walls_def(t_wall_def *w, size_t len, t_texture_def *tex)
{
	size_t	i;

	i = 0;
	if (!w)
		return ;
	while (i < len)
	{
		printf("%c : %s, %s, %s, %s\n", w[i].symbol, tex[w[i].tex_north].name,
			tex[w[i].tex_west].name, tex[w[i].tex_south].name,
			tex[w[i].tex_east].name);
		i++;
	}
	printf("------------------\n");
}

void	print_airs_def(t_air_def *a, size_t len, t_texture_def *tex)
{
	size_t	i;

	i = 0;
	if (!a)
		return ;
	while (i < len)
	{
		printf("%c : %s, %s\n", a[i].symbol, tex[a[i].tex_floor].name,
			tex[a[i].tex_ceiling].name);
		i++;
	}
	printf("------------------\n");
}

void	print_doors_def(t_door_def *d, size_t len, t_texture_def *tex)
{
	size_t	i;

	i = 0;
	if (!d)
		return ;
	while (i < len)
	{
		printf("%c : %s, %s, %s\n", d[i].symbol, tex[d[i].tex_front].name,
			tex[d[i].tex_back].name, tex[d[i].tex_side].name);
		i++;
	}
	printf("------------------\n");
}
