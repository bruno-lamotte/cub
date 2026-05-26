#include "../../includes/cub.h"

#define LIB_TEX_WIDTH 250
#define LIB_TEX_HEIGHT 250
#define LIB_MARGIN_X 50
#define LIB_MARGIN_Y 50
#define LIB_GAP_X 50
#define LIB_GAP_Y 50
#define LIB_TEXT_DIST 20

void	put_pixel_image(int x, int y, char *str, int color)
{
	unsigned char r;
	unsigned char g;
	unsigned char b;
	int len;

	if (x >= WINDOW_WIDTH || x < 0 || y >= WINDOW_HEIGHT || y < 0)
		return;

	len = WINDOW_WIDTH;

	r = (color >> 16) & 0xff;
	g = (color >> 8) & 0xff;
	b = color & 0xff;

	str[(x * 4) + (len * 4 * y)] = b;
	str[(x * 4) + (len * 4 * y) + 1] = g;
	str[(x * 4) + (len * 4 * y) + 2] = r;
	str[(x * 4) + (len * 4 * y) + 3] = 0;
	return ;
}

uint32_t get_pixel_img(t_img *img, int x, int y)
{
	uint32_t	*px;

	if (x < 0 || y < 0 || x >= img->width || y >= img->height)
		return (0);
	px = (uint32_t *)(img->addr + y * img->line_len + x * (img->bpp / 8));
	return (*px);
}

void draw_tex_lib_elem(t_screen *s, t_img *tex, int start_x, int start_y)
{
	int x;
	int y;
	int src_x;
	int src_y;
	uint32_t color;

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
			put_pixel_image(start_x + x, start_y + y,  s->img.addr, color);
			x++;
		}
		y++;
	}
}

int	get_img_per_line(void)
{
	int	usable_width;
	int	img_per_line;

	usable_width = WINDOW_WIDTH - 2 * LIB_MARGIN_X;
	img_per_line = (usable_width + LIB_GAP_X)
		/ (LIB_TEX_WIDTH + LIB_GAP_X);
	if (img_per_line < 1)
		img_per_line = 1;
	return (img_per_line);
}


void	draw_tex_lib(t_screen *s, t_data *d)
{
	uint8_t	i;
	int		img_per_line;
	int		x;
	int		y;
	int		col;
	int		row;

	img_per_line = get_img_per_line();
	i = 0;
	// draw textures 
	while (i < d->textures_len)
	{
		col = i % img_per_line;
		row = i / img_per_line;
		x = LIB_MARGIN_X + col * (LIB_TEX_WIDTH + LIB_GAP_X);
		y = LIB_MARGIN_Y + row * (LIB_TEX_HEIGHT + LIB_GAP_Y);
		draw_tex_lib_elem(s, &d->img_tab[i], x, y);
		i++;
	}
	mlx_put_image_to_window(s->mlx_ptr, s->win_ptr, s->img.img_ptr, 0, 0);
	i = 0;
	// draw textures names
	while (i < d->textures_len)
	{
		col = i % img_per_line;
		row = i / img_per_line;
		x = LIB_MARGIN_X + col * (LIB_TEX_WIDTH + LIB_GAP_X);
		y = LIB_MARGIN_Y + row * (LIB_TEX_HEIGHT + LIB_GAP_Y);
		mlx_string_put(s->mlx_ptr, s->win_ptr, x, y + LIB_TEX_HEIGHT + LIB_TEXT_DIST, 0xFFFFFF, d->textures_defs[i].name);
		i++;
	}
}