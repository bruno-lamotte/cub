#include "cub.h"
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>
#include <math.h>

bool	get_one_xpm(void *mlx, char *path, t_img *img);

typedef struct s_mstr_draw
{
	t_img			*tex;
	unsigned int	transparent_color;
	double			sprite_height;
	int				draw_start_y;
	int				draw_start_x;
	double			scaled_width;
	int				tex_x;
	int				y_start;
	int				y_end;
}	t_mstr_draw;

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
			*(unsigned int *)(dst.addr + (y * dst.line_len
						+ x * (dst.bpp / 8))) = *(unsigned int *)(src->addr
					+ (y * src->line_len + x * (src->bpp / 8)));
		}
	}
	return (dst);
}

static inline char	*draw_dxpm_pixels(t_img *dst, unsigned int *palette,
						char *ptr, int row, int col, int len, int cpp)
{
	int				i;
	char			tok[3];
	unsigned int	color;

	i = -1;
	while (++i < len)
	{
		tok[0] = ptr[0];
		tok[1] = 0;
		tok[2] = 0;
		if (cpp == 2)
			tok[1] = ptr[1];
		ptr += cpp;
		color = palette[get_tok_index(tok, cpp)];
		*(unsigned int *)(dst->addr + (row * dst->line_len
					+ (col + i) * (dst->bpp / 8))) = color;
	}
	return (ptr);
}

static void	parse_dxpm_row(char *line, t_img *dst, unsigned int *palette,
				int cpp)
{
	char	*ptr;
	int		row;
	int		col;
	int		len;

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
		ptr = draw_dxpm_pixels(dst, palette, ptr + 1, row, col, len, cpp);
	}
}

static unsigned int	get_transparent_color(t_img *img)
{
	if (img && img->addr && img->height > 656)
		return (*(unsigned int *)(img->addr + 656 * img->line_len));
	return (0xFF000000);
}

static inline void	parse_palette_color(char *line, int cpp,
						unsigned int *palette, t_img *prev)
{
	char			tok[3];
	char			*c_str;
	unsigned int	color;

	ft_bzero(tok, sizeof(tok));
	tok[0] = line[1];
	if (cpp == 2)
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
		palette[get_tok_index(tok, cpp)] = color;
	}
}

static inline void	parse_dimensions(char *line, int *cpp)
{
	char	*p;

	p = line + 1;
	while (*p >= '0' && *p <= '9')
		p++;
	while (*p == ' ')
		p++;
	while (*p >= '0' && *p <= '9')
		p++;
	while (*p == ' ')
		p++;
	while (*p >= '0' && *p <= '9')
		p++;
	while (*p == ' ')
		p++;
	*cpp = ft_atoi(p);
}

static void	parse_dxpm_header_line(char *line, int *cpp,
				unsigned int *palette, t_img *prev)
{
	if (line[1] >= '0' && line[1] <= '9')
		parse_dimensions(line, cpp);
	else
		parse_palette_color(line, *cpp, palette, prev);
}

static inline void	read_dxpm_lines(int fd, int *cpp,
						unsigned int *palette, t_img *prev, t_img *dst)
{
	char	*line;

	while (1)
	{
		line = get_next_line(fd);
		if (!line)
			break ;
		if (line[0] == '"')
			parse_dxpm_header_line(line, cpp, palette, prev);
		else if (line[0] == 'R')
			parse_dxpm_row(line, dst, palette, *cpp);
		free(line);
	}
}

static void	load_single_dxpm(void *mlx, char *path, t_img *prev, t_img *dst)
{
	int				fd;
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
	read_dxpm_lines(fd, &cpp, palette, prev, dst);
	free(palette);
	close(fd);
}

static inline int	load_frame_path(t_engine *eng, char *line, int count)
{
	char	path[512];
	char	*filename;
	int		len;

	filename = line;
	len = ft_strlen(filename);
	while (len > 0 && (filename[len - 1] == '\n'
			|| filename[len - 1] == '\r'))
	{
		filename[len - 1] = '\0';
		len--;
	}
	ft_bzero(path, sizeof(path));
	ft_strlcpy(path, "assets/monstre/", sizeof(path));
	ft_strlcat(path, filename, sizeof(path));
	if (ft_strnstr(path, ".dxpm", ft_strlen(path)))
		load_single_dxpm(eng->screen->mlx_ptr, path,
			&eng->mstr_frames[count - 1], &eng->mstr_frames[count]);
	else if (ft_strnstr(path, ".xpm", ft_strlen(path)))
		get_one_xpm(eng->screen->mlx_ptr, path, &eng->mstr_frames[count]);
	else
		return (count);
	return (count + 1);
}

static inline int	process_anim_lines(t_engine *eng, char *lines[256],
						int line_count)
{
	int		i;
	int		count;
	char	*space;
	char	*line;

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
				count = load_frame_path(eng, line, count);
			}
		}
		free(line);
	}
	return (count);
}

void	load_monster_anim(t_engine *eng)
{
	int		fd;
	char	*line;
	char	*lines[256];
	int		line_count;

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
	eng->mstr_frame_count = process_anim_lines(eng, lines, line_count);
}

#ifndef MONSTER_SCALE
# define MONSTER_SCALE 1.8
#endif

static inline int	calc_mstr_draw_params(t_engine *eng, int stripe,
						int *w_h, t_sprite *s, t_mstr_draw *d)
{
	double	sp[2];
	double	trans[2];
	int		sprite_screen_x;
	int		frame_idx;

	frame_idx = (eng->pool.current_frame / 2) % eng->mstr_frame_count;
	d->tex = &eng->mstr_frames[frame_idx];
	d->transparent_color = get_transparent_color(&eng->mstr_frames[0]);
	get_sprite_coords(eng, s->pos.d.x, s->pos.d.y, sp, trans);
	sprite_screen_x = (int)((eng->screen->win_width / 2)
			* (1.0 + trans[0] / trans[1]));
	d->scaled_width = w_h[0] * MONSTER_SCALE;
	d->draw_start_x = -d->scaled_width / 2 + sprite_screen_x;
	d->tex_x = (int)((stripe - d->draw_start_x) * d->tex->width
			/ d->scaled_width);
	if (d->tex_x < 0 || d->tex_x >= d->tex->width)
		return (0);
	d->sprite_height = d->scaled_width * ((double)d->tex->height
			/ d->tex->width);
	d->draw_start_y = eng->screen->win_height / 2 + w_h[0] * 0.5
		- 1263.0 * (d->scaled_width / 1080.0);
	return (1);
}

static inline void	loop_draw_mstr_stripe(t_engine *eng, int stripe,
						t_sprite *s, t_mstr_draw *d)
{
	int				y;
	int				tex_y;
	unsigned int	col;
	int				rgb[3];

	d->y_start = fmax(0, d->draw_start_y);
	d->y_end = fmin(eng->screen->win_height - 1,
			d->draw_start_y + d->sprite_height);
	y = d->y_start - 1;
	while (++y <= d->y_end)
	{
		tex_y = (int)((y - d->draw_start_y) * d->tex->height
				/ d->sprite_height);
		if (tex_y < 0 || tex_y >= d->tex->height || tex_y >= 1264)
			continue ;
		col = *(unsigned int *)(d->tex->addr + (tex_y * d->tex->line_len
					+ d->tex_x * (d->tex->bpp / 8)));
		if (col == d->transparent_color || (col & 0x00FFFFFF) == 0x00FFFFFF)
			continue ;
		rgb[0] = (int)(((col >> 16) & 0xFF) * s->shade);
		rgb[1] = (int)(((col >> 8) & 0xFF) * s->shade);
		rgb[2] = (int)((col & 0xFF) * s->shade);
		put_pixel(&eng->screen->img2, stripe, y,
			(rgb[0] << 16) | (rgb[1] << 8) | rgb[2]);
	}
}

void	draw_monster_stripe(t_engine *eng, int stripe, int *w_h,
			t_sprite *s)
{
	t_mstr_draw	d;

	if (eng->mstr_frame_count <= 0)
		return ;
	if (!calc_mstr_draw_params(eng, stripe, w_h, s, &d))
		return ;
	loop_draw_mstr_stripe(eng, stripe, s, &d);
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
