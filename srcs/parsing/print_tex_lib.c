/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   print_tex_lib.c                                    :+:      :+:    :+:   */
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
#define LIB_MARGIN_X 50
#define LIB_MARGIN_Y 50
#define LIB_GAP_X 50
#define LIB_GAP_Y 50
#define LIB_TEXT_DIST 20

void	put_pixel_image(t_screen *s, int x, int y, int color)
{
	unsigned char	r;
	unsigned char	g;
	unsigned char	b;
	int				len;

	if (x >= s->win_width || x < 0 || y >= s->win_height || y < 0)
		return ;
	len = s->win_width;
	r = (color >> 16) & 0xff;
	g = (color >> 8) & 0xff;
	b = color & 0xff;
	s->img.addr[(x * 4) + (len * 4 * y)] = b;
	s->img.addr[(x * 4) + (len * 4 * y) + 1] = g;
	s->img.addr[(x * 4) + (len * 4 * y) + 2] = r;
	s->img.addr[(x * 4) + (len * 4 * y) + 3] = 0;
}

uint32_t	get_pixel_img(t_img *img, int x, int y)
{
	uint32_t	*px;

	if (x < 0 || y < 0 || x >= img->width || y >= img->height)
		return (0);
	px = (uint32_t *)(img->addr + y * img->line_len + x * (img->bpp / 8));
	return (*px);
}

void	draw_tex_lib_elem(t_screen *s, t_img *tex, int start_x, int start_y)
{
	int			x;
	int			y;
	int			src_x;
	int			src_y;
	uint32_t	color;

	y = 0;
	while (y < LIB_TEX_HEIGHT)
	{
		x = 0;
		while (x < LIB_TEX_WIDTH)
		{
			if (tex->is_color)
				color = tex->hex_color;
			else
			{
				src_x = x * tex->width / LIB_TEX_WIDTH;
				src_y = y * tex->height / LIB_TEX_HEIGHT;
				color = get_pixel_img(tex, src_x, src_y);
			}
			put_pixel_image(s, start_x + x, start_y + y, color);
			x++;
		}
		y++;
	}
}

int	get_img_per_line(t_screen *s)
{
	int	usable_width;
	int	img_per_line;

	usable_width = s->win_width - 2 * LIB_MARGIN_X;
	img_per_line = (usable_width + LIB_GAP_X)
		/ (LIB_TEX_WIDTH + LIB_GAP_X);
	if (img_per_line < 1)
		img_per_line = 1;
	return (img_per_line);
}

void	make_yx_manip(int *x, int *y, int *col, int *row)
{
	*x = LIB_MARGIN_X + *col * (LIB_TEX_WIDTH + LIB_GAP_X);
	*y = LIB_MARGIN_Y + *row * (LIB_TEX_HEIGHT + LIB_GAP_Y);
}
