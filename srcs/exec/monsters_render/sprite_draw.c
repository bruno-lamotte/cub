#include "cub.h"
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>

bool	get_one_xpm(void *mlx, char *path, t_img *img);

static unsigned int	get_color_from_hex(char *hex)
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

static int	get_tok_index(char *tok, int cpp)
{
	if (cpp == 1)
		return ((unsigned char)tok[0]);
	return (((unsigned char)tok[0] << 8) | (unsigned char)tok[1]);
}

static t_img	copy_image_frame(void *mlx, t_img *src)
{
	t_img	dst;
	int		y;
	int		x;

	dst.width = src->width;
	dst.height = src->height;
	dst.is_color = false;
	dst.img_ptr = mlx_new_image(mlx, dst.width, dst.height);
	dst.addr = mlx_get_data_addr(dst.img_ptr, &dst.bpp, &dst.line_len,
			&dst.endian);
	y = -1;
	while (++y < dst.height)
	{
		x = -1;
		while (++x < dst.width)
		{
			*(unsigned int *)(dst.addr + (y * dst.line_len + x * (dst.bpp / 8))) =
				*(unsigned int *)(src->addr + (y * src->line_len + x * (src->bpp / 8)));
		}
	}
	return (dst);
}

static void	parse_dxpm_row(char *line, t_img *dst, unsigned int *palette,
				int cpp)
{
	char	*ptr;
	int		row;
	int		col;
	int		len;
	int		i;

	ptr = line + 1;
	row = ft_atoi(ptr);
	while (*ptr >= '0' && *ptr <= '9')
		ptr++;
	while (*ptr)
	{
		while (*ptr == ' ' || *ptr == '\t')
			ptr++;
		if (!*ptr || *ptr == '\n' || *ptr == '\r')
			break ;
		col = ft_atoi(ptr);
		while (*ptr >= '0' && *ptr <= '9')
			ptr++;
		if (*ptr != ',')
			break ;
		ptr++;
		len = ft_atoi(ptr);
		while (*ptr >= '0' && *ptr <= '9')
			ptr++;
		if (*ptr != ' ')
			break ;
		ptr++;
		i = -1;
		while (++i < len)
		{
			char tok[3] = {0};
			tok[0] = ptr[0];
			if (cpp == 2)
				tok[1] = ptr[1];
			ptr += cpp;
			unsigned int color = palette[get_tok_index(tok, cpp)];
			*(unsigned int *)(dst->addr + (row * dst->line_len + (col + i)
						* (dst->bpp / 8))) = color;
		}
	}
}

static unsigned int	get_transparent_color(t_img *img)
{
	if (img && img->addr && img->height > 656)
		return (*(unsigned int *)(img->addr + 656 * img->line_len));
	return (0xFF000000);
}

static void	parse_dxpm_header_line(char *line, int *cpp,
				unsigned int *palette, t_img *prev)
{
	char	*c_str;
	char	tok[3];
	char	*p;
	unsigned int color;

	if (line[1] >= '0' && line[1] <= '9')
	{
		p = line + 1;
		while (*p >= '0' && *p <= '9') p++;
		while (*p == ' ') p++;
		while (*p >= '0' && *p <= '9') p++;
		while (*p == ' ') p++;
		while (*p >= '0' && *p <= '9') p++;
		while (*p == ' ') p++;
		*cpp = ft_atoi(p);
	}
	else
	{
		ft_bzero(tok, sizeof(tok));
		tok[0] = line[1];
		if (*cpp == 2)
			tok[1] = line[2];
		c_str = ft_strnstr(line, " c ", ft_strlen(line));
		if (c_str)
		{
			c_str += 3;
			color = 0;
			if (ft_strncmp(c_str, "None", 4) == 0)
				color = get_transparent_color(prev);
			else if (c_str[0] == '#')
				color = get_color_from_hex(c_str + 1);
			palette[get_tok_index(tok, *cpp)] = color;
		}
	}
}

static void	load_single_dxpm(void *mlx, char *path, t_img *prev, t_img *dst)
{
	int				fd;
	char			*line;
	int				cpp;
	unsigned int	*palette;

	*dst = copy_image_frame(mlx, prev);
	fd = open(path, O_RDONLY);
	if (fd < 0)
		return ;
	cpp = 2;
	palette = ft_calloc(65536, sizeof(unsigned int));
	if (!palette)
	{
		close(fd);
		return ;
	}
	while (1)
	{
		line = get_next_line(fd);
		if (!line)
			break ;
		if (line[0] == '"')
			parse_dxpm_header_line(line, &cpp, palette, prev);
		else if (line[0] == 'R')
			parse_dxpm_row(line, dst, palette, cpp);
		free(line);
	}
	free(palette);
	close(fd);
}

void	load_monster_anim(t_engine *eng)
{
	int		fd;
	char	*line;
	int		count;
	char	path[512];
	char	*filename;
	char	*space;
	int		len;
	char	*lines[256];
	int		line_count;
	int		i;

	fd = open("assets/monstre/monstre.anim", O_RDONLY);
	if (fd < 0)
		return ;
	line_count = 0;
	while (line_count < 256)
	{
		line = get_next_line(fd);
		if (!line)
			break ;
		lines[line_count++] = line;
	}
	close(fd);
	count = 0;
	i = -1;
	while (++i < line_count)
	{
		line = lines[i];
		if (ft_strncmp(line, "offset_", 7) != 0
			&& ft_strncmp(line, "original_", 9) != 0
			&& line[0] != '\n' && line[0] != '\0' && line[0] != '\r')
		{
			if (count < 128)
			{
				space = ft_strchr(line, ' ');
				if (space)
					*space = '\0';
				filename = line;
				len = ft_strlen(filename);
				while (len > 0 && (filename[len - 1] == '\n' || filename[len - 1] == '\r'))
				{
					filename[len - 1] = '\0';
					len--;
				}
				ft_bzero(path, sizeof(path));
				ft_strlcpy(path, "assets/monstre/", sizeof(path));
				ft_strlcat(path, filename, sizeof(path));
				if (ft_strnstr(path, ".dxpm", ft_strlen(path)))
				{
					load_single_dxpm(eng->screen->mlx_ptr, path,
						&eng->mstr_frames[count - 1], &eng->mstr_frames[count]);
					count++;
				}
				else if (ft_strnstr(path, ".xpm", ft_strlen(path)))
				{
					get_one_xpm(eng->screen->mlx_ptr, path, &eng->mstr_frames[count]);
					count++;
				}
			}
		}
		free(line);
	}
	eng->mstr_frame_count = count;
}

#ifndef MONSTER_SCALE
# define MONSTER_SCALE 1.8
#endif

void	draw_monster_stripe(t_engine *eng, int stripe, int *w_h,
			t_sprite *s)
{
	int				y;
	unsigned int	col;
	int				r;
	int				g;
	int				b;
	t_img			*tex;
	int				tex_x;
	int				tex_y;
	double			sprite_height;
	int				draw_start_y;
	int				draw_start_x;
	double			sp[2];
	double			trans[2];
	int				sprite_screen_x;
	unsigned int	transparent_color;
	int				frame_idx;
	int				y_start;
	int				y_end;
	double			scaled_width;

	if (eng->mstr_frame_count <= 0)
		return ;
	frame_idx = (eng->pool.current_frame / 2) % eng->mstr_frame_count;
	tex = &eng->mstr_frames[frame_idx];
	transparent_color = get_transparent_color(&eng->mstr_frames[0]);
	get_sprite_coords(eng, s->pos.d.x, s->pos.d.y, sp, trans);
	sprite_screen_x = (int)((eng->screen->win_width / 2) * (1.0 + trans[0] / trans[1]));
	scaled_width = w_h[0] * MONSTER_SCALE;
	draw_start_x = -scaled_width / 2 + sprite_screen_x;
	tex_x = (int)((stripe - draw_start_x) * tex->width / scaled_width);
	if (tex_x < 0 || tex_x >= tex->width)
		return ;
	sprite_height = scaled_width * ((double)tex->height / tex->width);
	draw_start_y = eng->screen->win_height / 2 + w_h[0] * 0.5 - 1263.0 * (scaled_width / 1080.0);
	y_start = draw_start_y;
	if (y_start < 0)
		y_start = 0;
	y_end = draw_start_y + sprite_height;
	if (y_end >= eng->screen->win_height)
		y_end = eng->screen->win_height - 1;
	y = y_start - 1;
	while (++y <= y_end)
	{
		tex_y = (int)((y - draw_start_y) * tex->height / sprite_height);
		if (tex_y < 0 || tex_y >= tex->height)
			continue ;
		if (tex_y >= 1264)
			continue ;
		col = *(unsigned int *)(tex->addr + (tex_y * tex->line_len + tex_x * (tex->bpp / 8)));
		if (col == transparent_color || (col & 0x00FFFFFF) == 0x00FFFFFF)
			continue ;
		r = (int)(((col >> 16) & 0xFF) * s->shade);
		g = (int)(((col >> 8) & 0xFF) * s->shade);
		b = (int)((col & 0xFF) * s->shade);
		put_pixel(&eng->screen->img2, stripe, y, (r << 16) | (g << 8) | b);
	}
}

static unsigned int	get_object_stripe_color(t_engine *eng, int diff,
						int h, char sym)
{
	if (sym == 'T')
	{
		if (diff >= h * 0.3 && diff <= h * 0.6)
			return (0x00FFFF);
		return (0x00FF00);
	}
	if (eng->alarm_triggered && (eng->pool.current_frame / 10) % 2)
		return (0xFF0000);
	if (eng->alarm_triggered)
		return (0x550000);
	return (0xCCCCCC);
}

void	draw_object_stripe(t_engine *eng, int stripe, int *w_h,
			t_sprite *s)
{
	int				y;
	unsigned int	col;
	int				r;
	int				g;
	int				b;

	y = w_h[3] - 1;
	while (++y <= w_h[4])
	{
		col = get_object_stripe_color(eng, y - w_h[3], w_h[4] - w_h[3],
				s->sym);
		r = (int)(((col >> 16) & 0xFF) * s->shade);
		g = (int)(((col >> 8) & 0xFF) * s->shade);
		b = (int)((col & 0xFF) * s->shade);
		put_pixel(&eng->screen->img2, stripe, y, (r << 16) | (g << 8) | b);
	}
}

void	draw_objects_3d(t_engine *eng)
{
	(void)eng;
}
