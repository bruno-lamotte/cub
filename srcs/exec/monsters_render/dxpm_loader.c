#include "cub.h"

unsigned int	get_color_from_hex(char *hex)
{
	unsigned int	val;
	int				i;
	char			c;

	val = 0;
	i = -1;
	while (++i < 6)
	{
		c = hex[i];
		if (c >= '0' && c <= '9')
			val = val * 16 + (c - '0');
		else if (c >= 'a' && c <= 'f')
			val = val * 16 + (c - 'a' + 10);
		else if (c >= 'A' && c <= 'F')
			val = val * 16 + (c - 'A' + 10);
	}
	return (val);
}

int	get_tok_index(char *tok, int cpp)
{
	if (cpp == 1)
		return ((unsigned char)tok[0]);
	return (((unsigned char)tok[0] << 8) | (unsigned char)tok[1]);
}

t_img	copy_image_frame(void *mlx, t_img *src)
{
	t_img	dst;
	int		y;
	int		x;

	ft_bzero(&dst, sizeof(t_img));
	if (!src || !src->img_ptr || !src->addr)
		return (dst);
	dst.width = src->width;
	dst.height = src->height;
	dst.is_color = false;
	dst.img_ptr = mlx_new_image(mlx, dst.width, dst.height);
	dst.addr = mlx_get_data_addr(dst.img_ptr, &dst.bpp, &dst.line_len,
			&dst.endian);
	y = -1;
	while (++y < dst.height && dst.addr && src->addr)
	{
		x = -1;
		while (++x < dst.width)
			*(unsigned int *)(dst.addr + (y * dst.line_len + x * (dst.bpp
							/ 8))) = *(unsigned int *)(src->addr + (y
						* src->line_len + x * (src->bpp / 8)));
	}
	return (dst);
}

static char	*draw_run(t_img *dst, unsigned int *pal, char *ptr, int *p)
{
	int		i;
	char	tok[3];

	i = -1;
	while (++i < p[2])
	{
		tok[0] = ptr[0];
		tok[1] = 0;
		tok[2] = 0;
		if (p[3] == 2)
			tok[1] = ptr[1];
		ptr += p[3];
		*(unsigned int *)(dst->addr + (p[0] * dst->line_len
					+ (p[1] + i) * (dst->bpp / 8))) = pal[get_tok_index(tok,
				p[3])];
	}
	return (ptr);
}

void	parse_dxpm_row(char *line, t_img *dst, unsigned int *pal,
			int cpp)
{
	char	*ptr;
	int		p[4];

	ptr = line + 1;
	p[0] = ft_atoi(ptr);
	p[3] = cpp;
	while (*ptr >= '0' && *ptr <= '9')
		ptr++;
	while (*ptr)
	{
		while (*ptr == ' ' || *ptr == '\t')
			ptr++;
		if (!*ptr || *ptr == '\n' || *ptr == '\r')
			break ;
		p[1] = ft_atoi(ptr);
		while (*ptr >= '0' && *ptr <= '9')
			ptr++;
		if (*ptr++ != ',')
			break ;
		p[2] = ft_atoi(ptr);
		while (*ptr >= '0' && *ptr <= '9')
			ptr++;
		if (*ptr++ == ' ')
			ptr = draw_run(dst, pal, ptr, p);
	}
}
