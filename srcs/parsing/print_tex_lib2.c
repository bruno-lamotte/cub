/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   print_tex_lib2.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: blamotte <blamotte@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/11 02:00:00 by blamotte          #+#    #+#             */
/*   Updated: 2026/08/11 02:00:00 by blamotte         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/cub.h"

#define LIB_TEX_WIDTH 250
#define LIB_TEX_HEIGHT 250
#define LIB_TEXT_DIST 20

void	make_yx_manip(int *x, int *y, int *col, int *row);

static void	draw_tex_lib_names(t_screen *s, t_data *d, int img_per_line)
{
	int	i;
	int	x;
	int	y;
	int	col;
	int	row;

	i = -1;
	while (++i < d->textures_len)
	{
		col = i % img_per_line;
		row = i / img_per_line;
		make_yx_manip(&x, &y, &col, &row);
		mlx_string_put(s->mlx_ptr, s->win_ptr, x,
			y + LIB_TEX_HEIGHT + LIB_TEXT_DIST, 0xFFFFFF,
			d->textures_defs[i].name);
	}
}

void	draw_tex_lib(t_screen *s, t_data *d, int i)
{
	int		img_per_line;
	int		x;
	int		y;
	int		col;
	int		row;

	img_per_line = get_img_per_line(s);
	i = -1;
	while (++i < d->textures_len)
	{
		col = i % img_per_line;
		row = i / img_per_line;
		make_yx_manip(&x, &y, &col, &row);
		draw_tex_lib_elem(s, &d->img_tab[i], x, y);
	}
	mlx_put_image_to_window(s->mlx_ptr, s->win_ptr, s->img.img_ptr, 0, 0);
	draw_tex_lib_names(s, d, img_per_line);
}
