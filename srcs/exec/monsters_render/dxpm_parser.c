/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   dxpm_parser.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: blamotte <blamotte@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/11 01:39:07 by blamotte          #+#    #+#             */
/*   Updated: 2026/08/11 01:39:07 by blamotte         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub.h"
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>

unsigned int	get_transparent_color(t_img *img)
{
	if (img && img->addr && img->height > 656)
		return (*(unsigned int *)(img->addr + 656 * img->line_len));
	return (0xFF000000);
}

static void	parse_palette_color(char *line, int cpp,
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

void	parse_dxpm_header_line(char *line, int *cpp,
			unsigned int *palette, t_img *prev)
{
	char	*p;

	if (line[1] >= '0' && line[1] <= '9')
	{
		p = ft_strrchr(line, ' ');
		if (p)
			*cpp = ft_atoi(p + 1);
	}
	else
		parse_palette_color(line, *cpp, palette, prev);
}

static void	read_dxpm_lines(int fd, int *cpp,
				unsigned int *palette, t_img *prev_dst[2])
{
	char	*line;

	while (1)
	{
		line = get_next_line(fd);
		if (!line)
			break ;
		if (line[0] == '"')
			parse_dxpm_header_line(line, cpp, palette, prev_dst[0]);
		else if (line[0] == 'R')
			parse_dxpm_row(line, prev_dst[1], palette, *cpp);
		free(line);
	}
}

void	load_single_dxpm(void *mlx, char *path, t_img *prev, t_img *dst)
{
	int				fd;
	int				cpp;
	unsigned int	*palette;
	t_img			*prev_dst[2];

	*dst = copy_image_frame(mlx, prev);
	if (!dst->img_ptr)
		return ;
	fd = open(path, O_RDONLY);
	if (fd < 0)
		return ;
	cpp = 2;
	palette = ft_calloc(65536, sizeof(unsigned int));
	if (!palette)
		return ((void)close(fd));
	prev_dst[0] = prev;
	prev_dst[1] = dst;
	read_dxpm_lines(fd, &cpp, palette, prev_dst);
	free(palette);
	close(fd);
}
