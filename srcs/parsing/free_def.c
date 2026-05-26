#include "../../includes/cub.h"

void free_textures_def(t_texture_def **tex, size_t len)
{
    size_t i;

    i = 0;
    if (!(*tex))
        return ;
    while(i < len)
    {
        if (!(*tex)[i].is_color && (*tex)[i].path)
            free((*tex)[i].path);
        if ((*tex)[i].name)
            free((*tex)[i].name);
        i++;
    }
    free((*tex));
	*tex = NULL;
}

void free_all_def(t_data *d)
{
	if (!d)
		return ;
	if (d->textures_defs)
        free_textures_def(&d->textures_defs, d->textures_len);
	if (d->walls_defs)
		free(d->walls_defs);
	if (d->airs_defs)
		free(d->airs_defs);
	if (d->doors_defs)
		free(d->doors_defs);
	if (d->monsters_defs)
		free(d->monsters_defs);
	if (d->obj_defs)
		free(d->obj_defs);
	if (d->player_defs)
		free(d->player_defs);
	if (d->m_map)
		free_str_tab(d->m_map);
	if (d->b_map)
		free_str_tab(d->b_map);
}

void free_ids(t_data *d)
{
	if (!d)
		return ;
	if (d->all_ids)
		free(d->all_ids);
	if (d->wall_ids)
		free(d->wall_ids);
	if (d->air_ids)
		free(d->air_ids);
	if (d->door_ids)
		free(d->door_ids);
	if (d->monster_ids)
		free(d->monster_ids);
	if (d->obj_ids)
		free(d->obj_ids);
	if (d->player_ids)
		free(d->player_ids);
}

void free_preprocessing_data(t_data *d)
{
	free_all_def(d);
	free_ids(d);
}