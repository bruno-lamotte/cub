#include "../../includes/cub.h"

void build_px(t_data *d, void *blob)
{
    t_blob_hdr *h = get_blob_hdr(blob);
    uint32_t *dest = (uint32_t *)((uint8_t *)blob + h->pixels_data.start);
    t_bdef_tex *tex_defs = get_tex_bdef(blob);
    int i = 0;
    
    while (i < d->textures_len)
    {
        if (!d->textures_defs[i].is_color)
        {
            t_img *src_img = &d->img_tab[i];
            uint32_t *tex_dest = (uint32_t *)((uint8_t *)dest + tex_defs[i].offset);
            int y = 0;
            while (y < src_img->height)
            {
                int x = 0;
                while (x < src_img->width)
                {
                    char *src_pixel = src_img->addr + (y * src_img->line_len + x * (src_img->bpp / 8));
                    tex_dest[y * src_img->width + x] = *(uint32_t *)src_pixel;
                    x++;
                }
                y++;
            }
        }
        i++;
    }
}

bool build(t_data *data, void *blob, t_tmp_header *tmp_header)
{
	if (!build_hdr(data, blob, tmp_header))
		return (printf("ERR 1\n"), false);
	build_rt(data, blob);
	build_bdef(data, blob);
	build_px(data, blob);
	return (true);
}

bool build_blob(t_data *d, void **blob)
{
	size_t	alloc_size;
	t_tmp_header tmp_header;
	if (!fill_tmp_hdr(d, &tmp_header) || !get_alloc_size(&tmp_header, &alloc_size))
		return (false);
	*blob = ft_calloc(1, alloc_size);
	if (!*blob)
		return (false);
	if (!build(d, *blob, &tmp_header))
		return (free(*blob), false);
	return (true);
}