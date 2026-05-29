#include "cub.h"

void	put_pixel(t_img *img, int x, int y, int color)
{
	char	*dst;

	dst = img->addr + (y * img->line_len + x * (img->bpp / 8));
	*(unsigned int *)dst = color;
}

unsigned int	blend_colors(unsigned int src, unsigned int dst,
					unsigned char alpha)
{
	unsigned char	r;
	unsigned char	g;
	unsigned char	b;
	unsigned char	inv_alpha;

	inv_alpha = 255 - alpha;
	r = ((src >> 16 & 0xFF) * alpha + (dst >> 16 & 0xFF) * inv_alpha) >> 8;
	g = ((src >> 8 & 0xFF) * alpha + (dst >> 8 & 0xFF) * inv_alpha) >> 8;
	b = ((src & 0xFF) * alpha + (dst & 0xFF) * inv_alpha) >> 8;
	return (r << 16 | g << 8 | b);
}

void	draw_rect(t_img *img, t_vec2 p, t_vec2 sz, int color)
{
	int	i;
	int	j;

	i = -1;
	while (++i < sz.i.y)
	{
		j = -1;
		while (++j < sz.i.x)
		{
			if (p.i.x + j >= 0 && p.i.x + j < img->width
				&& p.i.y + i >= 0 && p.i.y + i < img->height)
				put_pixel(img, p.i.x + j, p.i.y + i, color);
		}
	}
}

void	draw_rect_blend(t_img *img, t_vec2 p, t_vec2 sz, int color)
{
	int				i;
	int				j;
	char			*dst_ptr;
	unsigned int	orig;

	i = 0;
	while (i < sz.i.y)
	{
		j = 0;
		while (j < sz.i.x)
		{
			if (p.i.x + j >= 0 && p.i.x + j < img->width
				&& p.i.y + i >= 0 && p.i.y + i < img->height)
			{
				dst_ptr = img->addr + ((p.i.y + i) * img->line_len
						+ (p.i.x + j) * (img->bpp / 8));
				orig = *(unsigned int *)dst_ptr;
				*(unsigned int *)dst_ptr = blend_colors(color, orig, 180);
			}
			j++;
		}
		i++;
	}
}

void	draw_filled_circle(t_img *img, t_vec2 c, int r, int color)
{
	int	x;
	int	y;

	y = -r - 1;
	while (++y <= r)
	{
		x = -r - 1;
		while (++x <= r)
		{
			if (x * x + y * y <= r * r)
			{
				if (c.i.x + x >= 0 && c.i.x + x < img->width
					&& c.i.y + y >= 0 && c.i.y + y < img->height)
					put_pixel(img, c.i.x + x, c.i.y + y, color);
			}
		}
	}
}
