#include "cub.h"
#include <stdlib.h>

t_solver_checkpoint	*clone_checkpoint(t_engine *eng)
{
	t_solver_checkpoint	*cp;
	size_t				blob_size;

	cp = malloc(sizeof(t_solver_checkpoint));
	if (!cp)
		return (NULL);
	cp->player = *eng->player;
	blob_size = ((t_blob_hdr *)eng->blob)->end_offset;
	cp->blob_copy = malloc(blob_size);
	if (!cp->blob_copy)
	{
		free(cp);
		return (NULL);
	}
	ft_memcpy(cp->blob_copy, eng->blob, blob_size);
	cp->alarm_triggered = eng->alarm_triggered;
	cp->hacking_timer = eng->hacking_timer;
	return (cp);
}

void	restore_checkpoint(t_engine *eng, t_solver_checkpoint *cp)
{
	size_t	blob_size;

	*eng->player = cp->player;
	blob_size = ((t_blob_hdr *)eng->blob)->end_offset;
	ft_memcpy(eng->blob, cp->blob_copy, blob_size);
	eng->alarm_triggered = cp->alarm_triggered;
	eng->hacking_timer = cp->hacking_timer;
}

void	free_checkpoint(t_solver_checkpoint *cp)
{
	if (cp)
	{
		free(cp->blob_copy);
		free(cp);
	}
}

void	init_dummy_textures(t_data *d)
{
	int				i;
	static uint32_t	dummy = 0;

	d->img_tab = ft_calloc(d->textures_len + 1, sizeof(t_img));
	if (!d->img_tab)
		return ;
	i = -1;
	while (++i < d->textures_len)
	{
		d->img_tab[i].width = 1;
		d->img_tab[i].height = 1;
		d->img_tab[i].line_len = 4;
		d->img_tab[i].bpp = 32;
		d->img_tab[i].addr = (char *)&dummy;
	}
}
