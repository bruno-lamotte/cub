#include "../../includes/cub.h"

void free_img_tab(void *mlx, t_img *img, size_t lim)
{
	size_t i;

	i = 0;
	while (i < lim)
	{
		if (!img[i].is_color && img[i].img_ptr)
			mlx_destroy_image(mlx, img[i].img_ptr);
		i++;
	}
	free(img);
}

bool get_one_xpm(void *mlx, char *path, t_img *img)
{
	if (!mlx || !path || !img)
		return (printf("3\n"), false);
	// printf("PATH = |%s| w %d | h %d\n", path,  img->width,  img->height);
	img->img_ptr = mlx_xpm_file_to_image(mlx, path, &img->width, &img->height);
	if (!img->img_ptr)
		return (printf("4\n"), false);
	img->addr = mlx_get_data_addr(img->img_ptr, &img->bpp, &img->line_len, &img->endian);
	if (!img->addr || img->bpp != 32)
		return (mlx_destroy_image(mlx, img->img_ptr), printf("4\n"), false);
	return (true);
}

bool get_all_tex(t_data *data, t_screen *s)
{
	size_t	i;
	size_t	len;
	t_img	*img;
	t_texture_def *defs;
	t_index	idx  = {0};

	defs = data->textures_defs;
	// print_textures_def(defs, data->textures_len);
	len = data->textures_len;
	img = ft_calloc(len, sizeof(t_img));
	if (!img)
		return (idx.err = PERR_MALLOC_ERR, printf("1\n"), print_perr(&idx), false);
	i = 0;
	while (i < len)
	{
		if (defs[i].is_color)
		{
			img[i].hex_color = defs[i].color;
			img[i].is_color = true;
		}
		else if (!get_one_xpm(s->mlx_ptr, defs[i].path, &img[i]))
			return (idx.err = PERR_MLX_ERROR, free_img_tab(s->mlx_ptr, img, i), printf("2 | %zd | %s\n", i, defs[i].path), print_perr(&idx), false);
		
		if (!defs[i].is_color)		
			mlx_put_image_to_window(s->mlx_ptr, s->win_ptr, img[i].img_ptr, 0, 0);
		i++;
	}
	data->img_tab = img;
	return (true);
}